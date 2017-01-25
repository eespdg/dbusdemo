#include "servermainwindow.h"
#include "ui_servermainwindow.h"
#include "vehicle_adaptor.h"
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusError>
#include <QDebug>
#include <QMessageBox>

ServerMainWindow::ServerMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ServerMainWindow)
    , m_vehicle(new Vehicle(this))
{
    ui->setupUi(this);

    connect(m_vehicle, &Vehicle::speedChanged, ui->spinBox, &QSpinBox::setValue);
    connect(ui->spinBox, SIGNAL(valueChanged(int)), m_vehicle, SLOT(setSpeed(int)));

    // this shows if the main thread hangs
    QTimer::singleShot(0, this, &ServerMainWindow::showProgress);

    // connect our object to D-Bus messaging
    new VehicleInterfaceAdaptor(m_vehicle);

    // create server for peer-to-peer connection
    m_dBusServer = new QDBusServer(QLatin1String("tcp:host=127.0.0.1,port=55555"), this);
    if (!m_dBusServer->isConnected())
    {
        // in windows
        qDebug() << m_dBusServer->lastError().message();
        QMessageBox::critical(this, "D-Bus Server application", m_dBusServer->lastError().message());
        exit(1);
    }

    // skip authentication
    m_dBusServer->setAnonymousAuthenticationAllowed(true);

    // client connection handler
    connect(m_dBusServer, &QDBusServer::newConnection, this, &ServerMainWindow::handleClientConnection);
}

ServerMainWindow::~ServerMainWindow()
{
    delete ui;
}


void ServerMainWindow::handleClientConnection(QDBusConnection connection)
{
    qDebug() << "Server: Client connected. Name:" << connection.name() << "Base service:" << connection.baseService();
    ui->plainTextEdit->appendPlainText(QString("%1 CONNECTED").arg(connection.name()));

//    QTimer::singleShot(0, [=](){
//        registerObjects(connection);
//    });

    registerObjects(connection);
}

void ServerMainWindow::registerObjects(QDBusConnection connection)
{
    qDebug() << "Registering Vehicle" << connection.name();
    if (!connection.isConnected())
    {
        qDebug() << "Not connected:" << connection.name();
        ui->plainTextEdit->appendPlainText(QString("%1 NOT CONNECTED").arg(connection.name()));
    }
    else if (!connection.registerObject("/Vehicle", m_vehicle))
    {
        qDebug() << "Error registering object:" << connection.lastError().message();
        ui->plainTextEdit->appendPlainText(QString("%1 ERROR REGISTERING").arg(connection.name()));
    }
    else
    {
        ui->plainTextEdit->appendPlainText(QString("%1 REGISTERED").arg(connection.name()));

        // notify the client (this client only) that objects have been registered
        if (!connection.send(QDBusMessage::createSignal("/Status", "com.test.if", "Ready")))
        {
            ui->plainTextEdit->appendPlainText(QString("%1 Error sending signal: %2")
                                               .arg(connection.name())
                                               .arg(connection.lastError().message()));
        }
    }
}

void ServerMainWindow::showProgress()
{
    int value = ui->progressBar->value();
    if (++value > ui->progressBar->maximum())
    {
        value = 0;
    }
    ui->progressBar->setValue(value);

    QTimer::singleShot(100, this, &ServerMainWindow::showProgress);
}

void ServerMainWindow::on_btnAccelerate_clicked()
{
    m_vehicle->accelerate();
}

void ServerMainWindow::on_btnDecelerate_clicked()
{
    m_vehicle->decelerate();
}
