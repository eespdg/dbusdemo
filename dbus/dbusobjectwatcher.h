#ifndef DBUSOBJECTWATCHER_H
#define DBUSOBJECTWATCHER_H

#include <QObject>
#include <QDBusConnection>
#include <QScopedPointer>

class DBusObjectWatcherPrivate;

class DBusObjectWatcher : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(DBusObjectWatcher)
    Q_DECLARE_PRIVATE(DBusObjectWatcher)

public:
    explicit DBusObjectWatcher(const QString& connectionName, const QString& serviceName, const QString& objectPath, QObject *parent = 0);
    virtual ~DBusObjectWatcher();
    QString connectionName();
    bool isObjectAvailable();
    QDBusConnection connection();

public Q_SLOTS:
    void startWatching();
    void stopWatching();
    void handleConnection();
    void handleDisconnection();

Q_SIGNALS:
    void objectAdded();
    void objectRemoved();

private:
    DBusObjectWatcherPrivate* d_ptr;
};

#endif // DBUSOBJECTWATCHER_H
