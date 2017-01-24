#include "clientmainwindow.h"
#include "ui_clientmainwindow.h"
#include <QtDBus/QDBusError>
#include <QDebug>
#include <QTimer>
#include <functional>

ClientMainWindow::ClientMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ClientMainWindow)
    , m_connected(false)
{
    ui->setupUi(this);
    ui->statusBar->showMessage("DISCONNECTED");
    QTimer::singleShot(0, this, &ClientMainWindow::monitorConnection);
}

ClientMainWindow::~ClientMainWindow()
{
    delete ui;
}

void ClientMainWindow::monitorConnection()
{
    static const QString connectionName("coreservice");

    // check the current connection
    if (m_connected)
    {
        // retrieve connection by name
        QDBusConnection connection(connectionName);
        if (!connection.isConnected())
        {
            qDebug() << connection.lastError().message();
            m_connected = false;
            QDBusConnection::disconnectFromPeer(connectionName);
            qDebug() << "Disconnected from coreservice";
            m_vehicle.reset();
            ui->statusBar->showMessage("CONNECTION INTERRUPTED");
        }
    }

    // try to connect
    if (!m_connected)
    {
        // try to establish the connection
        QDBusConnection connection = QDBusConnection::connectToPeer("tcp:host=127.0.0.1,port=55555", connectionName);
        if (connection.isConnected())
        {
            qDebug() << "Connected to coreservice";
            m_connected = true;
            ui->statusBar->showMessage("CONNECTED - starting...");

            m_vehicle.reset(new org::example::VehicleInterface("", "/Vehicle", connection, this));
            connect(m_vehicle.data(), &org::example::VehicleInterface::objectRegistered, this, &ClientMainWindow::useRemoteObject);

            connection.connect("", "/Status", "com.test.if", "Ready", this, SLOT(useRemoteObject()));

        }
        else
        {
            qDebug() << connection.lastError().message();
            QDBusConnection::disconnectFromPeer(connectionName);
            qDebug() << "Cannot connect to coreservice";
        }
    }

    QTimer::singleShot(1000, this, &ClientMainWindow::monitorConnection);
}


void ClientMainWindow::useRemoteObject()
{
    qDebug() << "Connection name:" << m_vehicle->connection().name();
    if (m_connected)
    {
        // start using the proxy object
        qDebug() << "Accelerate";
        m_vehicle->accelerate();
    }
    else
    {
        qWarning() << "I did not expect this to happen!";
    }
}

void ClientMainWindow::on_btnAccelerate_clicked()
{
    if (m_vehicle)
    {
        m_vehicle->accelerate();
    }
}

void ClientMainWindow::on_btnDecelerate_clicked()
{
    if (m_vehicle)
    {
        m_vehicle->decelerate();
    }
}
