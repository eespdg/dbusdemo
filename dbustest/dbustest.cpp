#include "dbustest.h"
#include "ui_dbustest.h"
#include "vehicle_adaptor.h"

DBusTest::DBusTest(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DBusTest)
    , m_client(new DBusClient("core", this))
    , m_watcher(Q_NULLPTR)
//    , m_serviceWatcher(Q_NULLPTR)
    , m_server(Q_NULLPTR)
    , m_vehicle(new Vehicle(this))
{
    ui->setupUi(this);

    connect(m_client, &DBusClient::connectedToServer, this, &DBusTest::handleClientConnection);
    connect(m_client, &DBusClient::disconnectedFromServer, this, &DBusTest::handleClientDisconnection);

    new VehicleInterfaceAdaptor(m_vehicle);

}

DBusTest::~DBusTest()
{
    delete ui;
}

void DBusTest::handleServerConnection(QDBusConnection connection)
{
    if (ui->cbProvide->isChecked())
    {
        connection.registerObject(ui->txtProvPath->text(), m_vehicle);
        connection.registerService(ui->txtProvService->text());
    }
}

void DBusTest::handleClientConnection()
{
    ui->plainTextEdit->appendPlainText("Connected");
    ui->lblConnStatus->setText("CONNECTED");
    if (ui->cbProvide->isChecked())
    {
        QDBusConnection connection = m_client->connection();
        connection.registerObject(ui->txtProvPath->text(), m_vehicle);
        connection.registerService(ui->txtProvService->text());
    }

//    m_serviceWatcher = new QDBusServiceWatcher("com.barco.healthcare.driving", m_client->connection());
//    connect(m_serviceWatcher, &QDBusServiceWatcher::serviceRegistered, this, &DBusTest::handleServiceRegistered);

}

void DBusTest::handleClientDisconnection()
{
    ui->plainTextEdit->appendPlainText("Disconnected");
    ui->lblConnStatus->setText("DISCONNECTED");
}

void DBusTest::handleObjectAdded()
{
    ui->plainTextEdit->appendPlainText("Object added");
    ui->lblConsStatus->setText("OBJECT ADDED");
//    ui->lblConsStatus->setStyleSheet("border: 2px solid green;");
    ui->lblConsStatus->setStyleSheet("background-color: lightgreen;");
}

void DBusTest::handleObjectRemoved()
{
    ui->plainTextEdit->appendPlainText("Object removed");
    ui->lblConsStatus->setText("OBJECT REMOVED");
    ui->lblConsStatus->setStyleSheet("background-color: lightgray;");
}

void DBusTest::handleServiceRegistered()
{
    ui->plainTextEdit->appendPlainText("Service registered");
}

void DBusTest::on_btnConnect_clicked()
{
    if (ui->btnConnect->text() == "Connect")
    {
        ui->plainTextEdit->appendPlainText("Connecting...");
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
        ui->plainTextEdit->appendPlainText("Disconnecting...");
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
//    deleteWatcher();
//    m_watcher = m_client->createObjectWatcher(ui->txtConsService->text(), ui->txtConsPath->text());
//    connect(m_watcher, &DBusObjectWatcher::objectAdded, this, &DBusTest::handleObjectAdded);
//    connect(m_watcher, &DBusObjectWatcher::objectRemoved, this, &DBusTest::handleObjectRemoved);
//    m_watcher->startWatching();

    DBusServiceMonitor* monitor = m_client->createServiceMonitor(ui->txtConsService->text());
    monitor->startWatching();
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

void DBusTest::on_btnTestCon_clicked()
{
    QDBusConnection::connectToBus(QDBusConnection::SystemBus, "pipi");
}

void DBusTest::on_btnTestDisconn_clicked()
{
    QDBusConnection::disconnectFromBus("pipi");
}

void DBusTest::on_btnShow_clicked()
{
    QStringList msg("Registered services: ");
    QDBusConnection conn = m_client->connection();
    if (conn.isConnected())
    {
        const QDBusReply<QStringList> repl = conn.interface()->registeredServiceNames();
        if (repl.isValid())
        {
            const QStringList list = repl.value();
            qDebug() << list;
            msg << list;
            for (const QString& name : list)
            {
//                ui->plainTextEdit->appendPlainText(name);
            }
        }
        else
        {
            qDebug() << repl.error();
        }
    }
    else
    {
        msg << "Not connected!";
    }
    ui->plainTextEdit->appendPlainText(msg.join(" "));
}
