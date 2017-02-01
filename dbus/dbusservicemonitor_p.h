#ifndef DBUSSERVICEMONITOR_P_H
#define DBUSSERVICEMONITOR_P_H

#include "dbusservicemonitor.h"
#include <QObject>

class DBusServiceMonitorPrivate : public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(DBusServiceMonitor)
    Q_DISABLE_COPY(DBusServiceMonitorPrivate)

public:
    explicit DBusServiceMonitorPrivate(const QString& connectionName, const QString &serviceName, DBusServiceMonitor *parent);
    virtual ~DBusServiceMonitorPrivate();
    void checkService();

public Q_SLOTS:
    void handleServiceRegistered(QString serviceName, QString oldOwner, QString newOwner);

public:
    DBusServiceMonitor* q_ptr;
    QString m_serviceName;
    QString m_connectionName; ///< connection name used, e.g. "coreservice"
    bool m_enabled;
    bool m_serviceReported;
    bool m_serviceDetected;
};

#endif // DBUSSERVICEMONITOR_P_H
