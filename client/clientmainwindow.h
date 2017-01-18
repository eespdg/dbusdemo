#ifndef CLIENTMAINWINDOW_H
#define CLIENTMAINWINDOW_H

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


private:


    QDBusConnection m_connection;
    QScopedPointer<QDBusServiceWatcher> m_watcher;
    Ui::ClientMainWindow *ui;
};

#endif // CLIENTMAINWINDOW_H
