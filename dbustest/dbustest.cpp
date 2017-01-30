#include "dbustest.h"
#include "ui_dbustest.h"
#include "vehicle_adaptor.h"

DBusTest::DBusTest(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DBusTest)
    , m_client(new DBusClient("core", this))
    , m_watcher(Q_NULLPTR)
    , m_server(Q_NULLPTR)
    , m_vehicle(new Vehicle(this))
{
    ui->setupUi(this);

    connect(m_client, &DBusClient::connectedToServer, this, &DBusTest::handleClientConnection);
    connect(m_client, &DBusClient::disconnectedFromServer, this, &DBusTest::handleClientConnection);

    new VehicleInterfaceAdaptor(m_vehicle);
}

DBusTest::~DBusTest()
{
    delete ui;
}

void DBusTest::handleServerConnection(QDBusConnection connection)
{
    if (ui->cbProvide)
    {
        connection.registerObject(ui->txtProvPath->text(), m_vehicle);
        connection.registerService("com.barco.healthcare.driving");
    }
}

void DBusTest::handleClientConnection()
{
    ui->lblConnStatus->setText("CONNECTED");
}

void DBusTest::handleClientDisconnection()
{
    ui->lblConnStatus->setText("DISCONNECTED");
}

void DBusTest::handleObjectAdded()
{
    ui->lblConsStatus->setText("OBJECT ADDED");
//    ui->lblConsStatus->setStyleSheet("border: 2px solid green;");
    ui->lblConsStatus->setStyleSheet("background-color: lightgreen;");
}

void DBusTest::handleObjectRemoved()
{
    ui->lblConsStatus->setText("OBJECT REMOVED");
    ui->lblConsStatus->setStyleSheet("background-color: lightgray;");
}

void DBusTest::on_btnConnect_clicked()
{
    if (ui->btnConnect->text() == "Connect")
    {
        ui->btnConnect->setText("Disconnect");
        ui->lblConnStatus->setText("CONNECTING");

        if (ui->cbConsume->isChecked())
        {
            addWatcher();
        }

        if (ui->rbSystemBus->isChecked())
        {
            m_client->connectToBus(QDBusConnection::SystemBus);
        }
        else if (ui->rbSessionBus->isChecked())
        {
            m_client->connectToBus(QDBusConnection::SessionBus);
        }
        else if (ui->rbPrivateBus->isChecked())
        {
            m_client->connectToBus(ui->txtPrivateBus->text());
        }
        else if (ui->rbPeer->isChecked())
        {
            m_client->connectToPeer(ui->txtPeer->text());
        }
        else if (ui->rbServer->isChecked())
        {
            m_server = new QDBusServer(ui->txtPeer->text(), this);
            connect(m_server, &QDBusServer::newConnection, this, &DBusTest::handleServerConnection);
        }
    }
    else
    {
        ui->btnConnect->setText("Connect");
        ui->lblConnStatus->setText("DISCONNECTING");

        disconnect();
    }
}

void DBusTest::disconnect()
{
    m_client->disconnectFromServer();
    deleteWatcher();
    if (m_server)
    {
        delete m_server;
        m_server = Q_NULLPTR;
    }
}

void DBusTest::addWatcher()
{
    deleteWatcher();
    m_watcher = m_client->createObjectWatcher(ui->txtConsPath->text());
    connect(m_watcher, &DBusObjectWatcher::objectAdded, this, &DBusTest::handleObjectAdded);
    connect(m_watcher, &DBusObjectWatcher::objectRemoved, this, &DBusTest::handleObjectRemoved);
    m_watcher->startWatching();
}

void DBusTest::deleteWatcher()
{
    if (m_watcher)
    {
        m_watcher->stopWatching();
        delete m_watcher;
        m_watcher = Q_NULLPTR;
    }
}

void DBusTest::on_cbConsume_clicked()
{
    if (ui->btnConnect->text() == "Disconnect")
    {
        // we are connected
        if (ui->cbConsume->isChecked())
        {
            addWatcher();
        }
        else
        {
            deleteWatcher();
        }
    }
}
