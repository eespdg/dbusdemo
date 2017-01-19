#ifndef CLIENTMAINWINDOW_H
#define CLIENTMAINWINDOW_H

#include "car_interface.h"
#include <QMainWindow>
#include <QtDBus/QtDBus>

namespace Ui {
class ClientMainWindow;
}

class ClientMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ClientMainWindow(QWidget *parent = 0);
    ~ClientMainWindow();

private Q_SLOTS:
    void handleServiceRegistration(const QString &serviceName);
    void tryConnect();


    void on_btnAccelerate_clicked();

    void on_btnDecelerate_clicked();

private:
    Ui::ClientMainWindow *ui;
    QDBusConnection m_connection;
    QScopedPointer<QDBusServiceWatcher> m_watcher;
    org::example::Examples::CarInterface *m_car;
};

#endif // CLIENTMAINWINDOW_H
