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
    void monitorConnection();
    void startCommunication();
    void on_btnAccelerate_clicked();
    void on_btnDecelerate_clicked();

private:
    Ui::ClientMainWindow *ui;
    QScopedPointer<org::example::Examples::CarInterface> m_car;
    bool m_connected;
};

#endif // CLIENTMAINWINDOW_H
