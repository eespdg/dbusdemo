#ifndef DBUSCLIENT_H
#define DBUSCLIENT_H

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
    explicit DBusClient(QObject *parent = 0);
    virtual ~DBusClient();
    QString serverAddress();
    QString connectionName();
    bool isConnected();
    QDBusConnection connection();

public Q_SLOTS:
    void connectToServer(const QString& serverAddress, const QString& connectionName);
    void connectToBus(QDBusConnection::BusType busType, const QString &connectionName);
    void disconnectFromServer();

Q_SIGNALS:
    void connectedToServer();
    void disconnectedFromServer();

private:
    DBusClientPrivate* d_ptr;
};

#endif // DBUSCLIENT_H
