#include "clientmainwindow.h"
#include "ui_clientmainwindow.h"
#include <QtDBus/QDBusError>
#include <QDebug>
#include <QTimer>
#include <functional>

ClientMainWindow::ClientMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ClientMainWindow)
    , m_connectionCounter(0)
    , m_lastSetSpeed(0)
{
    ui->setupUi(this);

    // this shows if the main thread hangs
    QTimer::singleShot(0, this, &ClientMainWindow::showProgress);

    handleDisconnection();

    // 1. connect the signals
    connect(&m_connection, &DBusClientConnection::connectedToServer,
            this, &ClientMainWindow::handleConnection);
    connect(&m_connection, &DBusClientConnection::disconnectedFromServer,
            this, &ClientMainWindow::handleDisconnection);
    connect(&m_connection, &DBusClientConnection::heartBeat,
            this, &ClientMainWindow::showTicks);

    // 2. initiate connection to server
    m_connection.connectToServer("tcp:host=127.0.0.1,port=55555", "core");
}

ClientMainWindow::~ClientMainWindow()
{
    delete ui;
}

void ClientMainWindow::showProgress()
{
    int value = ui->progressBar->value();
    if (++value > ui->progressBar->maximum())
    {
        value = 0;
    }
    ui->progressBar->setValue(value);

    QTimer::singleShot(100, this, &ClientMainWindow::showProgress);
}

void ClientMainWindow::handleConnection(QDBusConnection connection)
{
    qDebug() << "Connection name:" << connection.name();
    ui->plainTextEdit->appendPlainText("REGISTERED");
    ui->statusBar->showMessage("REGISTERED");
    ui->plainTextEdit->setStyleSheet("background-color: rgb(255, 255, 255);");

    // create proxy object
    m_vehicle.reset(new org::example::VehicleInterface("", "/Vehicle", connection, this));
    connect(m_vehicle.data(), &org::example::VehicleInterface::speedChanged, this->ui->spinBox, &QSpinBox::setValue);

    // it must be possible to call proxy object immediately
    on_btnAccelerate_clicked();
}

void ClientMainWindow::handleDisconnection()
{
    m_vehicle.reset();
    ui->plainTextEdit->appendPlainText("DISCONNECTED");
    ui->statusBar->showMessage("DISCONNECTED");
    ui->plainTextEdit->setStyleSheet("background-color: rgb(255, 155, 155);");
}

void ClientMainWindow::setVehicleSpeed(int value)
{
    if (m_vehicle)
    {
        if (m_lastSetSpeed != value)
        {
            ui->plainTextEdit->appendPlainText(QString("Set speed: %1").arg(value));
            QDBusPendingReply<> reply = m_vehicle->setSpeed(value);
            reply.waitForFinished();
            if (reply.isError())
            {
                ui->plainTextEdit->appendPlainText("Set speed: CONNECTION ERROR: " + reply.error().message());
            }
            else
            {
                m_lastSetSpeed = value;
            }
        }
    }
}

void ClientMainWindow::showTicks(qint64 ticks)
{
    ui->lblTicks->setText(QString::number(ticks));
}

void ClientMainWindow::on_btnAccelerate_clicked()
{
    if (m_vehicle)
    {
        QDBusPendingReply<> reply = m_vehicle->accelerate();
        reply.waitForFinished();
        if (reply.isError())
        {
            ui->plainTextEdit->appendPlainText("Accelerate: CONNECTION ERROR: " + reply.error().message());
        }
        else
        {
             ui->plainTextEdit->appendPlainText("Accelerate OK");
        }
    }
}

void ClientMainWindow::on_btnDecelerate_clicked()
{
    if (m_vehicle)
    {
        QDBusPendingReply<> reply = m_vehicle->decelerate();
        reply.waitForFinished();
        if (reply.isError())
        {
            ui->plainTextEdit->appendPlainText("Decelerate: CONNECTION ERROR: " + reply.error().message());
        }
        else
        {
             ui->plainTextEdit->appendPlainText("Decelerate OK");
        }
    }
}

void ClientMainWindow::on_btnReadSpeed_clicked()
{
    if (m_vehicle)
    {
        QDBusPendingReply<int> currentSpeed = m_vehicle->speed();
        currentSpeed.waitForFinished();
        if (currentSpeed.isError())
        {
            ui->plainTextEdit->appendPlainText("Vehicle speed read error: " + currentSpeed.error().message());
            ui->lblSpeed->setText("ERROR");
        }
        else
        {
            ui->lblSpeed->setText(QString::number(currentSpeed.value()));
            ui->plainTextEdit->appendPlainText(QString("Read speed: %1").arg(currentSpeed.value()));
        }
    }
}

void ClientMainWindow::on_btnSetSpeed_clicked()
{
    setVehicleSpeed(ui->spinBox->value());
}
