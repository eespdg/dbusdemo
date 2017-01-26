#ifndef DBUSCLIENTCONNECTION_H
#define DBUSCLIENTCONNECTION_H

#include <QObject>
#include <QDBusConnection>
#include <QScopedPointer>

class DBusClientConnectionPrivate;

class DBusClientConnection : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(DBusClientConnection)
    Q_DECLARE_PRIVATE(DBusClientConnection)

public:
    explicit DBusClientConnection(QObject* parent = Q_NULLPTR);
    virtual ~DBusClientConnection();
    QString serverAddress();
    QString connectionName();
    bool isConnected();
    QDBusConnection connection();

public Q_SLOTS:
    void connectToServer(const QString& serverAddress, const QString& connectionName);
    void disconnectFromServer();

Q_SIGNALS:
    void connectedToServer(QDBusConnection connection);
    void disconnectedFromServer(QString connectionName);
    void heartBeat(qint64 milliseconds);

private:
    QScopedPointer<DBusClientConnectionPrivate> d_ptr;
};

#endif // DBUSCLIENTCONNECTION_H
