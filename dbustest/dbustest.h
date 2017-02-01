#ifndef DBUSTEST_H
#define DBUSTEST_H

#include "dbusclient.h"
#include "vehicle.h"
#include "vehicle_interface.h"
#include <QMainWindow>
#include <QDBusServer>
//#include <QDBusServiceWatcher>

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
    void handleServiceAdded();
    void handleServiceRemoved();
    void handleObjectAdded();
    void handleObjectRemoved();
    void handleServiceRegistered();
    void on_btnConnect_clicked();
    void on_cbConsume_clicked();
    void on_btnTestCon_clicked();
    void on_btnTestDisconn_clicked();
    void on_btnShow_clicked();

private:
    void disconnect();
    void addConsumer();
    void deleteConsumer();

    Ui::DBusTest *ui;
    DBusClient* m_client;
    DBusObjectWatcher* m_watcher;
    DBusServiceMonitor* m_serviceMonitor;
    QDBusServer* m_server;
    Vehicle* m_vehicle;
    org::example::VehicleInterface* m_vehicleProxy;
};

#endif // DBUSTEST_H
