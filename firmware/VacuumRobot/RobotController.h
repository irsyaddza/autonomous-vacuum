#ifndef ROBOT_CONTROLLER_H
#define ROBOT_CONTROLLER_H

#include <Arduino.h>

class RobotController {
public:
    void begin();
    void update(); // Main logic loop

private:
    void stopAll();
    void handleCleaning();
    void handleReturning();
    void checkSafety();
    
    unsigned long _lastBatteryCheck = 0;
};

#endif
