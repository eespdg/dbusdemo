#include "servermainwindow.h"
#include "ui_servermainwindow.h"
#include "dbussession.h"
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusError>
#include <QDebug>

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

    m_dBusServer = new QDBusServer(QLatin1String("tcp:host=127.0.0.1,port=55555"), this);
    m_dBusServer->setAnonymousAuthenticationAllowed(true);
    connect(m_dBusServer, &QDBusServer::newConnection, this, &ServerMainWindow::handleClientConnection);

    QDBusError lastError = m_dBusServer->lastError();
    qDebug() << lastError.message();
}

ServerMainWindow::~ServerMainWindow()
{
    delete ui;
}

void ServerMainWindow::handleClientConnection(QDBusConnection connection)
{
    qDebug() << "Server: Client connected. Name:" << connection.name() << "Base service:" << connection.baseService();
    m_lastSession = new DBusSession(connection, m_car, this);
}

void ServerMainWindow::on_dump_clicked()
{
    m_lastSession->dump();
}
