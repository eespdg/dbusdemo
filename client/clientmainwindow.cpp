#include "clientmainwindow.h"
#include "ui_clientmainwindow.h"
#include <QtDBus/QDBusError>
#include <QDebug>
#include <QTimer>
#include <functional>

ClientMainWindow::ClientMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ClientMainWindow)
    , m_connection("Domagoj")
{
    ui->setupUi(this);
    QTimer::singleShot(0, this, &ClientMainWindow::tryConnect);
}

ClientMainWindow::~ClientMainWindow()
{
    delete ui;
}

void ClientMainWindow::handleServiceRegistration(const QString &serviceName)
{
    qDebug() << "serviceRegistered:" << serviceName;
}

void ClientMainWindow::tryConnect()
{
    m_connection = QDBusConnection::connectToPeer("tcp:host=127.0.0.1,port=55555", "Domagoj");
    if (!m_connection.isConnected())
    {
        QDBusError lastError = m_connection.lastError();
        qDebug() << lastError.message();

        QDBusConnection::disconnectFromBus(m_connection.name());
        qDebug() << "Waiting for dbus to be available...";
        QTimer::singleShot(1000, this, &ClientMainWindow::tryConnect);
    }
    else
    {
        qDebug() << "Connected!";
        m_watcher.reset(new QDBusServiceWatcher("", m_connection, QDBusServiceWatcher::WatchForOwnerChange, Q_NULLPTR));
        connect(m_watcher.data(), &QDBusServiceWatcher::serviceRegistered, this, &ClientMainWindow::handleServiceRegistration);
    }
}
