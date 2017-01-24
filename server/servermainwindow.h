#ifndef SERVERMAINWINDOW_H
#define SERVERMAINWINDOW_H

#include "car.h"
#include "vehicle.h"
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
    void showProgress();
    void on_btnAccelerate_clicked();
    void on_btnDecelerate_clicked();

private:
    Ui::ServerMainWindow *ui;
    QDBusServer* m_dBusServer;
    QGraphicsScene* m_scene;
    Vehicle* m_vehicle;
};

#endif // SERVERMAINWINDOW_H
