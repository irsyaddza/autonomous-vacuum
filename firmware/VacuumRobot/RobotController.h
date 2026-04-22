#ifndef ROBOT_CONTROLLER_H
#define ROBOT_CONTROLLER_H

#include <Arduino.h>
#include "CleaningAlgorithm.h"

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
    
    // For change detection and logging
    String _prevState = "";
    int _prevPowerValue = 0;
    
    // Autonomous cleaning algorithm
    CleaningAlgorithm _cleaner;
    bool _wasAutonomous = false;
};

#endif
