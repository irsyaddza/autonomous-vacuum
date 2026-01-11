#include "ApiClient.h"
#include "config.h"
#include <ArduinoJson.h> // Make sure you install ArduinoJson library

#include <WiFiManager.h> // PLEASE INSTALL: "WiFiManager" by tzapu

void ApiClient::connectWiFi() {
    // Initialize buzzer first
    initBuzzer();
    
    Serial.println("=============================");
    Serial.println("Starting WiFi Manager...");
    
    WiFiManager wm;
    
    // UI Customization
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

        /* Container Limit for WiFiManager default layout */
        div, form { width: 100%; }

        /* Main Card */
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

        /* Inputs */
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

        /* Buttons */
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

        /* Connect Button (Submit) */
        button[type='submit'] {
            background: linear-gradient(90deg, var(--primary), var(--secondary));
            color: white;
            box-shadow: 0 4px 15px rgba(78, 115, 223, 0.4);
        }

        button[type='submit']:hover {
            transform: translateY(-2px);
            box-shadow: 0 8px 25px rgba(78, 115, 223, 0.6);
        }

        /* Scan/Other Buttons */
        button:not([type='submit']) {
            background: rgba(255, 255, 255, 0.1);
            color: #fff;
            border: 1px solid var(--border);
        }

        button:not([type='submit']):hover {
            background: rgba(255, 255, 255, 0.2);
        }

        /* WiFi List Override */
        div.c { text-align: left; }
        
        div, a { color: #b0c4de; text-decoration: none; transition: 0.3s; }
        a:hover { color: #fff; text-shadow: 0 0 10px white; }

        /* Loader Animation Override (if scanning) */
        .q { float: right; }
        
        /* Custom Footer */
        .footer {
            margin-top: 30px;
            font-size: 0.8rem;
            color: rgba(255,255,255,0.3);
        }
    </style>
    <script>
        // Simple JS embellishments
        document.addEventListener('DOMContentLoaded', function() {
            // Add icon to title
            const h1 = document.querySelector('h1');
            if(h1) {
                h1.innerHTML = '🤖<br>' + h1.innerHTML;
                
                // Add subtext if not present
                if(!document.querySelector('h3')) {
                    const h3 = document.createElement('h3');
                    h3.innerText = 'Vacuum Robot Setup';
                    h1.after(h3);
                }
            }
            
            // Add custom footer
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
    
    // reset settings - wipe credentials for testing
    // wm.resetSettings(); 

    // Custom parameter for API URL (Optional, advanced)
    // WiFiManagerParameter custom_api_url("server", "API Base URL", API_BASE_URL, 60);
    // wm.addParameter(&custom_api_url);

    // Buka AP (Access Point) jika tidak connect
    // Nama dan password diambil dari config.h
    Serial.print("Opening AP: ");
    Serial.println(WIFI_AP_NAME);
    bool res = wm.autoConnect(WIFI_AP_NAME, WIFI_AP_PASSWORD); 

    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
        Serial.println(">>> WIFI CONNECTED SUCCESS! <<<");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
        
        // Serial.print("API URL: ");
        // Serial.println(custom_api_url.getValue());
        Serial.println("=============================");
    }
}

void ApiClient::update() {
    if (WiFi.status() != WL_CONNECTED) return;

    unsigned long now = millis();

    // 0. Check Serial for "reset_wifi" command
    if (Serial.available()) {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim(); // Remove whitespace
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

    // 0.5 Check Physical Button
    checkResetButton();

    // 1. Poll Status
    if (now - _lastPollTime > API_POLL_INTERVAL) {
        _lastPollTime = now;
        getStatus(); 
    }
}

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

void ApiClient::sendBattery(int percent, float voltage) {
    if (WiFi.status() != WL_CONNECTED) return;
    
    HTTPClient http;
    String url = String(API_BASE_URL) + "/battery";
    
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    
    DynamicJsonDocument doc(200);
    doc["battery_percent"] = percent;
    doc["battery_voltage"] = voltage;
    doc["estimated_time"] = String(percent / 10.0) + "h"; // Simple mock estimate
    
    String json;
    serializeJson(doc, json);
    
    int httpCode = http.POST(json);
    http.end();
}

void ApiClient::_parseStatusResponse(String json) {
    Serial.println("========== API RESPONSE DEBUG ==========");
    Serial.print("Raw JSON: ");
    Serial.println(json);
    
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
        String newDirection = doc["data"]["direction"] | "forward";  // Default to forward if not present
        
        Serial.println("--- PARSED VALUES ---");
        Serial.print("State: ");
        Serial.println(newState);
        Serial.print("Power Mode: ");
        Serial.println(newMode);
        Serial.print("Power Value: ");
        Serial.println(newVal);
        Serial.print("Direction: ");
        Serial.println(newDirection);
        Serial.println("---");
        
        // Log changes
        if (newState != lastState) {
            Serial.print(">>> COMMAND RECEIVED: State changed from ");
            Serial.print(lastState);
            Serial.print(" to ");
            Serial.println(newState);
        }

        if (newMode != lastPowerMode) {
             Serial.print(">>> COMMAND RECEIVED: Power Mode changed from ");
             Serial.print(lastPowerMode);
             Serial.print(" to ");
             Serial.println(newMode);
        }
        
        if (newVal != lastPowerValue) {
            Serial.print(">>> POWER VALUE UPDATED: ");
            Serial.print(lastPowerValue);
            Serial.print(" -> ");
            Serial.println(newVal);
        }
        
        if (newDirection != lastDirection) {
            Serial.print(">>> DIRECTION CHANGED: ");
            Serial.print(lastDirection);
            Serial.print(" -> ");
            Serial.println(newDirection);
        }
        
        // UPDATE VARIABLES
        lastState = newState;
        lastPowerMode = newMode;
        lastPowerValue = newVal;
        lastDirection = newDirection;
        
        Serial.print("CURRENT lastPowerValue = ");
        Serial.println(lastPowerValue);
        Serial.print("CURRENT lastDirection = ");
        Serial.println(lastDirection);
        Serial.println("========================================");
    } else {
        Serial.println("ERROR: Response success = false");
        Serial.println("========================================");
    }
}

void ApiClient::checkResetButton() {
    // Logic: Tekan tahan 5 detik untuk reset
    if (digitalRead(PIN_WIFI_RESET) == LOW) { // Button Pressed (Active Low)
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
            
            // Play buzzer beep to indicate WiFi reset success
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
    digitalWrite(PIN_BUZZER, LOW); // Ensure buzzer is off
    Serial.println("Buzzer initialized on GPIO 27");
}

void ApiClient::playWiFiResetBeep() {
    Serial.println("Playing WiFi reset beep...");
    
    // Pattern: 3 short beeps
    for (int i = 0; i < 3; i++) {
        digitalWrite(PIN_BUZZER, HIGH);
        delay(150);
        digitalWrite(PIN_BUZZER, LOW);
        delay(100);
    }
    
    // 1 long beep
    delay(200);
    digitalWrite(PIN_BUZZER, HIGH);
    delay(500);
    digitalWrite(PIN_BUZZER, LOW);
    
    Serial.println("Beep complete!");
}