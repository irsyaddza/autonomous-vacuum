#ifndef SENSOR_ARRAY_H
#define SENSOR_ARRAY_H

#include <Arduino.h>
#include "config.h"

class SensorArray {
public:
    void begin();
    
    // Read all sensors and update internal state
    // Returns true if any obstacle detected
    bool readObstacles();
    
    // Returns true if any cliff detected
    bool readCliffs();
    
    // Obstacle individual getters
    bool isFrontBlocked();
    bool isLeftBlocked();
    bool isRightBlocked();
    bool isAnyObstacle();
    
    // Cliff individual getters
    bool isCliffFront();
    bool isCliffLeft();
    bool isCliffRight();
    bool isCliffDetected();

private:
    // Obstacle states (debounced)
    bool _obsFront  = false;
    bool _obsLeft   = false;
    bool _obsRight  = false;
    
    // Cliff states (debounced)
    bool _cliffFront = false;
    bool _cliffLeft  = false;
    bool _cliffRight = false;
    
    // Debounce counters
    int _obsFrontCount  = 0;
    int _obsLeftCount   = 0;
    int _obsRightCount  = 0;
    int _cliffFrontCount = 0;
    int _cliffLeftCount  = 0;
    int _cliffRightCount = 0;
    
    // Helper: update debounced state
    bool _debounce(bool rawReading, int &counter, bool &state);
};

#endif
