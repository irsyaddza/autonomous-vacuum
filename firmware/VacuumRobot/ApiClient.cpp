#include "ApiClient.h"
#include "config.h"
#include <ArduinoJson.h>
#include "SensorArray.h"
#include "BatteryMonitor.h"
#include "TimingSettings.h"
#include "WheelMotor.h"

extern SensorArray sensors;
extern BatteryMonitor battery;
extern WheelMotor wheels;
#include <WiFiManager.h>
#include <Update.h>
#include <esp_wifi.h>

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
        @import url('https://fonts.googleapis.com/css2?family=Outfit:wght@300;400;500;600;700&display=swap');
        
        :root {
            --accent: #d4a054;
            --accent-glow: rgba(212,160,84,0.3);
            --bg: #0c0c0f;
            --surface: rgba(255,255,255,0.04);
            --border: rgba(255,255,255,0.08);
            --text: #e8e4df;
            --muted: #6b6560;
        }

        body {
            background-color: var(--bg);
            color: var(--text);
            font-family: 'Outfit', 'Segoe UI', sans-serif;
            margin: 0;
            min-height: 100dvh;
            display: flex;
            align-items: center;
            justify-content: center;
        }


        div, form { width: 100%; }

        .wrap {
            background: var(--surface);
            backdrop-filter: blur(20px);
            -webkit-backdrop-filter: blur(20px);
            border: 1px solid var(--border);
            border-radius: 20px;
            padding: 40px 30px;
            box-shadow: 0 8px 32px rgba(0,0,0,0.4), inset 0 1px 0 rgba(255,255,255,0.04);
            max-width: 400px;
            width: 90%;
            margin: 20px auto;
            animation: fadeIn 0.6s ease-out;
            text-align: center;
        }

        @keyframes fadeIn {
            from { opacity: 0; transform: translateY(16px); }
            to { opacity: 1; transform: translateY(0); }
        }

        h1 {
            color: #fff;
            font-weight: 700;
            font-size: 1.8rem;
            margin-bottom: 5px;
            letter-spacing: -0.5px;
        }
        
        h3 { color: var(--muted); font-weight: 300; font-size: 0.9rem; margin-top: 0; }

        input {
            width: 100%;
            box-sizing: border-box;
            background: rgba(255,255,255,0.03);
            border: 1px solid var(--border);
            border-radius: 10px;
            padding: 15px;
            color: var(--text);
            font-size: 1rem;
            font-family: 'Outfit', sans-serif;
            margin-bottom: 15px;
            transition: border-color 0.25s ease, box-shadow 0.25s ease;
        }

        input:focus {
            outline: none;
            border-color: var(--accent);
            box-shadow: 0 0 0 3px var(--accent-glow);
        }

        button {
            width: 100%;
            border: none;
            padding: 15px;
            border-radius: 12px;
            font-size: 1rem;
            font-weight: 600;
            cursor: pointer;
            font-family: 'Outfit', sans-serif;
            transition: transform 0.2s ease, box-shadow 0.2s ease, background 0.2s ease;
            letter-spacing: 0.3px;
            margin-top: 10px;
            position: relative;
            overflow: hidden;
        }

        button[type='submit'] {
            background: var(--accent);
            color: #0c0c0f;
            box-shadow: 0 4px 16px var(--accent-glow);
        }

        button[type='submit']:hover {
            transform: translateY(-2px);
            box-shadow: 0 8px 24px var(--accent-glow);
        }

        button[type='submit']:active {
            transform: translateY(0) scale(0.98);
        }

        button:not([type='submit']) {
            background: rgba(255,255,255,0.06);
            color: var(--text);
            border: 1px solid var(--border);
        }

        button:not([type='submit']):hover {
            background: rgba(255,255,255,0.10);
        }

        button:not([type='submit']):active {
            transform: scale(0.98);
        }

        div.c { text-align: left; }
        
        div, a { color: var(--muted); text-decoration: none; transition: color 0.2s ease; }
        a:hover { color: var(--text); }

        .q { float: right; }
        
        .footer {
            margin-top: 30px;
            font-size: 0.75rem;
            color: rgba(255,255,255,0.2);
            letter-spacing: 0.5px;
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
                foot.innerHTML = 'Powered by ESP32 & Laravel';
                wrap.appendChild(foot);
            }
        });
    </script>
    )";
    wm.setCustomHeadElement(custom_css);
    
    // Inject custom routes into WiFiManager's web server so /power is accessible in AP mode (192.168.4.1)
    wm.setWebServerCallback([&wm, this]() {
        if (wm.server) {
            wm.server->on("/power", HTTP_GET, [&wm, this]() { _handlePowerPage(wm.server.get()); });
            wm.server->on("/power-data", HTTP_GET, [&wm, this]() { _handlePowerData(wm.server.get()); });
            wm.server->on("/firmware", HTTP_GET, [&wm, this]() { _handleFirmwarePage(wm.server.get()); });
            wm.server->on("/update", HTTP_POST, [&wm, this]() {
                wm.server->sendHeader("Connection", "close");
                bool success = !Update.hasError();
                String msg = success ? "{\"success\":true,\"message\":\"Firmware updated! Restarting robot...\"}" 
                                     : "{\"success\":false,\"message\":\"Update FAILED! Check serial log.\"}";
                wm.server->send(success ? 200 : 500, "application/json", msg);
                delay(500);
                ESP.restart();
            }, [&wm, this]() { _handleFirmwareUpload(wm.server.get()); });
        }
    });
    
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
        
        // Enable WiFi Modem Sleep for power saving
        // Reduces WiFi radio power ~30-50% while still allowing HTTP server to work
        // The modem wakes automatically at DTIM beacon intervals
        esp_wifi_set_ps(WIFI_PS_MIN_MODEM);
        Serial.println("[POWER] WiFi Modem Sleep enabled (power saving mode)");
        
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
    int obsL = digitalRead(PIN_OB_LEFT);
    int obsF = digitalRead(PIN_OB_FRONT);
    int obsR = digitalRead(PIN_OB_RIGHT);
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
    ts["respiralIdleTime"] = timing.respiralIdleTime;
    ts["stuckObstacleCount"] = timing.stuckObstacleCount;
    ts["stuckTimeWindow"] = timing.stuckTimeWindow;
    ts["escapeTurnDuration"] = timing.escapeTurnDuration;
    ts["leftWheelSpeed"] = timing.leftWheelSpeed;
    ts["rightWheelSpeed"] = timing.rightWheelSpeed;
    
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
    if (doc.containsKey("respiralIdleTime"))      timing.respiralIdleTime = doc["respiralIdleTime"];
    if (doc.containsKey("stuckObstacleCount"))   timing.stuckObstacleCount = doc["stuckObstacleCount"];
    if (doc.containsKey("stuckTimeWindow"))      timing.stuckTimeWindow = doc["stuckTimeWindow"];
    if (doc.containsKey("escapeTurnDuration"))   timing.escapeTurnDuration = doc["escapeTurnDuration"];
    if (doc.containsKey("leftWheelSpeed")) {
        timing.leftWheelSpeed = doc["leftWheelSpeed"];
        wheels.setLeftSpeed(timing.leftWheelSpeed);
    }
    if (doc.containsKey("rightWheelSpeed")) {
        timing.rightWheelSpeed = doc["rightWheelSpeed"];
        wheels.setRightSpeed(timing.rightWheelSpeed);
    }
    
    // Reset ke default jika diminta
    if (doc.containsKey("resetDefaults") && doc["resetDefaults"] == true) {
        timing.resetDefaults();
        wheels.setLeftSpeed(timing.leftWheelSpeed);
        wheels.setRightSpeed(timing.rightWheelSpeed);
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
    
    // Power Management Web
    server.on("/power", HTTP_GET, [this]() { _handlePowerPage(&server); });
    server.on("/power-data", HTTP_GET, [this]() { _handlePowerData(&server); });
    
    // OTA Firmware Update
    server.on("/firmware", HTTP_GET, [this]() { _handleFirmwarePage(&server); });
    server.on("/update", HTTP_POST, [this]() {
        server.sendHeader("Connection", "close");
        bool success = !Update.hasError();
        String msg = success ? "{\"success\":true,\"message\":\"Firmware updated! Restarting robot...\"}" 
                             : "{\"success\":false,\"message\":\"Update FAILED! Check serial log.\"}";
        server.send(success ? 200 : 500, "application/json", msg);
        delay(500);
        ESP.restart();
    }, [this]() { _handleFirmwareUpload(&server); });
    
    server.begin();
    Serial.println(">>> HTTP Server started on port " + String(ESP32_HTTP_PORT));
    Serial.println(">>> Endpoints: POST /command, GET /status, GET /diagnostic, POST /settings");
    Serial.println(">>> OTA Update : GET /firmware  |  POST /update");
}

