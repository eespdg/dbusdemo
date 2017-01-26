#include "dbusclientconnection.h"
#include "dbusclientconnection_p.h"
#include <QDebug>
#include <QTimer>
#include <QDBusConnection>
#include <QDBusError>

DBusClientConnection::DBusClientConnection(QObject *parent)
    : QObject(parent)
    , d_ptr(new DBusClientConnectionPrivate())
{
    connect(d_ptr.data(), &DBusClientConnectionPrivate::connectedToServer,
            this, &DBusClientConnection::connectedToServer);
    connect(d_ptr.data(), &DBusClientConnectionPrivate::disconnectedFromServer,
            this, &DBusClientConnection::disconnectedFromServer);
    connect(d_ptr.data(), &DBusClientConnectionPrivate::heartBeat,
            this, &DBusClientConnection::heartBeat);
}

DBusClientConnection::~DBusClientConnection()
{
}

QString DBusClientConnection::serverAddress()
{
    return d_ptr->m_serverAddress;
}

QString DBusClientConnection::connectionName()
{
    return d_ptr->m_connectionName;
}

bool DBusClientConnection::isConnected()
{
    return d_ptr->m_ready;
}

QDBusConnection DBusClientConnection::connection()
{
    return QDBusConnection(d_ptr->m_connectionName);
}

void DBusClientConnection::connectToServer(const QString &serverAddress, const QString &connectionName)
{
    d_ptr->connectToServer(serverAddress, connectionName);
}

void DBusClientConnection::disconnectFromServer()
{
    d_ptr->disconnectFromServer();
}

DBusClientConnectionPrivate::DBusClientConnectionPrivate()
    : m_serverAddress()
    , m_connectionName()
    , m_connected(false)
    , m_ready(false)
    , m_timer(new QTimer(this))
    , m_dBusMonitor(Q_NULLPTR)
{
    connect(m_timer, &QTimer::timeout, this, &DBusClientConnectionPrivate::monitorConnection);
}

void DBusClientConnectionPrivate::connectToServer(const QString &serverAddress, const QString &connectionName)
{
    disconnectFromServer();

    if (serverAddress.isEmpty() || connectionName.isEmpty())
        return;

    m_serverAddress = serverAddress;
    m_connectionName = connectionName;
    monitorConnection();
    m_timer->start(1000);
}

void DBusClientConnectionPrivate::disconnectFromServer()
{
    m_timer->stop();
    if (!m_serverAddress.isEmpty() && !m_connectionName.isEmpty())
    {
        QDBusConnection::disconnectFromPeer(m_connectionName);
    }
    m_serverAddress.clear();
    m_connectionName.clear();
    m_connected = false;
    if (m_ready)
    {
        m_ready = false;
        Q_EMIT disconnectedFromServer(m_connectionName);
    }
}

void DBusClientConnectionPrivate::monitorConnection()
{
    // check the current connection
    if (m_connected)
    {
        // retrieve connection by name
        QDBusConnection connection(m_connectionName);
        if (!connection.isConnected())
        {
            qDebug() << "Disconnected from:" << m_connectionName << connection.lastError().message();
            m_connected = false;
            if (m_ready)
            {
                m_ready = false;
                Q_EMIT disconnectedFromServer(m_connectionName);
            }
        }
    }

    // try to connect
    if (!m_connected)
    {
        // try to establish the connection
        QDBusConnection connection = QDBusConnection::connectToPeer(m_serverAddress, m_connectionName);
        if (connection.isConnected())
        {
            qDebug() << "Connected to:" << m_connectionName;
            m_dBusMonitor = new com::barco::healthcare::DBusMonitorInterface("", "/DBusMonitor", connection, this);
            connect(m_dBusMonitor, &com::barco::healthcare::DBusMonitorInterface::heartBeat,
                    this, &DBusClientConnectionPrivate::receiveHeartBeat);
            m_connected = true;
        }
        else
        {
            qDebug() << "Error connecting to:" << m_connectionName << connection.lastError().message();
            QDBusConnection::disconnectFromPeer(m_connectionName);
        }
    }
}

void DBusClientConnectionPrivate::receiveHeartBeat(qint64 milliseconds)
{
    Q_EMIT heartBeat(milliseconds);

    if (!m_ready)
    {
        m_ready = true;
        Q_EMIT connectedToServer(QDBusConnection(m_connectionName));
    }
}
