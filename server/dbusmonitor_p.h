#ifndef DBUSMONITOR_P_H
#define DBUSMONITOR_P_H

#include <QElapsedTimer>

class DBusMonitorPrivate
{
public:
    DBusMonitorPrivate();

    QElapsedTimer m_elapsedTimer;
};

#endif // DBUSMONITOR_P_H
