#ifndef DBUSMONITOR_H
#define DBUSMONITOR_H

#include <QObject>
#include <QScopedPointer>
#include <QDBusConnection>

class DBusMonitorPrivate;

class DBusMonitor : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(DBusMonitor)
    Q_CLASSINFO("D-Bus Interface", "com.barco.healthcare.DBusMonitorInterface")

public:
    explicit DBusMonitor(QObject *parent = 0);
    virtual ~DBusMonitor();
    void registerForConnection(QDBusConnection connection);
    static QString dBusInterfaceName();

Q_SIGNALS:
    Q_SCRIPTABLE void heartBeat(qint64 upTimeMilliseconds);

private:
    QScopedPointer<DBusMonitorPrivate> d_ptr;
};

#endif // DBUSMONITOR_H
