#include "servermainwindow.h"
#include "ui_servermainwindow.h"
#include "dbussession.h"
#include "car_adaptor.h"
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusError>
#include <QDebug>
#include <QMessageBox>

ServerMainWindow::ServerMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ServerMainWindow),
    m_scene(new QGraphicsScene(-500, -500, 1000, 1000, this)),
    m_car(new Car()),
    m_lastSession(Q_NULLPTR)
{
    ui->setupUi(this);

    m_scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    m_scene->addItem(m_car);

    ui->graphicsView->setScene(m_scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setBackgroundBrush(Qt::darkGray);

    QTimer::singleShot(0, this, &ServerMainWindow::showProgress);

    // start the server from event loop
//    QTimer::singleShot(1000, this, &ServerMainWindow::startDBusServer);
    QMetaObject::invokeMethod(this, "startDBusServer", Qt::QueuedConnection);
}

ServerMainWindow::~ServerMainWindow()
{
    delete ui;
}

void ServerMainWindow::startDBusServer()
{
    new CarInterfaceAdaptor(m_car);
    m_dBusServer = new QDBusServer(QLatin1String("tcp:host=127.0.0.1,port=55555"), this);
    if (!m_dBusServer->isConnected())
    {
        // in windows
        qDebug() << m_dBusServer->lastError().message();
        QMessageBox::critical(this, "D-Bus Server application", "D-Bus Server cannot be started!");
        exit(1);
    }
    m_dBusServer->setAnonymousAuthenticationAllowed(true);
    connect(m_dBusServer, &QDBusServer::newConnection, this, &ServerMainWindow::handleClientConnection);
}

void ServerMainWindow::handleClientConnection(QDBusConnection connection)
{
    qDebug() << "Server: Client connected. Name:" << connection.name() << "Base service:" << connection.baseService();
//    m_lastSession = new DBusSession(connection, m_car, this);

//    new CarInterfaceAdaptor(m_car);

    // intentional delay before object registration to check
    // if clients can synchronize well
//    QThread::msleep(1000);

    if (!connection.registerObject("/Car", m_car))
    {
        qDebug() << "Error registering object:" << connection.lastError().message();
    }

    QTimer::singleShot(0, this, &ServerMainWindow::startClientCommunication);
//    QMetaObject::invokeMethod(this, "startClientCommunication", Qt::QueuedConnection);
}

void ServerMainWindow::startClientCommunication()
{
    // signal to the client that all the objects are registered now
    // so it can start caling the methods
    Q_EMIT m_car->crashed();
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

void ServerMainWindow::on_dump_clicked()
{
    m_lastSession->dump();
}
