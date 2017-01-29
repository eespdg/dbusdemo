#include "dbusconsumer.h"
#include "ui_dbusconsumer.h"

DBusConsumer::DBusConsumer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DBusConsumer)
    , m_client(new DBusClient("core", this))
{
    ui->setupUi(this);

    connect(m_client, &DBusClient::connectedToServer, this, &DBusConsumer::handleConnection);
    connect(m_client, &DBusClient::disconnectedFromServer, this, &DBusConsumer::handleDisconnection);

    DBusObjectWatcher* vehicleWatcher = m_client->createObjectWatcher("/Vehicle");
    connect(vehicleWatcher, &DBusObjectWatcher::objectAdded, this, &DBusConsumer::handleObjectAdded);
    connect(vehicleWatcher, &DBusObjectWatcher::objectRemoved, this, &DBusConsumer::handleObjectRemoved);
    vehicleWatcher->startWatching();

    m_client->connectToServer("tcp:host=127.0.0.1,port=55555");
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

void DBusConsumer::handleObjectAdded()
{
    ui->plainTextEdit->appendPlainText("Object ADDED");
}

void DBusConsumer::handleObjectRemoved()
{
    ui->plainTextEdit->appendPlainText("Object REMOVED");
}
