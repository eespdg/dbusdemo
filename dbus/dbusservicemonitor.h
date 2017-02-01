#ifndef DBUSSERVICEMONITOR_H
#define DBUSSERVICEMONITOR_H

#include <QObject>

class DBusServiceMonitorPrivate;

class DBusServiceMonitor : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(DBusServiceMonitor)
    Q_DECLARE_PRIVATE(DBusServiceMonitor)

public:
    explicit DBusServiceMonitor(const QString& connectionName, const QString& serviceName, QObject *parent = 0);
    virtual ~DBusServiceMonitor();
    bool isServiceAvailable();

public Q_SLOTS:
    void startWatching();
    void stopWatching();
    void handleConnection();
    void handleDisconnection();

Q_SIGNALS:
    void serviceAcquired();
    void serviceLost();

private:
    DBusServiceMonitorPrivate* d_ptr;
};

#endif // DBUSSERVICEMONITOR_H
