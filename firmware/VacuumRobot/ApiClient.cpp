#include "ApiClient.h"
#include "config.h"
#include <ArduinoJson.h>
#include <WiFiManager.h>

// Global WebServer instance
WebServer server(ESP32_HTTP_PORT);

// ===== WIFI CONNECTION =====

void ApiClient::connectWiFi() {
    initBuzzer();
    
    Serial.println("=============================");
    Serial.println("Starting WiFi Manager...");
    
    WiFiManager wm;
    
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
        lastState = "working";
    } else if (command == "stop") {
        lastState = "stopping";
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

void ApiClient::startWebServer() {
    // CORS preflight
    server.on("/command", HTTP_OPTIONS, [this]() { _handleCorsOptions(); });
    server.on("/status", HTTP_OPTIONS, [this]() { _handleCorsOptions(); });
    
    // Actual endpoints
    server.on("/command", HTTP_POST, [this]() { _handleCommand(); });
    server.on("/status", HTTP_GET, [this]() { _handleStatus(); });
    
    server.begin();
    Serial.println(">>> HTTP Server started on port " + String(ESP32_HTTP_PORT));
    Serial.println(">>> Endpoints: POST /command, GET /status");
}

void ApiClient::handleWebServer() {
    server.handleClient();
}

// ===== DEVICE REGISTRATION =====

void ApiClient::registerDevice() {
    if (WiFi.status() != WL_CONNECTED) return;
    
    HTTPClient http;
    String url = String(API_BASE_URL) + "/register-device";
    
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
    String url = String(API_BASE_URL) + "/command-log";
    
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

    unsigned long now = millis();

    // Handle incoming direct HTTP commands
    handleWebServer();

    // Check Serial for "reset_wifi" command
    if (Serial.available()) {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim();
        if (cmd == "reset_wifi") {
            Serial.println("!!! RESETTING WIFI SETTINGS !!!");
            WiFiManager wm;
            wm.resetSettings();
            Serial.println("Settings cleared. Restarting ESP32...");
            playWiFiResetBeep();
            delay(500);
            ESP.restart();
        }
    }

    // Check Physical Button
    checkResetButton();

    // Fallback: Poll Status from Laravel (in case commands came via DB)
    if (now - _lastPollTime > API_POLL_INTERVAL) {
        _lastPollTime = now;
        getStatus(); 
    }
}

// ===== LEGACY: POLL STATUS FROM LARAVEL =====

String ApiClient::getStatus() {
    HTTPClient http;
    String url = String(API_BASE_URL) + "/status";
    
    http.begin(url);
    int httpCode = http.GET();
    
    if (httpCode == 200) {
        String payload = http.getString();
        _parseStatusResponse(payload);
        return payload;
    } else {
        Serial.println("Error getting status");
        return "";
    }
    http.end();
}

// ===== BATTERY DATA =====

void ApiClient::sendBattery(int percent, float voltage) {
    if (WiFi.status() != WL_CONNECTED) return;
    
    HTTPClient http;
    String url = String(API_BASE_URL) + "/battery";
    
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    
    DynamicJsonDocument doc(200);
    doc["battery_percent"] = percent;
    doc["battery_voltage"] = voltage;
    doc["estimated_time"] = String(percent / 10.0) + "h";
    
    String json;
    serializeJson(doc, json);
    
    int httpCode = http.POST(json);
    http.end();
}

// ===== PARSE STATUS RESPONSE =====

void ApiClient::_parseStatusResponse(String json) {
    DynamicJsonDocument doc(512);
    DeserializationError error = deserializeJson(doc, json);
    
    if (error) {
        Serial.print("JSON PARSE ERROR: ");
        Serial.println(error.c_str());
        return;
    }
    
    if (doc["success"]) {
        String newState = String((const char*)doc["data"]["state"]);
        String newMode = String((const char*)doc["data"]["power_mode"]);
        int newVal = doc["data"]["power_value"];
        String newDirection = doc["data"]["direction"] | "forward";
        
        // Log changes only
        if (newState != lastState) {
            Serial.print(">>> FALLBACK: State changed to ");
            Serial.println(newState);
        }
        if (newMode != lastPowerMode) {
            Serial.print(">>> FALLBACK: Power Mode changed to ");
            Serial.println(newMode);
        }
        
        lastState = newState;
        lastPowerMode = newMode;
        lastPowerValue = newVal;
        lastDirection = newDirection;
    }
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