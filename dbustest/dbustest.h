#ifndef DBUSTEST_H
#define DBUSTEST_H

#include "dbusclient.h"
#include "vehicle.h"
#include <QMainWindow>
#include <QDBusServer>
#include <QDBusServiceWatcher>

namespace Ui {
class DBusTest;
}

class DBusTest : public QMainWindow
{
    Q_OBJECT

public:
    explicit DBusTest(QWidget *parent = 0);
    ~DBusTest();

private slots:
    void handleServerConnection(QDBusConnection connection);
    void handleClientConnection();
    void handleClientDisconnection();
    void handleObjectAdded();
    void handleObjectRemoved();
    void handleServiceRegistered();
    void on_btnConnect_clicked();

    void on_cbConsume_clicked();

    void on_btnTestCon_clicked();

    void on_btnTestDisconn_clicked();

private:
    void disconnect();
    void addWatcher();
    void deleteWatcher();

    Ui::DBusTest *ui;
    DBusClient* m_client;
    DBusObjectWatcher* m_watcher;
    QDBusServiceWatcher* m_serviceWatcher;
    QDBusServer* m_server;
    Vehicle* m_vehicle;
};

#endif // DBUSTEST_H
