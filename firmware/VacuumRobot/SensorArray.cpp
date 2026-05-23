#include "SensorArray.h"
#include "config.h"

SensorArray::SensorArray() {}

void SensorArray::begin() {

    // Obstacle IR
    pinMode(PIN_OB_LEFT, INPUT);
    pinMode(PIN_OB_FRONT, INPUT);
    pinMode(PIN_OB_RIGHT, INPUT);

    // Cliff IR
    pinMode(PIN_CLIFF_LEFT, INPUT);
    pinMode(PIN_CLIFF_FRONT, INPUT);
    pinMode(PIN_CLIFF_RIGHT, INPUT);

    Serial.println("[SENSOR] SensorArray initialized");
}

// =====================================================
// OBSTACLE SENSOR
// =====================================================

bool SensorArray::readObstacles() {

    // Active LOW
    _obsLeft  = (digitalRead(PIN_OB_LEFT)  == LOW);
    _obsFront = (digitalRead(PIN_OB_FRONT) == LOW);
    _obsRight = (digitalRead(PIN_OB_RIGHT) == LOW);

    return (_obsLeft || _obsFront || _obsRight);
}

// =====================================================
// CLIFF SENSOR
// =====================================================

bool SensorArray::readCliffs() {

    // Active LOW
    _cliffLeft  = (digitalRead(PIN_CLIFF_LEFT)  == LOW);
    _cliffFront = (digitalRead(PIN_CLIFF_FRONT) == LOW);
    _cliffRight = (digitalRead(PIN_CLIFF_RIGHT) == LOW);

    return (_cliffLeft || _cliffFront || _cliffRight);
}

// =====================================================
// OBSTACLE GETTER
// =====================================================

bool SensorArray::isFrontBlocked() {
    return _obsFront;
}

bool SensorArray::isLeftBlocked() {
    return _obsLeft;
}

bool SensorArray::isRightBlocked() {
    return _obsRight;
}

// =====================================================
// CLIFF GETTER
// =====================================================

bool SensorArray::isCliffFront() {
    return _cliffFront;
}

bool SensorArray::isCliffLeft() {
    return _cliffLeft;
}

bool SensorArray::isCliffRight() {
    return _cliffRight;
}

bool SensorArray::isCliffDetected() {
    return (_cliffLeft || _cliffFront || _cliffRight);
}

// =====================================================
// RSSI HOMING
// =====================================================

void SensorArray::updateRSSI(int rssi) {
    _rssi = rssi;
}

int SensorArray::getRSSI() {
    return _rssi;
}