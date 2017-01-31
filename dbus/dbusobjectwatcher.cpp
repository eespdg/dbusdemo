#include "dbusobjectwatcher.h"
#include "dbusobjectwatcher_p.h"
#include <QDebug>
#include <QTimer>
#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QDBusError>
#include <QDBusMessage>
#include <QThread>

DBusObjectWatcher::DBusObjectWatcher(const QString& connectionName, const QString &serviceName, const QString& objectPath, QObject *parent)
    : QObject(parent)
    , d_ptr(new DBusObjectWatcherPrivate(connectionName, serviceName, objectPath, this))
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

DBusObjectWatcherPrivate::DBusObjectWatcherPrivate(const QString &connectionName, const QString &serviceName, const QString &objectPath, DBusObjectWatcher *parent)
    : QObject(parent)
    , q_ptr(parent)
    , m_serviceName(serviceName)
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
        else if (!connection.interface()->isServiceRegistered(m_serviceName))
        {
            qDebug() << "Remote service unregistered:" << m_serviceName;
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
//            const QDBusReply<QStringList> repl = connection.interface()->registeredServiceNames();
//            if (repl.isValid())
//            {
//                const QStringList list = repl.value();
//                qDebug() << list;
//                if (list.contains(m_serviceName))
//                    qDebug() << "SERVICE FOUND!";
//            }
//            else
//            {
//                qDebug() << repl.error();
//            }

            if (connection.interface()->isServiceRegistered(m_serviceName))
            {
                m_objectAvailable = true;
                Q_Q(DBusObjectWatcher);
                Q_EMIT q->objectAdded();
            }

//            QDBusMessage reply = connection.call(QDBusMessage::createMethodCall("org.freedesktop.DBus", "/org/freedesktop/DBus", "org.freedesktop.DBus", "ListNames"));
//            qDebug() << "Reply:" << reply;

//            QDBusMessage reply = connection.call(QDBusMessage::createMethodCall("", m_objectPath, "", "DummyMethodWhichIsNotImplemented"));
//            qDebug() << "Reply:" << reply;
//            if (reply.errorName().contains("UnknownMethod"))
//            {
//                // 'UnknownMethod' error means that object exists,
//                // otherwise the error would be 'UnknownObject'
//                m_objectAvailable = true;
//                Q_Q(DBusObjectWatcher);
//                Q_EMIT q->objectAdded();
//            }
        }
    }
}

void DBusObjectWatcherPrivate::handleConnection()
{
    qDebug() << "Watcher connected to:" << m_connectionName;
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
