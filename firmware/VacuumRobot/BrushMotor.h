#ifndef BRUSH_MOTOR_H
#define BRUSH_MOTOR_H

#include <Arduino.h>

class BrushMotor {
public:
    void begin();
    void forward();              // Motor sapu berputar maju (with soft start)
    void reverse();              // Motor sapu berputar mundur (with soft start)
    void stop();                 // Immediate stop (no ramp - safety)
    void setSpeed(int pwm);      // 0-255
    void updateSoftStart();      // Call in loop() for non-blocking ramp
    bool isRamping();            // True if currently ramping

private:
    int _currentSpeed = 0;
    int _targetSpeed = 0;
    bool _isRunning = false;
    bool _isForward = true;      // Direction tracking
    bool _ramping = false;
    unsigned long _lastRampTime = 0;
    
    void _applySpeed();          // Internal: write PWM to hardware
};

#endif
