#ifndef SERVERMAINWINDOW_H
#define SERVERMAINWINDOW_H

#include "vehicle.h"
#include "dbusmonitor.h"
#include <QMainWindow>
#include <QtDBus/QDBusServer>
#include <QtDBus/QDBusConnection>

namespace Ui {
class ServerMainWindow;
}

class ServerMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ServerMainWindow(QWidget *parent = 0);
    ~ServerMainWindow();

private Q_SLOTS:
    void handleClientConnection(QDBusConnection connection);
    void registerObjects(QDBusConnection connection);
    void showProgress();
    void on_btnAccelerate_clicked();
    void on_btnDecelerate_clicked();

private:
    Ui::ServerMainWindow *ui;
    QDBusServer* m_dBusServer;
    Vehicle* m_vehicle;
    DBusMonitor m_dBusMonitor;
};

#endif // SERVERMAINWINDOW_H
