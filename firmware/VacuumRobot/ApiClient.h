#ifndef API_CLIENT_H
#define API_CLIENT_H

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WebServer.h>

class ApiClient {
public:
    void connectWiFi();
    void update(); // Main loop (handles web server + battery send)
    
    // Direct HTTP Server
    void startWebServer();   // Initialize WebServer for receiving browser commands
    void handleWebServer();  // Process incoming requests
    
    // Device Registration
    void registerDevice();   // Send IP + MAC to Laravel on boot
    
    // API interactions
    String getStatus();      // Poll status from Laravel (fallback)
    void sendBattery(int percent, float voltage);
    void logCommandToServer(String command, String status, int responseMs);
    
    // Utility
    void checkResetButton();
    void initBuzzer();
    void playWiFiResetBeep();
    
    // Local copy of server data
    String lastState = "standby";
    String lastPowerMode = "normal";
    int lastPowerValue = 200;
    String lastDirection = "forward";

private:
    unsigned long _lastPollTime = 0;
    unsigned long _lastBatterySendTime = 0;
    
    void _parseStatusResponse(String json);
    void _handleCommand();       // Handler for POST /command
    void _handleStatus();        // Handler for GET /status
    void _handleCorsOptions();   // Handler for OPTIONS (CORS preflight)
    void _addCorsHeaders();      // Add CORS headers to response
};

#endif
