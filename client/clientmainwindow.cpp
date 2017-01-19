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
    , m_car(Q_NULLPTR)
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
    QDBusConnection con = QDBusConnection::connectToPeer("tcp:host=127.0.0.1,port=55555", "Domagoj");
    if (!con.isConnected())
    {
        QDBusError lastError = con.lastError();
        qDebug() << lastError.message();

        qDebug() << "connection name:" << con.name();
        QDBusConnection::disconnectFromPeer(con.name());
        qDebug() << "Waiting for dbus to be available...";
        QTimer::singleShot(1000, this, &ClientMainWindow::tryConnect);
    }
    else
    {
        m_connection = con;
        qDebug() << "Connected!";

        m_car = new org::example::Examples::CarInterface("", "/Car",
                               m_connection, this);
        m_watcher.reset(new QDBusServiceWatcher("", m_connection, QDBusServiceWatcher::WatchForOwnerChange, Q_NULLPTR));
        connect(m_watcher.data(), &QDBusServiceWatcher::serviceRegistered, this, &ClientMainWindow::handleServiceRegistration);
    }
}

void ClientMainWindow::on_btnAccelerate_clicked()
{
    if (m_car)
    {
        auto reply = m_car->accelerate();
    }
}

void ClientMainWindow::on_btnDecelerate_clicked()
{
    if (m_car)
    {
        auto reply = m_car->decelerate();
    }
}