void ApiClient::handleWebServer() {
    server.handleClient();
}

// ===== DEVICE REGISTRATION =====

void ApiClient::registerDevice() {
    if (WiFi.status() != WL_CONNECTED) return;
    if (isRegistered) return;

    String url = apiBaseUrl + "/register-device";

    DynamicJsonDocument doc(256);
    doc["ip_address"]       = WiFi.localIP().toString();
    doc["mac_address"]      = WiFi.macAddress();
    doc["firmware_version"] = FIRMWARE_VERSION;

    String json;
    serializeJson(doc, json);

    Serial.println(">>> Registering device with server...");
    Serial.print("  URL: "); Serial.println(url);
    Serial.print("  IP:  "); Serial.println(WiFi.localIP());

    HTTPClient http;
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    http.setTimeout(3000);

    int httpCode = http.POST(json);
    http.end();

    if (httpCode == 200) {
        Serial.println(">>> Device registered successfully!");
        isRegistered = true;
    } else {
        Serial.print(">>> Registration failed: HTTP ");
        Serial.println(httpCode);
    }
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

// ===== OTA FIRMWARE UPDATE =====

void ApiClient::_handleFirmwarePage(WebServer* customServer) {
    String html = "";
    html += "<!DOCTYPE html><html lang=\"en\"><head>";
    html += "<meta charset=\"UTF-8\">";
    html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
    html += "<title>Firmware Update - Vacuum Robot</title>";
    html += "<style>";
    html += "@import url('https://fonts.googleapis.com/css2?family=Outfit:wght@300;400;500;600;700&display=swap');";
    html += ":root{--accent:#d4a054;--accent-glow:rgba(212,160,84,0.3);--surface:rgba(255,255,255,0.04);--border:rgba(255,255,255,0.08);--text:#e8e4df;--muted:#6b6560;}";
    html += "*{box-sizing:border-box;margin:0;padding:0;}";
    html += "body{background-color:#0c0c0f;font-family:'Outfit',sans-serif;color:var(--text);min-height:100dvh;display:flex;align-items:center;justify-content:center;padding:20px;}";
    html += ".card{background:var(--surface);backdrop-filter:blur(20px);-webkit-backdrop-filter:blur(20px);border:1px solid var(--border);border-radius:24px;padding:48px 40px;width:100%;max-width:480px;box-shadow:0 25px 50px rgba(0,0,0,0.4),inset 0 1px 0 rgba(255,255,255,0.03);animation:fadeUp 0.7s ease-out;text-align:center;}";
    html += "@keyframes fadeUp{from{opacity:0;transform:translateY(30px);}to{opacity:1;transform:translateY(0);}}";
    html += ".robot-icon{font-size:4rem;display:block;margin-bottom:12px;animation:float 3s ease-in-out infinite;}";
    html += "@keyframes float{0%,100%{transform:translateY(0);}50%{transform:translateY(-8px);}}";
    html += "h1{font-size:1.6rem;font-weight:700;color:#fff;letter-spacing:-0.3px;}";
    html += ".subtitle{color:var(--muted);font-size:0.85rem;margin-top:6px;margin-bottom:32px;}";
    html += ".info-badge{display:inline-flex;align-items:center;gap:8px;background:rgba(212,160,84,0.1);border:1px solid rgba(212,160,84,0.25);border-radius:50px;padding:6px 16px;font-size:0.78rem;color:var(--accent);margin-bottom:28px;}";
    html += ".drop-zone{border:2px dashed var(--border);border-radius:16px;padding:36px 20px;cursor:pointer;transition:all 0.3s ease;position:relative;background:rgba(0,0,0,0.2);}";
    html += ".drop-zone:hover,.drop-zone.drag-over{border-color:var(--accent);background:rgba(212,160,84,0.05);box-shadow:0 0 20px rgba(212,160,84,0.15);}";
    html += ".drop-zone input[type=file]{position:absolute;inset:0;opacity:0;cursor:pointer;width:100%;height:100%;}";
    html += ".drop-icon{font-size:2.5rem;display:block;margin-bottom:10px;}";
    html += ".drop-text{font-size:0.95rem;color:var(--muted);}";
    html += ".drop-text strong{color:var(--accent);}";
    html += ".file-info{display:none;margin-top:16px;background:rgba(92,181,133,0.08);border:1px solid rgba(92,181,133,0.25);border-radius:12px;padding:12px 16px;font-size:0.85rem;color:#7bed9f;text-align:left;}";
    html += ".file-info.visible{display:flex;align-items:center;gap:10px;}";
    html += ".progress-wrap{display:none;margin-top:20px;}";
    html += ".progress-wrap.visible{display:block;}";
    html += ".progress-label{display:flex;justify-content:space-between;font-size:0.8rem;color:var(--muted);margin-bottom:8px;}";
    html += ".progress-bar-bg{background:rgba(255,255,255,0.06);border-radius:50px;height:10px;overflow:hidden;}";
    html += ".progress-bar-fill{height:100%;width:0%;background:var(--accent);border-radius:50px;transition:width 0.3s ease;box-shadow:0 0 10px rgba(212,160,84,0.4);}";
    html += ".btn-upload{width:100%;margin-top:24px;padding:16px;border:none;border-radius:14px;background:var(--accent);color:#0c0c0f;font-family:'Outfit',sans-serif;font-size:1rem;font-weight:600;letter-spacing:0.3px;cursor:pointer;transition:transform 0.2s ease,box-shadow 0.2s ease;box-shadow:0 4px 20px rgba(212,160,84,0.3);}";
    html += ".btn-upload:hover:not(:disabled){transform:translateY(-2px);box-shadow:0 8px 30px rgba(212,160,84,0.4);}";
    html += ".btn-upload:active:not(:disabled){transform:translateY(0) scale(0.98);}";
    html += ".btn-upload:disabled{opacity:0.4;cursor:not-allowed;transform:none;}";
    html += ".status-msg{display:none;margin-top:16px;padding:14px 18px;border-radius:12px;font-size:0.9rem;font-weight:500;}";
    html += ".status-msg.success{display:block;background:rgba(92,181,133,0.1);border:1px solid rgba(92,181,133,0.3);color:#7bed9f;}";
    html += ".status-msg.error{display:block;background:rgba(196,92,92,0.1);border:1px solid rgba(196,92,92,0.3);color:#ff7675;}";
    html += ".warning-box{margin-top:24px;padding:14px 16px;border-radius:12px;background:rgba(212,160,84,0.06);border:1px solid rgba(212,160,84,0.2);font-size:0.78rem;color:var(--accent);text-align:left;line-height:1.6;}";
    html += ".warning-box strong{color:#e8b95a;}";
    html += ".footer{margin-top:28px;font-size:0.72rem;color:rgba(255,255,255,0.2);letter-spacing:0.5px;}";
    html += "</style></head><body>";
    html += "<div class=\"card\">";
    html += "<span class=\"robot-icon\">&#x1F916;</span>";
    html += "<h1>Firmware Update</h1>";
    html += "<p class=\"subtitle\">Over-The-Air Update &mdash; Vacuum Robot</p>";
    html += "<div class=\"info-badge\"><span>&#x26A1;</span><span>Current: v";
    html += String(FIRMWARE_VERSION);
    html += "</span></div>";
    html += "<form id=\"uploadForm\">";
    html += "<div class=\"drop-zone\" id=\"dropZone\">";
    html += "<input type=\"file\" id=\"fileInput\" name=\"update\" accept=\".bin\">";
    html += "<span class=\"drop-icon\">&#x1F4E6;</span>";
    html += "<p class=\"drop-text\">Drag &amp; drop file <strong>.bin</strong> di sini<br><small>atau klik untuk pilih file</small></p>";
    html += "</div>";
    html += "<div class=\"file-info\" id=\"fileInfo\"><span>&#x1F4C4;</span><span id=\"fileName\">-</span></div>";
    html += "<div class=\"progress-wrap\" id=\"progressWrap\">";
    html += "<div class=\"progress-label\"><span>Uploading firmware...</span><span id=\"progressPct\">0%</span></div>";
    html += "<div class=\"progress-bar-bg\"><div class=\"progress-bar-fill\" id=\"progressBar\"></div></div>";
    html += "</div>";
    html += "<button type=\"button\" class=\"btn-upload\" id=\"uploadBtn\" disabled onclick=\"startUpload()\">&#x1F680; Flash Firmware</button>";
    html += "</form>";
    html += "<div class=\"status-msg\" id=\"statusMsg\"></div>";
    html += "<div class=\"warning-box\">&#x26A0; <strong>Perhatian:</strong> Pastikan file <code>.bin</code> yang diupload benar dan sesuai untuk ESP32. Robot akan <strong>restart otomatis</strong> setelah update berhasil. Jangan cabut daya saat proses berlangsung.</div>";
    html += "<div class=\"footer\">Powered by ESP32 &amp; Laravel</div>";
    html += "</div>";
    html += "<script>";
    html += "var selectedFile=null;";
    html += "var dz=document.getElementById(\"dropZone\");";
    html += "var fi=document.getElementById(\"fileInput\");";
    html += "var ub=document.getElementById(\"uploadBtn\");";
    html += "var foi=document.getElementById(\"fileInfo\");";
    html += "var fn=document.getElementById(\"fileName\");";
    html += "var pw=document.getElementById(\"progressWrap\");";
    html += "var pb=document.getElementById(\"progressBar\");";
    html += "var pp=document.getElementById(\"progressPct\");";
    html += "var sm=document.getElementById(\"statusMsg\");";
    html += "dz.addEventListener(\"dragover\",function(e){e.preventDefault();dz.classList.add(\"drag-over\");});";
    html += "dz.addEventListener(\"dragleave\",function(){dz.classList.remove(\"drag-over\");});";
    html += "dz.addEventListener(\"drop\",function(e){e.preventDefault();dz.classList.remove(\"drag-over\");var dt=e.dataTransfer;if(dt.files.length){selectedFile=dt.files[0];hf(selectedFile);}});";
    html += "fi.addEventListener(\"change\",function(){if(fi.files.length){selectedFile=fi.files[0];hf(selectedFile);}});";
    html += "function hf(file){if(!file.name.endsWith(\".bin\")){ss(\"error\",\"[X] File harus berformat .bin!\");return;}";
    html += "fn.textContent=file.name+\" (\"+(file.size/1024).toFixed(1)+\" KB)\";";
    html += "foi.classList.add(\"visible\");ub.disabled=false;sm.className=\"status-msg\";sm.textContent=\"\";}";
    html += "function startUpload(){var file=selectedFile;if(!file){ss(\"error\",\"[X] Pilih file .bin terlebih dahulu!\");return;}";
    html += "ub.disabled=true;ub.textContent=\"Uploading...\";pw.classList.add(\"visible\");sm.className=\"status-msg\";";
    html += "var xhr=new XMLHttpRequest();xhr.open(\"POST\",\"/update\",true);";
    html += "xhr.upload.onprogress=function(e){if(e.lengthComputable){var pct=Math.round((e.loaded/e.total)*100);pb.style.width=pct+\"%\";pp.textContent=pct+\"%\";}};";
    html += "xhr.onload=function(){if(xhr.status===200){pb.style.width=\"100%\";pp.textContent=\"100%\";ss(\"success\",\"[OK] Firmware berhasil diupload! Robot sedang restart...\");ub.textContent=\"Selesai!\";}";
    html += "else{ss(\"error\",\"[X] Upload gagal: \"+xhr.responseText);ub.disabled=false;ub.textContent=\"Flash Firmware\";}};";
    html += "xhr.onerror=function(){ss(\"error\",\"[X] Koneksi terputus.\");ub.textContent=\"Flash Firmware\";};";
    html += "var fd=new FormData();fd.append(\"update\",file);xhr.send(fd);}";
    html += "function ss(t,m){sm.className=\"status-msg \"+t;sm.textContent=m;}";
    html += "</script></body></html>";
    WebServer* activeServer = customServer ? customServer : &server;
    activeServer->send(200, "text/html", html);
}

void ApiClient::_handleFirmwareUpload(WebServer* customServer) {
    WebServer* activeServer = customServer ? customServer : &server;
    HTTPUpload& upload = activeServer->upload();

    if (upload.status == UPLOAD_FILE_START) {
        Serial.print("[OTA] Starting update: ");
        Serial.println(upload.filename);
        // Start OTA - use UPDATE_SIZE_UNKNOWN so any .bin can be used
        if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {
            Update.printError(Serial);
        }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
            Update.printError(Serial);
        }
        Serial.print(".");
    } else if (upload.status == UPLOAD_FILE_END) {
        if (Update.end(true)) {
            Serial.println();
            Serial.print("[OTA] Update success! Total bytes: ");
            Serial.println(upload.totalSize);
        } else {
            Update.printError(Serial);
        }
    }
}

