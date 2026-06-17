#ifndef ROBOT_CONTROLLER_H
#define ROBOT_CONTROLLER_H

#include <Arduino.h>

class RobotController {
public:

    void begin();
    void update();

private:

    // Main Mode
    void handleCleaning();

    // Utility
    void stopAll();

    // Anti-loop
    unsigned long _loopStartTime = 0;
    bool _escapeMode    = false;
    bool _escapeToggle  = false; // alternating: false=kiri, true=kanan

    // Battery
    unsigned long _lastBatteryCheck = 0;

    // Debug
    String _prevState    = "";
    int _prevPowerValue  = -1;
};

#endif