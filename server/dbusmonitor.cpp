#include "dbusmonitor.h"
#include "dbusmonitor_p.h"
#include "dbusmonitor_adaptor.h"
#include <QTimer>

DBusMonitorPrivate::DBusMonitorPrivate()
    : m_elapsedTimer()
{
}

DBusMonitor::DBusMonitor(QObject *parent)
    : QObject(parent)
    , d_ptr(new DBusMonitorPrivate())
{
    // create D-Bus adaptor for this object
    new DBusMonitorInterfaceAdaptor(this);

    // periodically emit milliseconds from start
    Q_D(DBusMonitor);
    d->m_elapsedTimer.start();
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [=](){
        Q_EMIT heartBeat(d->m_elapsedTimer.elapsed());
    });
    timer->start(1000);
}

DBusMonitor::~DBusMonitor()
{
}

void DBusMonitor::registerForConnection(QDBusConnection connection)
{
    connection.registerObject("/DBusMonitor", dBusInterfaceName(), this);
}

QString DBusMonitor::dBusInterfaceName()
{
    const int index = staticMetaObject.indexOfClassInfo("D-Bus Interface");
    Q_ASSERT(index >= 0);
    if (index < 0)
    {
        qCritical() << "Missing Q_CLASSINFO('D-Bus Interface', ...";
        return QString();
    }
    return staticMetaObject.classInfo(index).value();
}
