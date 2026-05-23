#ifndef SENSOR_ARRAY_H
#define SENSOR_ARRAY_H

#include <Arduino.h>

class SensorArray {
public:
    SensorArray();

    void begin();

    // Read Sensors
    bool readObstacles();
    bool readCliffs();

    // Obstacle
    bool isFrontBlocked();
    bool isLeftBlocked();
    bool isRightBlocked();

    // Cliff
    bool isCliffFront();
    bool isCliffLeft();
    bool isCliffRight();
    bool isCliffDetected();

    // BLE RSSI Homing
    void updateRSSI(int rssi);
    int getRSSI();

private:

    // Obstacle states
    bool _obsFront = false;
    bool _obsLeft  = false;
    bool _obsRight = false;

    // Cliff states
    bool _cliffFront = false;
    bool _cliffLeft  = false;
    bool _cliffRight = false;

    // BLE RSSI
    int _rssi = -100;
};

#endif