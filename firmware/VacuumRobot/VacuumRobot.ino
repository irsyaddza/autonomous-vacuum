#include <Arduino.h>

#include "config.h"

#include "BrushMotor.h"
#include "VacuumMotor.h"
#include "WheelMotor.h"

#include "SensorArray.h"
#include "BatteryMonitor.h"

#include "ApiClient.h"
#include "RobotController.h"

// =====================================================
// GLOBAL OBJECTS
// =====================================================

BrushMotor brush;
VacuumMotor vacuum;
WheelMotor wheels;

SensorArray sensors;
BatteryMonitor battery;

ApiClient api;
RobotController robot;

// =====================================================
// SETUP
// =====================================================

void setup() {

    Serial.begin(115200);

    Serial.println("\n=================================");
    Serial.println(" VACUUM ROBOT ESP32 STARTING ");
    Serial.println("=================================");

    // =========================================
    // HARDWARE INIT
    // =========================================

    brush.begin();
    vacuum.begin();
    wheels.begin();

    sensors.begin();
    battery.begin();

    // =========================================
    // WIFI & API
    // =========================================

    api.connectWiFi();

    api.startWebServer();

    api.registerDevice();

    // =========================================
    // ROBOT LOGIC
    // =========================================

    robot.begin();

    // =========================================
    // READY
    // =========================================

    Serial.println("\n=================================");
    Serial.println(" ROBOT READY ");
    Serial.println("=================================");

    Serial.print("IP Address : ");
    Serial.println(WiFi.localIP());

    Serial.print("Firmware   : ");
    Serial.println(FIRMWARE_VERSION);

    Serial.print("HTTP Port  : ");
    Serial.println(ESP32_HTTP_PORT);

    Serial.println("\nFeatures:");
    Serial.println("- Autonomous Cleaning");
    Serial.println("- Left Wall Following");
    Serial.println("- Obstacle Avoidance");
    Serial.println("- Cliff Detection");
    Serial.println("- Anti-loop Navigation");

    Serial.println("=================================\n");
}

// =====================================================
// LOOP
// =====================================================

void loop() {

    robot.update();

    api.update();

    delay(10);
}