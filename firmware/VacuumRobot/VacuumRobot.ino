#include <Arduino.h>
#include "config.h"
#include "BrushMotor.h"
#include "VacuumMotor.h"
#include "WheelMotor.h"
#include "SensorArray.h"
#include "BatteryMonitor.h"
#include "ApiClient.h"
#include "RobotController.h"
#include "TimingSettings.h"

// Global Objects
BrushMotor brush;
VacuumMotor vacuum;
WheelMotor wheels;
SensorArray sensors;
BatteryMonitor battery;
ApiClient api;
RobotController robot;
TimingSettings timing;

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

  // Load timing settings from NVS
  timing.load();

  // Apply NVS wheel speeds to motors (overrides config.h defaults)
  wheels.setLeftSpeed(timing.leftWheelSpeed);
  wheels.setRightSpeed(timing.rightWheelSpeed);

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
  Serial.println("Battery report: every " + String(BATTERY_SEND_INTERVAL / 1000) + "s (active), " + String(BATTERY_SEND_INTERVAL_IDLE / 1000) + "s (idle)");
  Serial.println("==========================================\n");
}

void loop() {
  // Update all subsystems
  // RobotController handles: motor control, battery reporting, battery protection
  // ApiClient handles: WebServer (incoming HTTP), device registration, reset button
  robot.update();
  api.update();
  
  // Adaptive delay: save power when idle, responsive when active
  if (api.lastState == "working") {
    delay(ACTIVE_LOOP_DELAY);
  } else {
    delay(IDLE_LOOP_DELAY);
  }
}
