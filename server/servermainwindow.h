#ifndef SERVERMAINWINDOW_H
#define SERVERMAINWINDOW_H

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

private:
    Ui::ServerMainWindow *ui;
    QDBusServer* m_dBusServer;
};

#endif // SERVERMAINWINDOW_H
