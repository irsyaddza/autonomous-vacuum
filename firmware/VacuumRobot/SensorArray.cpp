#include "SensorArray.h"
#include "config.h"

void SensorArray::begin() {
    pinMode(PIN_IR_FRONT, INPUT);
    pinMode(PIN_IR_LEFT, INPUT);
    pinMode(PIN_IR_RIGHT, INPUT);
    
    pinMode(PIN_CLIFF_FRONT, INPUT);
    pinMode(PIN_CLIFF_LEFT, INPUT);
    pinMode(PIN_CLIFF_RIGHT, INPUT);
}

bool SensorArray::readObstacles() {
    // IR Sensors usually LOW when obstacle detected (Active Low)
    // Adjust based on your specific sensor module
    _obsFront = (digitalRead(PIN_IR_FRONT) == LOW);
    _obsLeft  = (digitalRead(PIN_IR_LEFT) == LOW);
    _obsRight = (digitalRead(PIN_IR_RIGHT) == LOW);
    
    return (_obsFront || _obsLeft || _obsRight);
}

bool SensorArray::readCliffs() {
    // Cliff sensors: usually HIGH if surface detected (Reflection), LOW if Cliff (No reflection)
    // CAUTION: Verify your sensor logic!
    _cliffFront = (digitalRead(PIN_CLIFF_FRONT) == LOW); 
    _cliffLeft  = (digitalRead(PIN_CLIFF_LEFT) == LOW);
    _cliffRight = (digitalRead(PIN_CLIFF_RIGHT) == LOW);
    
    return (_cliffFront || _cliffLeft || _cliffRight);
}

bool SensorArray::isFrontBlocked() { return _obsFront; }
bool SensorArray::isLeftBlocked()  { return _obsLeft; }
bool SensorArray::isRightBlocked() { return _obsRight; }
bool SensorArray::isCliffDetected() { return (_cliffFront || _cliffLeft || _cliffRight); }
