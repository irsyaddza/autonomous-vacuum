#ifndef VACUUM_MOTOR_H
#define VACUUM_MOTOR_H

#include <Arduino.h>

class VacuumMotor {
public:
    void begin();
    void setPower(int pwm);       // Set target power with soft start ramp
    void setPowerDirect(int pwm); // Set power immediately (bypass soft start)
    void start();                 // Uses default NORMAL power (with soft start)
    void stop();                  // Immediate stop (no ramp down - safety)
    void updateSoftStart();       // Call in loop() for non-blocking ramp
    bool isRamping();             // True if currently ramping up/down

private:
    int _currentPower = 0;
    int _targetPower = 0;
    unsigned long _lastRampTime = 0;
    bool _ramping = false;
    
    void _applyPower(int pwm);   // Internal: write PWM to hardware
};

#endif
