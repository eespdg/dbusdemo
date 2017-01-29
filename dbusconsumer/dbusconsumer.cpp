#include "dbusconsumer.h"
#include "ui_dbusconsumer.h"

DBusConsumer::DBusConsumer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DBusConsumer)
    , m_client(new DBusClient(this))
{
    ui->setupUi(this);

    connect(m_client, &DBusClient::connectedToServer, this, &DBusConsumer::handleConnection);
    connect(m_client, &DBusClient::disconnectedFromServer, this, &DBusConsumer::handleDisconnection);
    m_client->connectToServer("tcp:host=127.0.0.1,port=55555", "core");

}

DBusConsumer::~DBusConsumer()
{
    delete ui;
}

void DBusConsumer::handleConnection()
{
    ui->plainTextEdit->appendPlainText("CONNECTED");
}

void DBusConsumer::handleDisconnection()
{
    ui->plainTextEdit->appendPlainText("DISCONNECTED");
}
