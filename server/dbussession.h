#ifndef DBUSSESSION_H
#define DBUSSESSION_H

#include <QObject>
#include <QtDBus>

class DBusSession : public QObject
{
    Q_OBJECT
public:
    explicit DBusSession(const QDBusConnection &connection, QObject *parent = 0);

signals:

public slots:

private:
    QDBusConnection m_connection;
};

#endif // DBUSSESSION_H
