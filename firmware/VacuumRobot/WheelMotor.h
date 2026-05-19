#ifndef WHEEL_MOTOR_H
#define WHEEL_MOTOR_H

#include <Arduino.h>

class WheelMotor {
public:
    void begin();

    void setSpeed(int pwm);

    void moveForward();
    void moveBackward();

    void turnLeft();
    void turnRight();

    void stop();

private:
    int _speed = 170;
};

#endif