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
    void setSpeed(int pwm); // 0-255 (both wheels)
    void setLeftSpeed(int pwm);   // 0-255 (left wheel only)
    void setRightSpeed(int pwm);  // 0-255 (right wheel only)
    int getLeftSpeed() const { return _leftSpeed; }
    int getRightSpeed() const { return _rightSpeed; }

private:
    int _leftSpeed = 0;
    int _rightSpeed = 0;
};

#endif