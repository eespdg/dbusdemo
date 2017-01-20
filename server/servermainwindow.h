#ifndef SERVERMAINWINDOW_H
#define SERVERMAINWINDOW_H

#include "car.h"
#include "dbussession.h"
#include <QMainWindow>
#include <QtDBus/QDBusServer>
#include <QtDBus/QDBusConnection>
#include <QGraphicsScene>

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
    void startDBusServer();
    void handleClientConnection(QDBusConnection connection);
    void startClientCommunication();
    void showProgress();
    void on_dump_clicked();

private:
    Ui::ServerMainWindow *ui;
    QDBusServer* m_dBusServer;
    QGraphicsScene* m_scene;
    Car* m_car;
    DBusSession* m_lastSession;
};

#endif // SERVERMAINWINDOW_H
