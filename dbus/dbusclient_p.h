#ifndef DBUSCLIENT_P_H
#define DBUSCLIENT_P_H

#include "dbusclient.h"
//#include "dbusmonitor_interface.h"
#include <QTimer>

class DBusClientPrivate : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(DBusClientPrivate)

public:
    DBusClientPrivate(const QString& connectionName, QObject* parent = Q_NULLPTR);

public Q_SLOTS:
    void connectToPeer(const QString &serverAddress);
    void connectToBus(const QString& serverAddress);
    void connectToBus(QDBusConnection::BusType busType);
    void disconnectFromServer();

private Q_SLOTS:
    void monitorConnection();

Q_SIGNALS:
    void connectedToServer();
    void disconnectedFromServer();

private:
    void handleDisconnection();

public:
    QDBusConnection::BusType m_busType;
    QString m_serverAddress; ///< connection string, e.g. "tcp:host=127.0.0.1,port=55555"
    QString m_connectionName; ///< unique name for the connection, e.g. "coreservice"
    bool m_connectToPeer;
    bool m_enabled;
    bool m_connected; ///< server connection state
    QTimer* m_timer;
};

#endif // DBUSCLIENT_P_H
