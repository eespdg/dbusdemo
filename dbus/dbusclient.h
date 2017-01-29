#ifndef DBUSCLIENT_H
#define DBUSCLIENT_H

#include "dbusobjectwatcher.h"
#include <QObject>
#include <QDBusConnection>
#include <QScopedPointer>

class DBusClientPrivate;

class DBusClient : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(DBusClient)
    Q_DECLARE_PRIVATE(DBusClient)

public:
    explicit DBusClient(const QString& connectionName, QObject *parent = 0);
    virtual ~DBusClient();
    QString serverAddress();
    QString connectionName();
    bool isConnected();
    QDBusConnection connection();
    DBusObjectWatcher* createObjectWatcher(const QString& objectPath);

public Q_SLOTS:
    void connectToServer(const QString& serverAddress);
    void connectToBus(QDBusConnection::BusType busType);
    void disconnectFromServer();

Q_SIGNALS:
    void connectedToServer();
    void disconnectedFromServer();

private:
    DBusClientPrivate* d_ptr;
};

#endif // DBUSCLIENT_H
