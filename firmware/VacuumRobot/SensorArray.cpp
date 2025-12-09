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
}

bool SensorArray::readObstacles() {
    // IR Sensors: LOW = obstacle detected (Active Low)
    // Sesuaikan jika sensor kamu berbeda
    _obsLeft  = (digitalRead(PIN_IR_LEFT) == LOW);
    _obsFront = (digitalRead(PIN_IR_FRONT) == LOW);
    _obsRight = (digitalRead(PIN_IR_RIGHT) == LOW);
    
    return (_obsFront || _obsLeft || _obsRight);
}

bool SensorArray::readCliffs() {
    // Cliff sensors: LOW = cliff detected (no reflection)
    // HIGH = surface detected (reflection)
    _cliffLeft  = (digitalRead(PIN_CLIFF_LEFT) == LOW);
    _cliffFront = (digitalRead(PIN_CLIFF_FRONT) == LOW);
    _cliffRight = (digitalRead(PIN_CLIFF_RIGHT) == LOW);
    
    return (_cliffFront || _cliffLeft || _cliffRight);
}

bool SensorArray::isFrontBlocked() { return _obsFront; }
bool SensorArray::isLeftBlocked()  { return _obsLeft; }
bool SensorArray::isRightBlocked() { return _obsRight; }
bool SensorArray::isCliffDetected() { return (_cliffFront || _cliffLeft || _cliffRight); }
