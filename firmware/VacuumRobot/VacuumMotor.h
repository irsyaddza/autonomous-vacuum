#ifndef VACUUM_MOTOR_H
#define VACUUM_MOTOR_H

#include <Arduino.h>

class VacuumMotor {
public:
    void begin();
    void setPower(int pwm); // 0-255
    void start(); // Uses default NORMAL power
    void stop();

private:
    int _currentPower = 0;
};

#endif
