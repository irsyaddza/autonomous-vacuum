#include "RobotController.h"

#include "config.h"
#include "BrushMotor.h"
#include "VacuumMotor.h"
#include "WheelMotor.h"
#include "SensorArray.h"
#include "BatteryMonitor.h"
#include "ApiClient.h"

// =====================================================
// EXTERNAL OBJECTS
// =====================================================

extern BrushMotor brush;
extern VacuumMotor vacuum;
extern WheelMotor wheels;
extern SensorArray sensors;
extern BatteryMonitor battery;
extern ApiClient api;

// =====================================================
// BEGIN
// =====================================================

void RobotController::begin() {

    Serial.println("[ROBOT] Controller initialized");

    stopAll();
}

// =====================================================
// MAIN UPDATE
// =====================================================

void RobotController::update() {

    // =========================================
    // Battery Reporting
    // =========================================

    if (millis() - _lastBatteryCheck > BATTERY_SEND_INTERVAL) {

        _lastBatteryCheck = millis();

        int pct = battery.getPercentage();
        float volt = battery.getVoltage();

        api.sendBattery(pct, volt);

        Serial.print("[BATTERY] ");
        Serial.print(pct);
        Serial.print("% | ");
        Serial.println(volt);
    }

    // =========================================
    // Read Sensor
    // =========================================

    sensors.readObstacles();
    sensors.readCliffs();

    // =========================================
    // API State
    // =========================================

    String targetState = api.lastState;

    // =========================================
    // State Change Log
    // =========================================

    if (targetState != _prevState) {

        Serial.print("[ROBOT] State: ");
        Serial.print(_prevState);
        Serial.print(" -> ");
        Serial.println(targetState);

        _prevState = targetState;
    }

    // =========================================
    // LOW BATTERY AUTO RETURN
    // =========================================

    if (battery.getPercentage() <= LOW_BATTERY_PERCENT) {

        api.lastState = "returning";

        Serial.println("[ROBOT] LOW BATTERY -> RETURN HOME");
    }

    // =========================================
    // MAIN STATE MACHINE
    // =========================================

    if (targetState == "working") {

        handleCleaning();
    }
    else if (targetState == "returning") {

        handleReturning();
    }
    else {

        stopAll();
    }
}

// =====================================================
// CLEANING MODE
// =====================================================

void RobotController::handleCleaning() {

    // Start brush & vacuum
    brush.forward();
    vacuum.setPower(api.lastPowerValue);

    // =========================================
    // CLIFF SAFETY
    // =========================================

    if (sensors.isCliffDetected()) {

        Serial.println("[SAFETY] CLIFF DETECTED");

        wheels.moveBackward();
        delay(BACKWARD_DELAY);

        wheels.turnRight();
        delay(TURN_DELAY);

        wheels.stop();

        return;
    }

    // =========================================
    // FRONT BLOCKED
    // =========================================

    if (sensors.isFrontBlocked()) {

        Serial.println("[NAVIGATION] FRONT BLOCKED");

        wheels.moveBackward();
        delay(BACKWARD_DELAY);

        // Anti-loop timer
        if (_loopStartTime == 0) {
            _loopStartTime = millis();
        }

        // Escape Mode
        if (millis() - _loopStartTime > LOOP_TIMEOUT) {

            Serial.println("[ANTI-LOOP] ESCAPE MODE");

            _escapeMode = true;
        }

        // Random escape
        if (_escapeMode) {

            if (random(0, 2) == 0) {

                wheels.turnLeft();
            }
            else {

                wheels.turnRight();
            }

            delay(ESCAPE_DELAY);

            _escapeMode = false;
            _loopStartTime = 0;
        }
        else {

            // Normal turn
            if (sensors.isLeftBlocked()) {

                wheels.turnRight();
            }
            else {

                wheels.turnLeft();
            }

            delay(TURN_DELAY);
        }

        return;
    }

    // =========================================
    // WALL FOLLOWING
    // =========================================

    if (sensors.isLeftBlocked()) {

        // Stay near wall
        wheels.turnRight();
        delay(100);
    }
    else {

        // Edge-to-center behavior
        wheels.moveForward();
    }
}

// =====================================================
// RETURN HOME MODE
// =====================================================

void RobotController::handleReturning() {

    Serial.println("[HOMING] RETURNING HOME");

    // Placeholder homing logic
    // Future:
    // - BLE RSSI Navigation
    // - IR Docking

    wheels.moveBackward();
    delay(300);

    wheels.turnRight();
    delay(500);

    wheels.moveForward();
}

// =====================================================
// SAFETY
// =====================================================

void RobotController::checkSafety() {

    if (sensors.isCliffDetected()) {

        stopAll();

        Serial.println("[SAFETY] Emergency stop");
    }
}

// =====================================================
// STOP ALL
// =====================================================

void RobotController::stopAll() {

    wheels.stop();
    brush.stop();
    vacuum.stop();

    Serial.println("[ROBOT] STOP ALL");
}