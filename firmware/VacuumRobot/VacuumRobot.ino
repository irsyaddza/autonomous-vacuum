#include <Arduino.h>
#include "config.h"
#include "MotorDriver.h"
#include "VacuumMotor.h"
#include "SensorArray.h"
#include "BatteryMonitor.h"
#include "ApiClient.h"
#include "RobotController.h"

// Global Objects
MotorDriver motor;
VacuumMotor vacuum;
SensorArray sensors;
BatteryMonitor battery;
ApiClient api;
RobotController robot;

void setup() {
  Serial.begin(115200);
  Serial.println("=== Vacuum Robot ESP32 Starting ===");

  // Initialize Hardware
  motor.begin();
  vacuum.begin();
  sensors.begin();
  battery.begin();

  // Initialize Network
  api.connectWiFi();

  // Initialize Logic
  robot.begin();
}

void loop() {
  // Update all subsystems
  robot.update();
  api.update(); // Handle network polling
  
  delay(10); // Small delay for stability
}
