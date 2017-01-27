#ifndef DBUSCLIENTCONNECTION_P_H
#define DBUSCLIENTCONNECTION_P_H

#include "dbusclientconnection.h"
#include "dbusmonitor_interface.h"
#include <QTimer>

class DBusClientConnectionPrivate : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(DBusClientConnectionPrivate)

public:
    DBusClientConnectionPrivate(QObject* parent = Q_NULLPTR);

public Q_SLOTS:
    void connectToServer(const QString &serverAddress, const QString &connectionName);
    void connectToBus(QDBusConnection::BusType busType, const QString &connectionName);
    void disconnectFromServer();

private Q_SLOTS:
    void monitorConnection();
    void receiveHeartBeat(qint64 milliseconds);
    void handleNameOwnerChange(QString service, QString before, QString after);

Q_SIGNALS:
    void connectedToServer();
    void disconnectedFromServer();
    void heartBeat(qint64 milliseconds);

private:
    void handleDisconnection();

public:
    QDBusConnection::BusType m_busType;
    QString m_serverAddress; ///< connection string, e.g. "tcp:host=127.0.0.1,port=55555"
    QString m_connectionName; ///< unique name for the connection, e.g. "coreservice"
    bool m_connected; ///< server connection state
    bool m_ready; ///< ready to communicate to server
    QTimer* m_timer;
    com::barco::healthcare::DBusMonitorInterface* m_dBusMonitor;
};

#endif // DBUSCLIENTCONNECTION_P_H
