#ifndef API_CLIENT_H
#define API_CLIENT_H

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

class ApiClient {
public:
    void connectWiFi();
    void update(); // Main polling loop
    
    // API interactions
    String getStatus();
    void sendBattery(int percent, float voltage);
    void checkResetButton();
    
    // Local copy of server data
    String lastState = "standby";
    String lastPowerMode = "normal";
    int lastPowerValue = 200;

private:
    unsigned long _lastPollTime = 0;
    unsigned long _lastBatterySendTime = 0;
    
    void _parseStatusResponse(String json);
};

#endif
