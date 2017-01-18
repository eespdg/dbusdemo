#include "dbussession.h"
#include <QDebug>

DBusSession::DBusSession(const QDBusConnection &connection, QObject *parent)
    : QObject(parent)
    , m_connection(connection)
{
    qDebug() << "DBus session created" << m_connection.baseService();
    if (!m_connection.registerService("a.b.c"))
    {
        qDebug() << "Error registering service:" << m_connection.lastError().message();
    }
}
