#include <Arduino.h>
#include "config.h"
#include "BrushMotor.h"
#include "VacuumMotor.h"
#include "WheelMotor.h"
#include "SensorArray.h"
#include "BatteryMonitor.h"
#include "ApiClient.h"
#include "RobotController.h"

// Global Objects
BrushMotor brush;
VacuumMotor vacuum;
WheelMotor wheels;
SensorArray sensors;
BatteryMonitor battery;
ApiClient api;
RobotController robot;

void setup() {
  Serial.begin(115200);
  Serial.println("=== Vacuum Robot ESP32 Starting ===");

  // Initialize Hardware
  brush.begin();     // Motor Sapu (OUT1 & OUT2)
  vacuum.begin();    // Motor Vakum (OUT3 & OUT4)
  wheels.begin();    // Motor Roda (L298N #2)
  sensors.begin();
  battery.begin();

  // Initialize Network
  api.connectWiFi();

  // Start Direct HTTP Server (receives commands from browser)
  api.startWebServer();

  // Register device IP with Laravel server
  api.registerDevice();

  // Initialize Logic
  robot.begin();
  
  Serial.println("\n=== ROBOT READY (Direct HTTP Mode) ===");
  Serial.println("Motor Configuration:");
  Serial.println("  - Motor Driver 1: Brush (OUT1/2) + Vacuum (OUT3/4)");
  Serial.println("  - Motor Driver 2: Wheel Motors (LEFT + RIGHT)");
  Serial.print("Direct HTTP Server: Port ");
  Serial.println(ESP32_HTTP_PORT);
  Serial.print("Device IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("Firmware: v");
  Serial.println(FIRMWARE_VERSION);
  Serial.println("Commands: POST /command, GET /status");
  Serial.println("Fallback polling: every " + String(API_POLL_INTERVAL / 1000) + " seconds");
  Serial.println("==========================================\n");
}

unsigned long lastBatterySend = 0;

void loop() {
  // Update all subsystems
  robot.update();
  api.update(); // Handle WebServer + fallback polling
  
  // Send battery data periodically
  if (millis() - lastBatterySend >= BATTERY_SEND_INTERVAL || lastBatterySend == 0) {
    lastBatterySend = millis();
    if (lastBatterySend == 0) lastBatterySend = 1; // Prevent re-triggering immediately
    
    float voltage = battery.getVoltage();
    int percent = battery.getPercentage();
    
    Serial.print("[BATTERY] Voltage: ");
    Serial.print(voltage);
    Serial.print("V, Percentage: ");
    Serial.print(percent);
    Serial.println("%");
    
    api.sendBattery(percent, voltage);
  }
  
  delay(10); // Small delay for stability
}
