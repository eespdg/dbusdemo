#include "dbusclient.h"
#include "dbusclient_p.h"
#include <QDebug>
#include <QTimer>
#include <QDBusConnection>
#include <QDBusError>
#include <QThread>

DBusClient::DBusClient(QObject *parent)
    : QObject(parent)
    , d_ptr(Q_NULLPTR)
{
    QThread* thread = new QThread(this);
    d_ptr = new DBusClientPrivate();
    d_ptr->moveToThread(thread);
    connect(thread, &QThread::finished, d_ptr, &DBusClientPrivate::deleteLater);

    connect(d_ptr, &DBusClientPrivate::connectedToServer,
            this, &DBusClient::connectedToServer);
    connect(d_ptr, &DBusClientPrivate::disconnectedFromServer,
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

void DBusClient::connectToServer(const QString &serverAddress, const QString &connectionName)
{
    QMetaObject::invokeMethod(d_ptr, "connectToServer", Qt::QueuedConnection, Q_ARG(QString, serverAddress), Q_ARG(QString, connectionName));
}

void DBusClient::connectToBus(QDBusConnection::BusType busType, const QString &connectionName)
{
    static int s_reg = qRegisterMetaType<QDBusConnection::BusType>("QDBusConnection::BusType");

    QMetaObject::invokeMethod(d_ptr, "connectToBus", Qt::QueuedConnection, Q_ARG(QDBusConnection::BusType, busType), Q_ARG(QString, connectionName));
}

void DBusClient::disconnectFromServer()
{
    d_ptr->disconnectFromServer();
}

DBusClientPrivate::DBusClientPrivate(QObject *parent)
    : QObject(parent)
    , m_busType(QDBusConnection::SystemBus)
    , m_serverAddress()
    , m_connectionName()
    , m_connected(false)
    , m_timer(new QTimer(this))
{
    connect(m_timer, &QTimer::timeout, this, &DBusClientPrivate::monitorConnection);
}

void DBusClientPrivate::connectToServer(const QString &serverAddress, const QString &connectionName)
{
    disconnectFromServer();

    if (serverAddress.isEmpty() || connectionName.isEmpty())
        return;

    m_serverAddress = serverAddress;
    m_connectionName = connectionName;

    monitorConnection();
    m_timer->start(1000);
}

void DBusClientPrivate::connectToBus(QDBusConnection::BusType busType, const QString &connectionName)
{
    disconnectFromServer();

    if (connectionName.isEmpty())
        return;

    m_busType = busType;
    m_connectionName = connectionName;

    monitorConnection();
    m_timer->start(1000);
}

void DBusClientPrivate::disconnectFromServer()
{
    m_timer->stop();
    if (!m_connectionName.isEmpty())
    {
        if (m_serverAddress.isEmpty())
        {
            QDBusConnection::disconnectFromBus(m_connectionName);
        }
        else
        {
            QDBusConnection::disconnectFromPeer(m_connectionName);
        }
    }
    m_serverAddress.clear();
    m_connectionName.clear();
    handleDisconnection();
}

void DBusClientPrivate::monitorConnection()
{
    if (m_connectionName.isEmpty())
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
            m_serverAddress.isEmpty() ?
                    QDBusConnection::connectToBus(m_busType, m_connectionName) :
                    QDBusConnection::connectToPeer(m_serverAddress, m_connectionName) ;

        if (connection.isConnected())
        {
            qDebug() << "Connected to:" << m_connectionName;
            m_connected = true;
            Q_EMIT connectedToServer();
        }
        else
        {
            qDebug() << "Error connecting to:" << m_connectionName << connection.lastError().message();
            if (m_serverAddress.isEmpty())
            {
                QDBusConnection::disconnectFromBus(m_connectionName);
            }
            else
            {
                QDBusConnection::disconnectFromPeer(m_connectionName);
            }
        }
    }
}

void DBusClientPrivate::handleDisconnection()
{
    if (m_connected)
    {
        m_connected = false;
        Q_EMIT disconnectedFromServer();
    }
}
