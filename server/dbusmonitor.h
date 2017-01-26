#ifndef DBUSMONITOR_H
#define DBUSMONITOR_H

#include <QObject>
#include <QScopedPointer>
#include <QDBusConnection>

class DBusMonitorPrivate;

/**
 * @brief D-Bus object and interface used in D-Bus peer-to-peer server.
 *
 * An instance of this class shall be created for every instance of QDBusServer.
 *
 * This object periodically emits a signal with current timestamp which
 * will be received by all connected clients on the D-Bus.
 *
 * On client connection, the server shall register the D-Bus objects and
 * call registerForConnection to signal that the client can start using
 * any exposed interface.
 */
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
