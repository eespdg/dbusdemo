#include "vehicle.h"

Vehicle::Vehicle(QObject *parent)
    : QObject(parent)
    , m_speed(0)
{
}

int Vehicle::speed()
{
    return m_speed;
}

void Vehicle::accelerate()
{
    ++ m_speed;
    Q_EMIT speedChanged(m_speed);
}

void Vehicle::decelerate()
{
    -- m_speed;
    Q_EMIT speedChanged(m_speed);
}

void Vehicle::setSpeed(int speed)
{
    m_speed = speed;
    Q_EMIT speedChanged(m_speed);
}
