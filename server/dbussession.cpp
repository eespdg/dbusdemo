#include "dbussession.h"
#include "car_adaptor.h"
#include <QDebug>

DBusSession::DBusSession(const QDBusConnection &connection, Car* car, QObject *parent)
    : QObject(parent)
    , m_connection(connection)
    , m_car(car)
{
    qDebug() << "DBus session created" << m_connection.baseService();


    new CarInterfaceAdaptor(car);
    if (!m_connection.registerObject("/Car", car))
    {
        qDebug() << "Error registering object:" << m_connection.lastError().message();
    }

//    if (!m_connection.registerService(""))
//    {
//        // not supported for peer-to-peer connections!
//        qDebug() << "Error registering service:" << m_connection.lastError().message();
//    }

}

void DBusSession::dump()
{
//    qDebug() << "Name" << m_connection.name();
}
