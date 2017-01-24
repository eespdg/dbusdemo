#ifndef VEHICLE_H
#define VEHICLE_H

#include <QObject>

/**
 * @brief Test object exposed over D-Bus
 */
class Vehicle : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.example.VehicleInterface")
public:
    Vehicle(QObject* parent = Q_NULLPTR);

public:
    int speed();

public Q_SLOTS:
    void accelerate();
    void decelerate();
    void setSpeed(int speed);

Q_SIGNALS:
    Q_SCRIPTABLE void speedChanged(int speed);
    Q_SCRIPTABLE void objectRegistered(QString connectionName);

private:
    int m_speed;
};

#endif // VEHICLE_H
