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
    , m_connectionCounter(0)
    , m_lastSetSpeed(0)
{
    ui->setupUi(this);

//    connect(ui->spinBox, SIGNAL(valueChanged(int)), this, SLOT(setVehicleSpeed(int)));

    // this shows if the main thread hangs
    QTimer::singleShot(0, this, &ClientMainWindow::showProgress);

    ui->statusBar->showMessage("DISCONNECTED");
    QTimer::singleShot(0, this, &ClientMainWindow::monitorConnection);
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
            ui->plainTextEdit->appendPlainText("DISCONNECTED");
            ui->statusBar->showMessage("DISCONNECTED");
            m_vehicle.reset();
        }
    }

    // try to connect
    if (!m_connected)
    {
        // try to establish the connection
        const QString connectionString("tcp:host=127.0.0.1,port=55555");
        ui->plainTextEdit->appendPlainText(QString("[%1] CONNECTING TO: %2 ...").arg(++m_connectionCounter).arg(connectionString));
        ui->plainTextEdit->repaint();
        ui->statusBar->showMessage("CONNECTING");
        ui->statusBar->repaint();
        QDBusConnection connection = QDBusConnection::connectToPeer(connectionString, connectionName);
        if (connection.isConnected())
        {
            qDebug() << "Connected to coreservice";
            ui->plainTextEdit->appendPlainText("CONNECTED");
            ui->statusBar->showMessage("CONNECTED");
            m_connected = true;

            // create proxy
            m_vehicle.reset(new org::example::VehicleInterface("", "/Vehicle", connection, this));

//            connect(m_vehicle.data(), &org::example::VehicleInterface::objectRegistered, this, &ClientMainWindow::useRemoteObject);
            connection.connect("", "/Status", "com.test.if", "Ready", this, SLOT(useRemoteObject()));

            connect(m_vehicle.data(), &org::example::VehicleInterface::speedChanged, this->ui->spinBox, &QSpinBox::setValue);


        }
        else
        {
            qDebug() << connection.lastError().message();
            ui->plainTextEdit->appendPlainText("CONNECTION ERROR: " + connection.lastError().message());
            ui->statusBar->showMessage("CONNECTION ERROR");
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
        ui->plainTextEdit->appendPlainText("REGISTERED");
        ui->statusBar->showMessage("REGISTERED");

        // try to call proxy object
        on_btnAccelerate_clicked();
    }
    else
    {
        qWarning() << "I did not expect this to happen!";
        ui->plainTextEdit->appendPlainText("'Ready' signal received but not connected!");
    }
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
