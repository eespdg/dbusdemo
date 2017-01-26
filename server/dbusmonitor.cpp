#include "dbusmonitor.h"
#include "dbusmonitor_adaptor.h"
#include <QTimer>

DBusMonitor::DBusMonitor(QObject *parent)
    : QObject(parent)
    , m_elapsedTimer()
{
    // create D-Bus adaptor for this object
    new DBusMonitorInterfaceAdaptor(this);

    // periodically emit milliseconds from start
    m_elapsedTimer.start();
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [=](){
        Q_EMIT heartBeat(m_elapsedTimer.elapsed());
    });
    timer->start(1000);
}

void DBusMonitor::registerForConnection(QDBusConnection connection)
{
    connection.registerObject("/DBusMonitor", "com.barco.healthcare.DBusMonitorInterface", this);
}
