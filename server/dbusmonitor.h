#ifndef DBUSMONITOR_H
#define DBUSMONITOR_H

#include <QObject>
#include <QElapsedTimer>
#include <QDBusConnection>

class DBusMonitor : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.barco.healthcare.DBusMonitorInterface")

public:
    explicit DBusMonitor(QObject *parent = 0);
    void registerForConnection(QDBusConnection connection);

Q_SIGNALS:
    Q_SCRIPTABLE void heartBeat(qint64 upTimeMilliseconds);

private:
    QElapsedTimer m_elapsedTimer;
};

#endif // DBUSMONITOR_H
