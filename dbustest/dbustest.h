#ifndef DBUSTEST_H
#define DBUSTEST_H

#include "dbusclient.h"
#include "vehicle.h"
#include <QMainWindow>
#include <QDBusServer>

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
    void on_btnConnect_clicked();

    void on_cbConsume_clicked();

private:
    void disconnect();
    void addWatcher();
    void deleteWatcher();

    Ui::DBusTest *ui;
    DBusClient* m_client;
    DBusObjectWatcher* m_watcher;
    QDBusServer* m_server;
    Vehicle* m_vehicle;
};

#endif // DBUSTEST_H
