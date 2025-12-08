#ifndef SENSOR_ARRAY_H
#define SENSOR_ARRAY_H

#include <Arduino.h>

class SensorArray {
public:
    void begin();
    
    // Returns true if obstacle detected
    bool readObstacles();
    
    // Returns true if cliff detected
    bool readCliffs();
    
    bool isFrontBlocked();
    bool isLeftBlocked();
    bool isRightBlocked();
    
    bool isCliffDetected();

private:
    // Raw states
    bool _obsFront;
    bool _obsLeft;
    bool _obsRight;
    
    bool _cliffFront;
    bool _cliffLeft;
    bool _cliffRight;
};

#endif
