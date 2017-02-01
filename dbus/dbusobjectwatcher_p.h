#ifndef DBUSOBJECTWATCHER_P_H
#define DBUSOBJECTWATCHER_P_H

#include "dbusobjectwatcher.h"
//#include "dbusmonitor_interface.h"
#include <QTimer>

class DBusObjectWatcherPrivate : public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(DBusObjectWatcher)
    Q_DISABLE_COPY(DBusObjectWatcherPrivate)

public:
    DBusObjectWatcherPrivate(const QString& connectionName, const QString &serviceName, const QString& objectPath, DBusObjectWatcher *parent);
    virtual ~DBusObjectWatcherPrivate();
    void startWatching();
    void stopWatching();

private Q_SLOTS:
    void monitorConnection();

private:
    void handleConnection();
    void handleDisconnection();

public:
    DBusObjectWatcher* q_ptr;
    QString m_serviceName;
    QString m_objectPath; ///< watched object path, e.g. "/logger"
    QString m_connectionName; ///< connection name used, e.g. "coreservice"
    bool m_enabled;
    bool m_connected;
    bool m_objectAvailable; ///< remote object availability
    QTimer* m_timer;
};

#endif // DBUSOBJECTWATCHER_P_H
