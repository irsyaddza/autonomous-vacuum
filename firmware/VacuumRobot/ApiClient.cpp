#include "ApiClient.h"
#include "config.h"
#include <ArduinoJson.h>
#include "SensorArray.h"
#include "BatteryMonitor.h"
#include "TimingSettings.h"

extern SensorArray sensors;
extern BatteryMonitor battery;
#include <WiFiManager.h>

// Global WebServer instance
WebServer server(ESP32_HTTP_PORT);

// ===== WIFI CONNECTION =====

void ApiClient::connectWiFi() {
    initBuzzer();
    pinMode(PIN_WIFI_RESET, INPUT_PULLUP);
    
    // Load API URL from NVS Preferences
    preferences.begin("vacuum-app", false);
    apiBaseUrl = preferences.getString("api_url", DEFAULT_API_BASE_URL);
    
    Serial.println("=============================");
    Serial.println("Starting WiFi Manager...");
    Serial.print("Current API Base URL: ");
    Serial.println(apiBaseUrl);
    
    WiFiManager wm;
    
    // Setup Custom Parameter for API URL
    // max length 100 bytes
    char apiUrlStr[100];
    strncpy(apiUrlStr, apiBaseUrl.c_str(), sizeof(apiUrlStr));
    WiFiManagerParameter custom_api_url("api_url", "Server IP/URL (e.g. http://192.168.1.5:8000/v1/vacuum)", apiUrlStr, 100);
    
    wm.addParameter(&custom_api_url);
    
    // UI Customization - Premium Animated Dark Theme
    const char* custom_css = R"(
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        @import url('https://fonts.googleapis.com/css2?family=Outfit:wght@300;500;700&display=swap');
        
        :root {
            --primary: #4e73df;
            --secondary: #8e44ad;
            --bg-dark: #0f172a;
            --glass: rgba(255, 255, 255, 0.05);
            --border: rgba(255, 255, 255, 0.1);
        }

        body {
            background-color: var(--bg-dark);
            color: #e0e0e0;
            font-family: 'Outfit', 'Segoe UI', sans-serif;
            margin: 0;
            min-height: 100vh;
            display: flex;
            align-items: center;
            justify-content: center;
            background: linear-gradient(-45deg, #0f172a, #1e293b, #250838, #0f172a);
            background-size: 400% 400%;
            animation: gradientBG 15s ease infinite;
        }

        @keyframes gradientBG {
            0% { background-position: 0% 50%; }
            50% { background-position: 100% 50%; }
            100% { background-position: 0% 50%; }
        }

        div, form { width: 100%; }

        .wrap {
            background: var(--glass);
            backdrop-filter: blur(16px);
            -webkit-backdrop-filter: blur(16px);
            border: 1px solid var(--border);
            border-radius: 20px;
            padding: 40px 30px;
            box-shadow: 0 8px 32px 0 rgba(0, 0, 0, 0.37);
            max-width: 400px;
            width: 90%;
            margin: 20px auto;
            animation: fadeIn 0.8s ease-out;
            text-align: center;
        }

        @keyframes fadeIn {
            from { opacity: 0; transform: translateY(20px); }
            to { opacity: 1; transform: translateY(0); }
        }

        h1 {
            color: #fff;
            font-weight: 700;
            font-size: 1.8rem;
            margin-bottom: 5px;
            letter-spacing: 1px;
            text-shadow: 0 0 20px rgba(78, 115, 223, 0.5);
        }
        
        h3 { color: #a0aec0; font-weight: 300; font-size: 0.9rem; margin-top: 0; }

        input {
            width: 100%;
            box-sizing: border-box;
            background: rgba(0, 0, 0, 0.2);
            border: 1px solid var(--border);
            border-radius: 12px;
            padding: 15px;
            color: white;
            font-size: 1rem;
            margin-bottom: 15px;
            transition: all 0.3s ease;
        }

        input:focus {
            outline: none;
            border-color: var(--primary);
            box-shadow: 0 0 15px rgba(78, 115, 223, 0.3);
            background: rgba(0, 0, 0, 0.4);
        }

        button {
            width: 100%;
            border: none;
            padding: 15px;
            border-radius: 12px;
            font-size: 1rem;
            font-weight: 600;
            cursor: pointer;
            transition: all 0.3s ease;
            text-transform: uppercase;
            letter-spacing: 1px;
            margin-top: 10px;
            position: relative;
            overflow: hidden;
        }

        button[type='submit'] {
            background: linear-gradient(90deg, var(--primary), var(--secondary));
            color: white;
            box-shadow: 0 4px 15px rgba(78, 115, 223, 0.4);
        }

        button[type='submit']:hover {
            transform: translateY(-2px);
            box-shadow: 0 8px 25px rgba(78, 115, 223, 0.6);
        }

        button:not([type='submit']) {
            background: rgba(255, 255, 255, 0.1);
            color: #fff;
            border: 1px solid var(--border);
        }

        button:not([type='submit']):hover {
            background: rgba(255, 255, 255, 0.2);
        }

        div.c { text-align: left; }
        
        div, a { color: #b0c4de; text-decoration: none; transition: 0.3s; }
        a:hover { color: #fff; text-shadow: 0 0 10px white; }

        .q { float: right; }
        
        .footer {
            margin-top: 30px;
            font-size: 0.8rem;
            color: rgba(255,255,255,0.3);
        }
    </style>
    <script>
        document.addEventListener('DOMContentLoaded', function() {
            const h1 = document.querySelector('h1');
            if(h1) {
                h1.innerHTML = '🤖<br>' + h1.innerHTML;
                
                if(!document.querySelector('h3')) {
                    const h3 = document.createElement('h3');
                    h3.innerText = 'Vacuum Robot Setup';
                    h1.after(h3);
                }
            }
            
            const wrap = document.querySelector('.wrap');
            if(wrap) {
                const foot = document.createElement('div');
                foot.className = 'footer';
                foot.innerHTML = 'Powered by ESP32 & Laravel<br>ANTIGRAVITY SYSTEM';
                wrap.appendChild(foot);
            }
        });
    </script>
    )";
    wm.setCustomHeadElement(custom_css);
    
    Serial.print("Opening AP: ");
    Serial.println(WIFI_AP_NAME);
    bool res = wm.autoConnect(WIFI_AP_NAME, WIFI_AP_PASSWORD); 

    if(!res) {
        Serial.println("Failed to connect");
    } 
    else {
        Serial.println(">>> WIFI CONNECTED SUCCESS! <<<");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
        
        // WiFi connected melody — 2 cheerful beeps
        tone(PIN_BUZZER, 1047);  // C6
        delay(100);
        noTone(PIN_BUZZER);
        delay(50);
        tone(PIN_BUZZER, 1319);  // E6
        delay(150);
        noTone(PIN_BUZZER);
        
        // Save possibly updated API URL
        apiBaseUrl = String(custom_api_url.getValue());
        preferences.putString("api_url", apiBaseUrl);
        Serial.print("Saved API Base URL: ");
        Serial.println(apiBaseUrl);
        
        Serial.println("=============================");
    }
}

// ===== DIRECT HTTP WEB SERVER =====

void ApiClient::_addCorsHeaders() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
}

void ApiClient::_handleCorsOptions() {
    _addCorsHeaders();
    server.send(204);
}

void ApiClient::_handleCommand() {
    _addCorsHeaders();
    
    if (!server.hasArg("plain")) {
        server.send(400, "application/json", "{\"success\":false,\"message\":\"No body\"}");
        return;
    }
    
    String body = server.arg("plain");
    Serial.println(">>> DIRECT COMMAND RECEIVED <<<");
    Serial.print("Body: ");
    Serial.println(body);
    
    DynamicJsonDocument doc(256);
    DeserializationError error = deserializeJson(doc, body);
    
    if (error) {
        server.send(400, "application/json", "{\"success\":false,\"message\":\"Invalid JSON\"}");
        return;
    }
    
    String command = doc["command"] | "";
    int value = doc["value"] | -1;
    
    // Map commands to states
    if (command == "start") {
        // Block start if battery too low
        if (!battery.canStart()) {
            int pct = battery.getPercentage();
            Serial.print("[API] START BLOCKED - Battery too low: ");
            Serial.print(pct);
            Serial.println("%");
            
            DynamicJsonDocument errDoc(256);
            errDoc["success"] = false;
            errDoc["message"] = "Battery too low to start (" + String(pct) + "%). Need > " + String(BATTERY_BLOCK_START_PCT) + "%";
            errDoc["battery_percent"] = pct;
            errDoc["blocked"] = true;
            
            String errResponse;
            serializeJson(errDoc, errResponse);
            server.send(400, "application/json", errResponse);
            return;
        }
        lastState = "working";
        lastDirection = "autonomous";
    } else if (command == "stop") {
        lastState = "stopping";
        lastDirection = "forward";
    } else if (command == "return_home") {
        lastState = "returning";
    } else if (command == "eco") {
        lastPowerMode = "eco";
        lastPowerValue = 150;
    } else if (command == "normal") {
        lastPowerMode = "normal";
        lastPowerValue = 200;
    } else if (command == "strong") {
        lastPowerMode = "strong";
        lastPowerValue = 255;
    } else {
        server.send(400, "application/json", "{\"success\":false,\"message\":\"Unknown command\"}");
        return;
    }
    
    // If a custom power value was provided
    if (value >= 0 && value <= 255) {
        lastPowerValue = value;
    }
    
    Serial.print(">>> State: "); Serial.println(lastState);
    Serial.print(">>> Power Mode: "); Serial.println(lastPowerMode);
    Serial.print(">>> Power Value: "); Serial.println(lastPowerValue);
    
    // Build response
    DynamicJsonDocument resDoc(256);
    resDoc["success"] = true;
    resDoc["message"] = "Command executed: " + command;
    resDoc["state"] = lastState;
    resDoc["power_mode"] = lastPowerMode;
    resDoc["power_value"] = lastPowerValue;
    
    String response;
    serializeJson(resDoc, response);
    
    server.send(200, "application/json", response);
    
    // Log to Laravel server asynchronously
    logCommandToServer(command, "success", 0);
}

void ApiClient::_handleStatus() {
    _addCorsHeaders();
    
    DynamicJsonDocument doc(256);
    doc["success"] = true;
    doc["state"] = lastState;
    doc["power_mode"] = lastPowerMode;
    doc["power_value"] = lastPowerValue;
    doc["direction"] = lastDirection;
    doc["ip"] = WiFi.localIP().toString();
    
    String response;
    serializeJson(doc, response);
    
    server.send(200, "application/json", response);
}

void ApiClient::_handleDiagnostic() {
    _addCorsHeaders();
    
    // Read raw digital values from all 6 sensors
    int obsL = digitalRead(PIN_IR_LEFT);
    int obsF = digitalRead(PIN_IR_FRONT);
    int obsR = digitalRead(PIN_IR_RIGHT);
    int clfL = digitalRead(PIN_CLIFF_LEFT);
    int clfF = digitalRead(PIN_CLIFF_FRONT);
    int clfR = digitalRead(PIN_CLIFF_RIGHT);
    
    // Read battery
    float voltage = battery.getVoltage();
    int percent = battery.getPercentage();
    
    DynamicJsonDocument doc(1024);
    doc["success"] = true;
    
    // Raw digital values (0 or 1)
    JsonObject obs = doc.createNestedObject("obstacle_raw");
    obs["left"] = obsL;
    obs["front"] = obsF;
    obs["right"] = obsR;
    
    JsonObject clf = doc.createNestedObject("cliff_raw");
    clf["left"] = clfL;
    clf["front"] = clfF;
    clf["right"] = clfR;
    
    // Debounced (processed) values
    JsonObject obsD = doc.createNestedObject("obstacle_debounced");
    obsD["left"] = sensors.isLeftBlocked();
    obsD["front"] = sensors.isFrontBlocked();
    obsD["right"] = sensors.isRightBlocked();
    
    JsonObject clfD = doc.createNestedObject("cliff_debounced");
    clfD["left"] = sensors.isCliffLeft();
    clfD["front"] = sensors.isCliffFront();
    clfD["right"] = sensors.isCliffRight();
    
    // Robot state
    doc["state"] = lastState;
    doc["direction"] = lastDirection;
    doc["power_mode"] = lastPowerMode;
    doc["power_value"] = lastPowerValue;
    
    // Battery
    JsonObject bat = doc.createNestedObject("battery");
    bat["voltage"] = voltage;
    bat["percent"] = percent;
    
    // Uptime
    doc["uptime_ms"] = millis();
    
    // Current timing settings
    JsonObject ts = doc.createNestedObject("timing");
    ts["backupDuration"] = timing.backupDuration;
    ts["turnDurationMin"] = timing.turnDurationMin;
    ts["turnDurationMax"] = timing.turnDurationMax;
    ts["turnDurationSmall"] = timing.turnDurationSmall;
    ts["cliffBackupDuration"] = timing.cliffBackupDuration;
    ts["cliffTurnDuration"] = timing.cliffTurnDuration;
    ts["spiralInitialDuration"] = timing.spiralInitialDuration;
    ts["spiralIncrement"] = timing.spiralIncrement;
    ts["spiralMaxDuration"] = timing.spiralMaxDuration;
    ts["spiralTurnDuration"] = timing.spiralTurnDuration;
    ts["stuckObstacleCount"] = timing.stuckObstacleCount;
    ts["stuckTimeWindow"] = timing.stuckTimeWindow;
    ts["escapeTurnDuration"] = timing.escapeTurnDuration;
    
    String response;
    serializeJson(doc, response);
    server.send(200, "application/json", response);
}

void ApiClient::_handleSettings() {
    _addCorsHeaders();
    
    String body = server.arg("plain");
    DynamicJsonDocument doc(512);
    DeserializationError err = deserializeJson(doc, body);
    
    if (err) {
        server.send(400, "application/json", "{\"success\":false,\"error\":\"Invalid JSON\"}");
        return;
    }
    
    // Update hanya field yang dikirim (partial update)
    if (doc.containsKey("backupDuration"))      timing.backupDuration = doc["backupDuration"];
    if (doc.containsKey("turnDurationMin"))      timing.turnDurationMin = doc["turnDurationMin"];
    if (doc.containsKey("turnDurationMax"))      timing.turnDurationMax = doc["turnDurationMax"];
    if (doc.containsKey("turnDurationSmall"))    timing.turnDurationSmall = doc["turnDurationSmall"];
    if (doc.containsKey("cliffBackupDuration"))  timing.cliffBackupDuration = doc["cliffBackupDuration"];
    if (doc.containsKey("cliffTurnDuration"))    timing.cliffTurnDuration = doc["cliffTurnDuration"];
    if (doc.containsKey("spiralInitialDuration")) timing.spiralInitialDuration = doc["spiralInitialDuration"];
    if (doc.containsKey("spiralIncrement"))      timing.spiralIncrement = doc["spiralIncrement"];
    if (doc.containsKey("spiralMaxDuration"))    timing.spiralMaxDuration = doc["spiralMaxDuration"];
    if (doc.containsKey("spiralTurnDuration"))   timing.spiralTurnDuration = doc["spiralTurnDuration"];
    if (doc.containsKey("stuckObstacleCount"))   timing.stuckObstacleCount = doc["stuckObstacleCount"];
    if (doc.containsKey("stuckTimeWindow"))      timing.stuckTimeWindow = doc["stuckTimeWindow"];
    if (doc.containsKey("escapeTurnDuration"))   timing.escapeTurnDuration = doc["escapeTurnDuration"];
    
    // Reset ke default jika diminta
    if (doc.containsKey("resetDefaults") && doc["resetDefaults"] == true) {
        timing.resetDefaults();
    }
    
    timing.save();
    
    Serial.println("[API] Timing settings updated from web and saved to NVS");
    server.send(200, "application/json", "{\"success\":true,\"message\":\"Settings updated\"}");
}

void ApiClient::startWebServer() {
    // CORS preflight
    server.on("/command", HTTP_OPTIONS, [this]() { _handleCorsOptions(); });
    server.on("/status", HTTP_OPTIONS, [this]() { _handleCorsOptions(); });
    server.on("/diagnostic", HTTP_OPTIONS, [this]() { _handleCorsOptions(); });
    server.on("/settings", HTTP_OPTIONS, [this]() { _handleCorsOptions(); });
    
    // Actual endpoints
    server.on("/command", HTTP_POST, [this]() { _handleCommand(); });
    server.on("/status", HTTP_GET, [this]() { _handleStatus(); });
    server.on("/diagnostic", HTTP_GET, [this]() { _handleDiagnostic(); });
    server.on("/settings", HTTP_POST, [this]() { _handleSettings(); });
    
    server.begin();
    Serial.println(">>> HTTP Server started on port " + String(ESP32_HTTP_PORT));
    Serial.println(">>> Endpoints: POST /command, GET /status, GET /diagnostic, POST /settings");
}

void ApiClient::handleWebServer() {
    server.handleClient();
}

// ===== DEVICE REGISTRATION =====

void ApiClient::registerDevice() {
    if (WiFi.status() != WL_CONNECTED) return;
    
    HTTPClient http;
    String url = apiBaseUrl + "/register-device";
    
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    
    DynamicJsonDocument doc(256);
    doc["ip_address"] = WiFi.localIP().toString();
    doc["mac_address"] = WiFi.macAddress();
    doc["firmware_version"] = FIRMWARE_VERSION;
    
    String json;
    serializeJson(doc, json);
    
    Serial.println(">>> Registering device with server...");
    Serial.print("  IP: "); Serial.println(WiFi.localIP());
    Serial.print("  MAC: "); Serial.println(WiFi.macAddress());
    
    int httpCode = http.POST(json);
    
    if (httpCode == 200) {
        Serial.println(">>> Device registered successfully!");
    } else {
        Serial.print(">>> Device registration failed: HTTP ");
        Serial.println(httpCode);
    }
    
    http.end();
}

// ===== COMMAND HISTORY LOGGING =====

void ApiClient::logCommandToServer(String command, String status, int responseMs) {
    if (WiFi.status() != WL_CONNECTED) return;
    
    HTTPClient http;
    String url = apiBaseUrl + "/command-log";
    
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    
    DynamicJsonDocument doc(256);
    doc["command"] = command;
    doc["source"] = "esp32";
    doc["status"] = status;
    doc["response_time_ms"] = responseMs;
    doc["esp32_ip"] = WiFi.localIP().toString();
    
    String json;
    serializeJson(doc, json);
    
    int httpCode = http.POST(json);
    http.end();
    
    if (httpCode == 201) {
        Serial.println("  Command logged to server.");
    }
}

// ===== MAIN UPDATE LOOP =====

void ApiClient::update() {
    if (WiFi.status() != WL_CONNECTED) return;

    // Handle incoming direct HTTP commands from browser
    handleWebServer();

    // Check Serial for "reset_wifi" command
    if (Serial.available()) {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim();
        if (cmd == "reset_wifi") {
            Serial.println("!!! RESETTING WIFI SETTINGS !!!");
            WiFiManager wm;
            wm.resetSettings();
            preferences.begin("vacuum-app", false);
            preferences.clear();
            Serial.println("Settings cleared. Restarting ESP32...");
            playWiFiResetBeep();
            delay(500);
            ESP.restart();
        }
    }

    // Check Physical Button
    checkResetButton();
}

// ===== BATTERY DATA =====

void ApiClient::sendBattery(int percent, float voltage) {
    if (WiFi.status() != WL_CONNECTED) return;
    
    HTTPClient http;
    String url = apiBaseUrl + "/battery";
    
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    
    DynamicJsonDocument doc(200);
    doc["battery_percent"] = percent;
    doc["battery_voltage"] = voltage;
    doc["estimated_time"] = battery.getEstimatedTime(lastPowerMode);
    
    String json;
    serializeJson(doc, json);
    
    int httpCode = http.POST(json);
    http.end();
}

// ===== BATTERY EVENTS =====

void ApiClient::sendBatteryEvent(String event, int percent, float voltage) {
    if (WiFi.status() != WL_CONNECTED) return;
    
    HTTPClient http;
    String url = apiBaseUrl + "/battery-event";
    
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    
    DynamicJsonDocument doc(256);
    doc["event"] = event;
    doc["battery_percent"] = percent;
    doc["battery_voltage"] = voltage;
    doc["power_mode"] = lastPowerMode;
    
    String json;
    serializeJson(doc, json);
    
    Serial.print("[API] Sending battery event: ");
    Serial.println(event);
    
    int httpCode = http.POST(json);
    http.end();
    
    if (httpCode == 201) {
        Serial.println("[API] Battery event logged successfully");
    } else {
        Serial.print("[API] Battery event failed: HTTP ");
        Serial.println(httpCode);
    }
}

void ApiClient::sendAutoStop(int percent, float voltage) {
    // 1. Send battery event
    sendBatteryEvent("auto_stop_low_battery", percent, voltage);
    
    // 2. Update state on server to 'stopping' so dashboard reflects auto-stop
    if (WiFi.status() != WL_CONNECTED) return;
    
    HTTPClient http;
    String url = apiBaseUrl + "/command-log";
    
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    
    DynamicJsonDocument doc(256);
    doc["command"] = "auto_stop_low_battery";
    doc["source"] = "esp32";
    doc["status"] = "success";
    doc["response_time_ms"] = 0;
    doc["esp32_ip"] = WiFi.localIP().toString();
    
    String json;
    serializeJson(doc, json);
    
    int httpCode = http.POST(json);
    http.end();
    
    // Also update local state
    lastState = "stopping";
}


// ===== PHYSICAL BUTTON =====

void ApiClient::checkResetButton() {
    if (digitalRead(PIN_WIFI_RESET) == LOW) {
        unsigned long startPress = millis();
        bool longPress = false;
        
        Serial.print("Reset Button Pressed. Hold for 5 seconds...");
        
        while (digitalRead(PIN_WIFI_RESET) == LOW) {
            if (millis() - startPress > 5000) {
                longPress = true;
                Serial.println(" REQUIRED TIME REACHED!");
                break;
            }
            delay(100);
            Serial.print(".");
        }
        
        if (longPress) {
            Serial.println("\n!!! RESETTING WIFI SETTINGS !!!");
            WiFiManager wm;
            wm.resetSettings();
            preferences.begin("vacuum-app", false);
            preferences.clear();
            Serial.println("Settings cleared. Restarting...");
            playWiFiResetBeep();
            delay(500);
            ESP.restart();
        } else {
             Serial.println("\nButton released too early. Cancelled.");
        }
    }
}

// ===== BUZZER FUNCTIONS =====

void ApiClient::initBuzzer() {
    pinMode(PIN_BUZZER, OUTPUT);
    digitalWrite(PIN_BUZZER, LOW);
    Serial.println("Buzzer initialized on GPIO 27");
}

void ApiClient::playWiFiResetBeep() {
    Serial.println("Playing WiFi reset beep...");
    
    for (int i = 0; i < 3; i++) {
        digitalWrite(PIN_BUZZER, HIGH);
        delay(150);
        digitalWrite(PIN_BUZZER, LOW);
        delay(100);
    }
    
    delay(200);
    digitalWrite(PIN_BUZZER, HIGH);
    delay(500);
    digitalWrite(PIN_BUZZER, LOW);
    
    Serial.println("Beep complete!");
}