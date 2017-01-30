#include "dbusclient.h"
#include "dbusclient_p.h"
#include <QDebug>
#include <QTimer>
#include <QDBusConnection>
#include <QDBusError>
#include <QThread>

DBusClient::DBusClient(const QString& connectionName, QObject *parent)
    : QObject(parent)
    , d_ptr(new DBusClientPrivate(connectionName))
{
    Q_D(DBusClient);
    QThread* thread = new QThread(this);
    d->moveToThread(thread);
    connect(thread, &QThread::finished, d, &DBusClientPrivate::deleteLater);

    connect(d, &DBusClientPrivate::connectedToServer,
            this, &DBusClient::connectedToServer);
    connect(d, &DBusClientPrivate::disconnectedFromServer,
            this, &DBusClient::disconnectedFromServer);

    thread->start();
}

DBusClient::~DBusClient()
{
    d_ptr->thread()->quit();
    d_ptr->thread()->wait();
}

QString DBusClient::serverAddress()
{
    return d_ptr->m_serverAddress;
}

QString DBusClient::connectionName()
{
    return d_ptr->m_connectionName;
}

bool DBusClient::isConnected()
{
    return d_ptr->m_connected;
}

QDBusConnection DBusClient::connection()
{
    return QDBusConnection(d_ptr->m_connectionName);
}

DBusObjectWatcher *DBusClient::createObjectWatcher(const QString &objectPath)
{
    Q_D(DBusClient);
    DBusObjectWatcher* watcher = new DBusObjectWatcher(d->m_connectionName, objectPath, this);
    connect(this, &DBusClient::connectedToServer, watcher, &DBusObjectWatcher::handleConnection);
    connect(this, &DBusClient::disconnectedFromServer, watcher, &DBusObjectWatcher::handleDisconnection);
    return watcher;
}

void DBusClient::connectToPeer(const QString &serverAddress)
{
    QMetaObject::invokeMethod(d_ptr, "connectToPeer", Qt::QueuedConnection, Q_ARG(QString, serverAddress));
}

void DBusClient::connectToBus(const QString &serverAddress)
{
    QMetaObject::invokeMethod(d_ptr, "connectToBus", Qt::QueuedConnection, Q_ARG(QString, serverAddress));
}

void DBusClient::connectToBus(QDBusConnection::BusType busType)
{
    static int s_reg = qRegisterMetaType<QDBusConnection::BusType>("QDBusConnection::BusType");
    QMetaObject::invokeMethod(d_ptr, "connectToBus", Qt::QueuedConnection, Q_ARG(QDBusConnection::BusType, busType));
}

void DBusClient::disconnectFromServer()
{
    d_ptr->disconnectFromServer();
}

DBusClientPrivate::DBusClientPrivate(const QString &connectionName, QObject *parent)
    : QObject(parent)
    , m_busType(QDBusConnection::SystemBus)
    , m_serverAddress()
    , m_connectionName(connectionName)
    , m_connectToPeer(false)
    , m_enabled(false)
    , m_connected(false)
    , m_timer(new QTimer(this))
{
    connect(m_timer, &QTimer::timeout, this, &DBusClientPrivate::monitorConnection);
}

void DBusClientPrivate::connectToPeer(const QString &serverAddress)
{
    disconnectFromServer();

    if (serverAddress.isEmpty())
        return;

    m_enabled = true;
    m_connectToPeer = true;
    m_serverAddress = serverAddress;

    monitorConnection();
    m_timer->start(1000);
}

void DBusClientPrivate::connectToBus(const QString &serverAddress)
{
    disconnectFromServer();

    m_enabled = true;
    m_connectToPeer = false;
    m_serverAddress= serverAddress;

    monitorConnection();
    m_timer->start(1000);
}

void DBusClientPrivate::connectToBus(QDBusConnection::BusType busType)
{
    disconnectFromServer();

    m_enabled = true;
    m_connectToPeer = false;
    m_busType = busType;

    monitorConnection();
    m_timer->start(1000);
}

void DBusClientPrivate::disconnectFromServer()
{
    m_timer->stop();
    m_enabled = false;
    if (m_connectToPeer)
    {
        QDBusConnection::disconnectFromPeer(m_connectionName);
    }
    else
    {
        QDBusConnection::disconnectFromBus(m_connectionName);
    }
    m_serverAddress.clear();
    handleDisconnection();
}

void DBusClientPrivate::monitorConnection()
{
    if (!m_enabled || m_connectionName.isEmpty())
    {
        return;
    }

    // check the current connection
    if (m_connected)
    {
        // retrieve connection by name
        QDBusConnection connection(m_connectionName);
        if (!connection.isConnected())
        {
            qDebug() << "Disconnected from:" << m_connectionName << connection.lastError().message();
            handleDisconnection();
        }
    }

    // try to connect
    if (!m_connected)
    {
        // try to establish the connection
        QDBusConnection connection =
            m_connectToPeer ?
            QDBusConnection::connectToPeer(m_serverAddress, m_connectionName) :
            m_serverAddress.isEmpty() ?
            QDBusConnection::connectToBus(m_busType, m_connectionName) :
            QDBusConnection::connectToBus(m_serverAddress, m_connectionName) ;

        if (connection.isConnected())
        {
            qDebug() << "Connected to:" << m_connectionName;
            m_connected = true;
            Q_EMIT connectedToServer();
        }
        else
        {
            qDebug() << "Error connecting to:" << m_connectionName << connection.lastError().message();
            if (m_connectToPeer)
            {
                QDBusConnection::disconnectFromPeer(m_connectionName);
            }
            else
            {
                QDBusConnection::disconnectFromBus(m_connectionName);
            }
        }
    }
}

void DBusClientPrivate::handleDisconnection()
{
    if (m_connectToPeer)
    {
        QDBusConnection::disconnectFromPeer(m_connectionName);
    }
    else
    {
        QDBusConnection::disconnectFromBus(m_connectionName);
    }

    if (m_connected)
    {
        m_connected = false;
        Q_EMIT disconnectedFromServer();
    }
}
