#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <Arduino.h>

class MotorDriver {
public:
    void begin();
    void setSpeed(int speed); // 0-255
    void stop();
    void moveForward();
    void moveBackward();
    void turnLeft();
    void turnRight();

private:
    int _currentSpeed = 0;
};

#endif
