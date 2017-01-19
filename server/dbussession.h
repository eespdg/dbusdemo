#ifndef DBUSSESSION_H
#define DBUSSESSION_H

#include "car.h"
#include <QObject>
#include <QtDBus>

class DBusSession : public QObject
{
    Q_OBJECT
public:
    explicit DBusSession(const QDBusConnection &connection, Car *car, QObject *parent = 0);

    void dump();

signals:

public slots:

private:
    QDBusConnection m_connection;
    Car* m_car;
};

#endif // DBUSSESSION_H
