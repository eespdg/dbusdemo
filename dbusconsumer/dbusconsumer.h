#ifndef DBUSCONSUMER_H
#define DBUSCONSUMER_H

#include "dbusclient.h"
#include <QMainWindow>

namespace Ui {
class DBusConsumer;
}

class DBusConsumer : public QMainWindow
{
    Q_OBJECT

public:
    explicit DBusConsumer(QWidget *parent = 0);
    ~DBusConsumer();

private Q_SLOTS:
    void handleConnection();
    void handleDisconnection();
    void handleObjectAdded();
    void handleObjectRemoved();

private:
    Ui::DBusConsumer *ui;
    DBusClient* m_client;
};

#endif // DBUSCONSUMER_H