// ===== MAIN UPDATE LOOP =====

void ApiClient::update() {
    if (WiFi.status() != WL_CONNECTED) {
        isRegistered = false;
        return;
    }

    // Handle incoming direct HTTP commands from browser
    handleWebServer();
    
    // Auto retry registration every 10 seconds if failed
    if (!isRegistered && millis() - lastRegistrationAttempt > 10000) {
        lastRegistrationAttempt = millis();
        if (lastRegistrationAttempt == 0) lastRegistrationAttempt = 1;
        registerDevice();
    }

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
    
    lastSyncTime = millis();
    if (httpCode == 201 || httpCode == 200) {
        lastSyncStatus = "Success (HTTP " + String(httpCode) + ")";
    } else {
        lastSyncStatus = "Failed (HTTP " + String(httpCode) + ")";
    }
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
    
    lastSyncTime = millis();
    
    if (httpCode == 201 || httpCode == 200) {
        lastBatteryEventStatus = "Success: " + event;
        lastSyncStatus = "Event Success (HTTP " + String(httpCode) + ")";
        Serial.println("[API] Battery event logged successfully");
    } else {
        lastBatteryEventStatus = "Failed: " + event;
        lastSyncStatus = "Event Failed (HTTP " + String(httpCode) + ")";
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
        unsigned long lastBeepTime = 0;
        
        Serial.print("Reset Button Pressed. Hold for 5 seconds...");
        
        while (digitalRead(PIN_WIFI_RESET) == LOW) {
            unsigned long elapsed = millis() - startPress;
            
            if (elapsed > 5000) {
                longPress = true;
                Serial.println(" BOOM!");
                break;
            }
            
            // Countdown bomb: beeps accelerate as time runs out
            // Phase 1 (0-1s): 1 beep/sec  |  Phase 2 (1-2s): 2 beeps/sec
            // Phase 3 (2-3s): 3 beeps/sec |  Phase 4 (3-4s): 5 beeps/sec
            // Phase 5 (4-5s): 8 beeps/sec (rapid ticking)
            int interval;
            int beepLen;
            if (elapsed < 1000)       { interval = 1000; beepLen = 120; }
            else if (elapsed < 2000)  { interval = 500;  beepLen = 100; }
            else if (elapsed < 3000)  { interval = 333;  beepLen = 80; }
            else if (elapsed < 4000)  { interval = 200;  beepLen = 50; }
            else                      { interval = 125;  beepLen = 30; }
            
            // Pitch rises with urgency
            int pitch = 800 + (elapsed / 5);  // 800Hz → 1800Hz
            
            if (millis() - lastBeepTime >= (unsigned long)interval) {
                lastBeepTime = millis();
                tone(PIN_BUZZER, pitch, beepLen);
                Serial.print("tick ");
            }
            
            delay(10);
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
            noTone(PIN_BUZZER);
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
    Serial.println("Playing bomb explosion sound...");
    
    // BOOM: loud low-frequency burst
    tone(PIN_BUZZER, 150);
    delay(300);
    
    // Descending explosion sweep
    for (int f = 150; f > 30; f -= 5) {
        tone(PIN_BUZZER, f);
        delay(15);
    }
    noTone(PIN_BUZZER);
    delay(100);
    
    // Crackle / debris: rapid noise bursts
    for (int i = 0; i < 8; i++) {
        tone(PIN_BUZZER, random(200, 800), 25);
        delay(random(30, 70));
    }
    noTone(PIN_BUZZER);
    delay(80);
    
    // Final low boom
    tone(PIN_BUZZER, 60);
    delay(400);
    noTone(PIN_BUZZER);
    
    Serial.println("Explosion complete!");
}

// ===== POWER MANAGEMENT WEB =====

void ApiClient::_handlePowerPage(WebServer* customServer) {
    String html = "";
    html += "<!DOCTYPE html><html lang=\"en\"><head>";
    html += "<meta charset=\"UTF-8\">";
    html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
    html += "<title>Power Management - Vacuum</title>";
    html += "<style>";
    html += "@import url('https://fonts.googleapis.com/css2?family=Outfit:wght@300;400;500;600;700&display=swap');";
    html += ":root{--accent:#d4a054;--accent-glow:rgba(212,160,84,0.25);--success:#5cb585;--warning:#d4a054;--danger:#c45c5c;--bg:#0c0c0f;--card:rgba(255,255,255,0.04);--text:#e8e4df;--text-light:#6b6560;--border:rgba(255,255,255,0.08);}";
    html += "*{box-sizing:border-box;margin:0;padding:0;font-family:'Outfit',sans-serif;}";
    html += "body{background-color:var(--bg);color:var(--text);line-height:1.5;padding:20px;min-height:100dvh;}";
    html += ".container{max-width:600px;margin:0 auto;animation:fadeUp 0.6s ease-out;}";
    html += "@keyframes fadeUp{from{opacity:0;transform:translateY(20px);}to{opacity:1;transform:translateY(0);}}";
    html += ".header{text-align:center;margin-bottom:30px;}";
    html += ".header h1{font-size:1.5rem;font-weight:700;color:#fff;margin-bottom:5px;letter-spacing:-0.3px;}";
    html += ".header p{color:var(--text-light);font-size:0.9rem;}";
    html += ".card{background:var(--card);backdrop-filter:blur(20px);-webkit-backdrop-filter:blur(20px);border:1px solid var(--border);border-radius:16px;padding:20px;margin-bottom:20px;box-shadow:0 4px 24px rgba(0,0,0,0.3),inset 0 1px 0 rgba(255,255,255,0.03);transition:box-shadow 0.3s ease;}";
    html += ".card:hover{box-shadow:0 8px 32px rgba(0,0,0,0.4),inset 0 1px 0 rgba(255,255,255,0.03);}";
    html += ".grid{display:grid;grid-template-columns:1fr 1fr;gap:15px;}";
    html += ".data-item{display:flex;flex-direction:column;}";
    html += ".data-label{font-size:0.75rem;color:var(--text-light);text-transform:uppercase;letter-spacing:0.5px;font-weight:600;margin-bottom:4px;}";
    html += ".data-value{font-size:1.25rem;font-weight:700;color:var(--text);font-variant-numeric:tabular-nums;}";
    html += ".data-value.large{font-size:2.5rem;color:var(--accent);}";
    html += ".badge{display:inline-block;padding:4px 10px;border-radius:999px;font-size:0.75rem;font-weight:600;}";
    html += ".badge-success{background:rgba(92,181,133,0.12);color:var(--success);}";
    html += ".badge-warning{background:rgba(212,160,84,0.12);color:var(--warning);}";
    html += ".badge-danger{background:rgba(196,92,92,0.12);color:var(--danger);}";
    html += ".badge-neutral{background:rgba(255,255,255,0.05);color:var(--text-light);}";
    html += ".progress-container{width:100%;background:rgba(255,255,255,0.06);border-radius:999px;height:12px;margin-top:10px;overflow:hidden;}";
    html += ".progress-bar{height:100%;background:var(--accent);border-radius:999px;transition:width 0.5s ease,background-color 0.5s ease;box-shadow:0 0 8px var(--accent-glow);}";
    html += ".footer{text-align:center;font-size:0.75rem;color:rgba(255,255,255,0.2);margin-top:30px;letter-spacing:0.5px;}";
    html += ".sync-card{background:rgba(255,255,255,0.02);border:1px dashed rgba(255,255,255,0.08);}";
    html += "@media (max-width:480px){.grid{grid-template-columns:1fr;}}";
    html += "</style></head><body>";
    html += "<div class=\"container\">";
    html += "<div class=\"header\">";
    html += "<h1>⚡ Power Management</h1>";
    html += "<p>Local Diagnostics & Telemetry Dashboard</p>";
    html += "</div>";
    html += "<div class=\"card\">";
    html += "<div class=\"data-item\" style=\"align-items:center;margin-bottom:20px;\">";
    html += "<span class=\"data-label\">Battery Level</span>";
    html += "<span class=\"data-value large\" id=\"val-percent\">--%</span>";
    html += "<div class=\"progress-container\"><div class=\"progress-bar\" id=\"bar-percent\" style=\"width:0%\"></div></div>";
    html += "</div>";
    html += "<div class=\"grid\">";
    html += "<div class=\"data-item\"><span class=\"data-label\">Voltage</span><span class=\"data-value\" id=\"val-voltage\">-- V</span></div>";
    html += "<div class=\"data-item\"><span class=\"data-label\">Raw ADC</span><span class=\"data-value\" id=\"val-adc\">--</span></div>";
    html += "<div class=\"data-item\"><span class=\"data-label\">Est. Runtime</span><span class=\"data-value\" id=\"val-time\">--</span></div>";
    html += "<div class=\"data-item\"><span class=\"data-label\">Power Mode</span><span class=\"data-value\" id=\"val-mode\">--</span></div>";
    html += "</div></div>";
    html += "<div class=\"card sync-card\">";
    html += "<h3 style=\"font-size:1rem;margin-bottom:15px;color:var(--text);\">Server Synchronization</h3>";
    html += "<div class=\"grid\">";
    html += "<div class=\"data-item\"><span class=\"data-label\">Last Sync Status</span><span class=\"badge badge-neutral\" id=\"val-sync\" style=\"width:fit-content;\">--</span></div>";
    html += "<div class=\"data-item\" style=\"grid-column: 1 / -1;\"><span class=\"data-label\">Time since last sync</span><span class=\"data-value\" style=\"font-size:1rem;\" id=\"val-ago\">--</span></div>";
    html += "</div></div>";
    html += "<div class=\"footer\">ESP32 Local Dashboard • Auto-refreshing</div>";
    html += "</div>";
    html += "<script>";
    html += "function updateData(){";
    html += "fetch('/power-data').then(r=>r.json()).then(d=>{";
    html += "document.getElementById('val-percent').innerText = d.percent + '%';";
    html += "document.getElementById('val-voltage').innerText = d.voltage.toFixed(2) + ' V';";
    html += "document.getElementById('val-adc').innerText = d.raw_adc;";
    html += "document.getElementById('val-time').innerText = d.estimated_time;";
    html += "document.getElementById('val-mode').innerText = d.power_mode.toUpperCase();";
    html += "const bar = document.getElementById('bar-percent');";
    html += "bar.style.width = d.percent + '%';";
    html += "bar.style.backgroundColor = d.percent > 50 ? 'var(--success)' : d.percent > 20 ? 'var(--warning)' : 'var(--danger)';";
    html += "const syncBadge = document.getElementById('val-sync');";
    html += "syncBadge.innerText = d.sync_status;";
    html += "syncBadge.className = 'badge ' + (d.sync_status.includes('Success') ? 'badge-success' : d.sync_status.includes('Failed') ? 'badge-danger' : 'badge-neutral');";
    html += "let ago = d.sync_ago_ms > 0 ? Math.floor(d.sync_ago_ms/1000) + ' seconds ago' : 'Never';";
    html += "document.getElementById('val-ago').innerText = ago;";
    html += "}).catch(e=>console.log('Error fetching data'));";
    html += "}";
    html += "updateData(); setInterval(updateData, 2000);";
    html += "</script></body></html>";
    
    WebServer* activeServer = customServer ? customServer : &server;
    activeServer->send(200, "text/html", html);
}

void ApiClient::_handlePowerData(WebServer* customServer) {
    WebServer* activeServer = customServer ? customServer : &server;
    activeServer->sendHeader("Access-Control-Allow-Origin", "*");
    activeServer->sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    activeServer->sendHeader("Access-Control-Allow-Headers", "Content-Type");
    
    DynamicJsonDocument doc(256);
    doc["raw_adc"] = analogRead(PIN_BATTERY_ADC);
    doc["voltage"] = battery.getVoltage();
    doc["percent"] = battery.getPercentage();
    doc["estimated_time"] = battery.getEstimatedTime(lastPowerMode);
    doc["power_mode"] = lastPowerMode;
    doc["sync_status"] = lastSyncStatus;
    doc["event_status"] = lastBatteryEventStatus;
    doc["sync_ago_ms"] = lastSyncTime > 0 ? (millis() - lastSyncTime) : 0;
    
    String response;
    serializeJson(doc, response);
    activeServer->send(200, "application/json", response);
}
