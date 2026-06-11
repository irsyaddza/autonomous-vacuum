#ifndef API_CLIENT_H
#define API_CLIENT_H

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <Preferences.h>

class ApiClient {
public:
    void connectWiFi();
    void update(); // Main loop (handles web server + reset button)
    
    // Direct HTTP Server
    void startWebServer();   // Initialize WebServer for receiving browser commands
    void handleWebServer();  // Process incoming requests
    
    // Device Registration
    void registerDevice();   // Send IP + MAC to Laravel on boot
    
    // API interactions (outgoing to Laravel)
    void sendBattery(int percent, float voltage);
    void sendBatteryEvent(String event, int percent, float voltage);
    void sendAutoStop(int percent, float voltage);  // Notify server robot auto-stopped
    void logCommandToServer(String command, String status, int responseMs);
    
    // Utility
    void checkResetButton();
    void initBuzzer();
    void playWiFiResetBeep();
    
    // Local copy of server data
    String lastState = "standby";
    String lastPowerMode = "normal";
    int lastPowerValue = 200;
    String lastDirection = "";

private:
    void _handleCommand();           // Handler for POST /command
    void _handleStatus();            // Handler for GET /status
    void _handleDiagnostic();        // Handler for GET /diagnostic
    void _handleSettings();          // Handler for POST /settings
    void _handleCorsOptions();       // Handler for OPTIONS (CORS preflight)
    void _addCorsHeaders();          // Add CORS headers to response
    void _handleFirmwarePage(WebServer* customServer = nullptr);      // Handler for GET /firmware (OTA upload page)
    void _handleFirmwareUpload(WebServer* customServer = nullptr);    // Handler for POST /update (OTA binary upload)
    
    // Power Management Web
    void _handlePowerPage(WebServer* customServer = nullptr);         // Handler for GET /power
    void _handlePowerData(WebServer* customServer = nullptr);         // Handler for GET /power-data
    
    String apiBaseUrl;           // Stored API URL
    Preferences preferences;     // Preferences for NVS storage
    
    bool isRegistered = false;
    unsigned long lastRegistrationAttempt = 0;
    
    // Battery Sync Tracking
    String lastSyncStatus = "Never Synced";
    unsigned long lastSyncTime = 0;
    String lastBatteryEventStatus = "None";
};

#endif
