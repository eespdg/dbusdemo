#include "dbusservicemonitor.h"
#include "dbusservicemonitor_p.h"
#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QDebug>

DBusServiceMonitor::DBusServiceMonitor(const QString &connectionName, const QString &serviceName, QObject *parent)
    : QObject(parent)
    , d_ptr(new DBusServiceMonitorPrivate(connectionName, serviceName, this))
{
}

DBusServiceMonitor::~DBusServiceMonitor()
{

}

bool DBusServiceMonitor::isServiceAvailable()
{
    Q_D(DBusServiceMonitor);
    return d->m_serviceReported;
}

void DBusServiceMonitor::startWatching()
{
    Q_D(DBusServiceMonitor);
    d->m_enabled = true;
    d->checkService();
}

void DBusServiceMonitor::stopWatching()
{
    Q_D(DBusServiceMonitor);
    d->m_enabled = false;
    d->checkService();
}

void DBusServiceMonitor::handleConnection()
{
    bool detected = false;
    Q_D(DBusServiceMonitor);
    QDBusConnection connection(d->m_connectionName);
    if (connection.isConnected())
    {
        connection.connect("org.freedesktop.DBus", "/org/freedesktop/DBus",
                           "org.freedesktop.DBus", "NameOwnerChanged",
                           d, SLOT(handleServiceRegistered(QString, QString, QString)));

        if (connection.interface()->isServiceRegistered(d->m_serviceName))
        {
            qDebug() << "Service detected during connection:" << d->m_serviceName;
            detected = true;
        }
    }
    else
    {
        qWarning() << "Not connected";
    }
    d->m_serviceDetected = detected;
    d->checkService();
}

void DBusServiceMonitor::handleDisconnection()
{
    Q_D(DBusServiceMonitor);
    d->m_serviceDetected = false;
    d->checkService();
}

DBusServiceMonitorPrivate::DBusServiceMonitorPrivate(const QString &connectionName, const QString &serviceName, DBusServiceMonitor *parent)
    : QObject(parent)
    , q_ptr(parent)
    , m_serviceName(serviceName)
    , m_connectionName(connectionName)
    , m_enabled(false)
    , m_serviceReported(false)
    , m_serviceDetected(false)
{

}

DBusServiceMonitorPrivate::~DBusServiceMonitorPrivate()
{

}

void DBusServiceMonitorPrivate::checkService()
{
    if (m_enabled && m_serviceDetected)
    {
        if (!m_serviceReported)
        {
            m_serviceReported = true;
            Q_Q(DBusServiceMonitor);
            Q_EMIT q->serviceAcquired();
        }
    }
    else
    {
        if (m_serviceReported)
        {
            m_serviceReported = false;
            Q_Q(DBusServiceMonitor);
            Q_EMIT q->serviceLost();
        }
    }
}

void DBusServiceMonitorPrivate::handleServiceRegistered(QString serviceName, QString oldOwner, QString newOwner)
{
    qDebug() << "Handle service registered:" << serviceName;
    if (serviceName == m_serviceName)
    {
        if (newOwner.isEmpty())
        {
            m_serviceDetected = false;
        }
        else
        {
            m_serviceDetected = true;
        }
        qDebug() << "Service registered: " << serviceName << m_serviceDetected;
        checkService();
    }
}
