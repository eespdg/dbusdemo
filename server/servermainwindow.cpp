#include "servermainwindow.h"
#include "ui_servermainwindow.h"
#include "dbussession.h"
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusError>
#include <QDebug>

ServerMainWindow::ServerMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ServerMainWindow)
{
    ui->setupUi(this);

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
    new DBusSession(connection);
}
