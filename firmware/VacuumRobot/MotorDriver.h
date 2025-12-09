#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <Arduino.h>

class MotorDriver {
public:
    void begin();
    void stop();
    void moveForward();
    void moveBackward();
    void turnLeft();
    void turnRight();
};

#endif
