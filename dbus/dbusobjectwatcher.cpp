#include "dbusobjectwatcher.h"
#include "dbusobjectwatcher_p.h"
#include <QDebug>
#include <QTimer>
#include <QDBusConnection>
#include <QDBusError>
#include <QDBusMessage>
#include <QThread>

DBusObjectWatcher::DBusObjectWatcher(const QString& connectionName, const QString& objectPath, QObject *parent)
    : QObject(parent)
    , d_ptr(new DBusObjectWatcherPrivate(connectionName, objectPath, this))
{
}

DBusObjectWatcher::~DBusObjectWatcher()
{
}

QString DBusObjectWatcher::connectionName()
{
    return d_ptr->m_connectionName;
}

bool DBusObjectWatcher::isObjectAvailable()
{
    Q_D(DBusObjectWatcher);
    return d->m_objectAvailable;
}

QDBusConnection DBusObjectWatcher::connection()
{
    return QDBusConnection(d_ptr->m_connectionName);
}

void DBusObjectWatcher::startWatching()
{
    Q_D(DBusObjectWatcher);
    d->startWatching();
}

void DBusObjectWatcher::stopWatching()
{
    Q_D(DBusObjectWatcher);
    d->stopWatching();
}

void DBusObjectWatcher::handleConnection()
{
    Q_D(DBusObjectWatcher);
    d->handleConnection();
}

void DBusObjectWatcher::handleDisconnection()
{
    Q_D(DBusObjectWatcher);
    d->handleDisconnection();
}

DBusObjectWatcherPrivate::DBusObjectWatcherPrivate(const QString &connectionName, const QString &objectPath, DBusObjectWatcher *parent)
    : QObject(parent)
    , q_ptr(parent)
    , m_objectPath(objectPath)
    , m_connectionName(connectionName)
    , m_enabled(false)
    , m_objectAvailable(false)
    , m_timer(new QTimer(this))
{
    connect(m_timer, &QTimer::timeout, this, &DBusObjectWatcherPrivate::monitorConnection);
}

DBusObjectWatcherPrivate::~DBusObjectWatcherPrivate()
{
    handleDisconnection();
}

void DBusObjectWatcherPrivate::startWatching()
{
    m_enabled = true;
    monitorConnection();
    m_timer->start(1000);
}

void DBusObjectWatcherPrivate::stopWatching()
{
    m_enabled = false;
    m_timer->stop();
    handleDisconnection();
}

void DBusObjectWatcherPrivate::monitorConnection()
{
    if (!m_enabled || m_connectionName.isEmpty() || m_objectPath.isEmpty())
    {
        return;
    }

    // check if remote object is still there
    if (m_objectAvailable)
    {
        // retrieve connection by name
        QDBusConnection connection(m_connectionName);
        if (!connection.isConnected())
        {
            qDebug() << "Disconnected from:" << m_connectionName << connection.lastError().message();
            handleDisconnection();
        }
    }

    // try to connect to remote object
    if (!m_objectAvailable)
    {
        // try to establish the connection
        QDBusConnection connection(m_connectionName);
        if (connection.isConnected())
        {
            qDebug() << "Watcher connected to:" << m_connectionName;

            QDBusMessage reply = connection.call(QDBusMessage::createMethodCall("", m_objectPath, "", "DummyMethodWhichIsNotImplemented"));
            qDebug() << "Reply:" << reply;
            if (reply.errorName().contains("UnknownMethod"))
            {
                // 'UnknownMethod' error means that object exists,
                // otherwise the error would be 'UnknownObject'
                m_objectAvailable = true;
                Q_Q(DBusObjectWatcher);
                Q_EMIT q->objectAdded();
            }
        }
    }
}

void DBusObjectWatcherPrivate::handleConnection()
{
    monitorConnection();
}

void DBusObjectWatcherPrivate::handleDisconnection()
{
    if (m_objectAvailable)
    {
        m_objectAvailable = false;
        Q_Q(DBusObjectWatcher);
        Q_EMIT q->objectRemoved();
    }
}
