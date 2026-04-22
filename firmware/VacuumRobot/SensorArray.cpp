#include "SensorArray.h"
#include "config.h"

void SensorArray::begin() {
    // Obstacle Avoidance Sensors (IR1, IR2, IR3)
    pinMode(PIN_IR_LEFT, INPUT);
    pinMode(PIN_IR_FRONT, INPUT);
    pinMode(PIN_IR_RIGHT, INPUT);
    
    // Cliff Detection Sensors (IR4, IR5, IR6)
    pinMode(PIN_CLIFF_LEFT, INPUT);
    pinMode(PIN_CLIFF_FRONT, INPUT);
    pinMode(PIN_CLIFF_RIGHT, INPUT);
    
    Serial.println("[SENSOR] All 6 IR sensors initialized (3 obstacle + 3 cliff)");
}

// Debounce helper: requires SENSOR_DEBOUNCE_COUNT consecutive
// identical readings before changing state (symmetric for ON and OFF)
bool SensorArray::_debounce(bool rawReading, int &counter, bool &state) {
    if (rawReading) {
        if (counter < SENSOR_DEBOUNCE_COUNT) counter++;
        if (counter >= SENSOR_DEBOUNCE_COUNT) state = true;
    } else {
        if (counter > 0) counter--;
        if (counter <= 0) state = false;
    }
    return state;
}

bool SensorArray::readObstacles() {
    // IR Sensors: LOW = obstacle detected (Active Low)
    bool rawLeft  = (digitalRead(PIN_IR_LEFT) == LOW);
    bool rawFront = (digitalRead(PIN_IR_FRONT) == LOW);
    bool rawRight = (digitalRead(PIN_IR_RIGHT) == LOW);
    
    // Apply debounce
    _debounce(rawLeft,  _obsLeftCount,  _obsLeft);
    _debounce(rawFront, _obsFrontCount, _obsFront);
    _debounce(rawRight, _obsRightCount, _obsRight);
    
    return (_obsFront || _obsLeft || _obsRight);
}

bool SensorArray::readCliffs() {
    // Cliff sensors: LOW = cliff detected (no reflection from surface)
    // HIGH = surface present (IR reflected back)
    bool rawLeft  = (digitalRead(PIN_CLIFF_LEFT) == LOW);
    bool rawFront = (digitalRead(PIN_CLIFF_FRONT) == LOW);
    bool rawRight = (digitalRead(PIN_CLIFF_RIGHT) == LOW);
    
    // Apply debounce
    _debounce(rawLeft,  _cliffLeftCount,  _cliffLeft);
    _debounce(rawFront, _cliffFrontCount, _cliffFront);
    _debounce(rawRight, _cliffRightCount, _cliffRight);
    
    return (_cliffFront || _cliffLeft || _cliffRight);
}

// Individual obstacle getters
bool SensorArray::isFrontBlocked()  { return _obsFront; }
bool SensorArray::isLeftBlocked()   { return _obsLeft; }
bool SensorArray::isRightBlocked()  { return _obsRight; }
bool SensorArray::isAnyObstacle()   { return (_obsFront || _obsLeft || _obsRight); }

// Individual cliff getters
bool SensorArray::isCliffFront()    { return _cliffFront; }
bool SensorArray::isCliffLeft()     { return _cliffLeft; }
bool SensorArray::isCliffRight()    { return _cliffRight; }
bool SensorArray::isCliffDetected() { return (_cliffFront || _cliffLeft || _cliffRight); }
