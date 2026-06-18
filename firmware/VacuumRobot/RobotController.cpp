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
    // Update soft start ramping (non-blocking, must run every loop)
    vacuum.updateSoftStart();
    brush.updateSoftStart();
    
    // 1. Battery Reporting (adaptive interval: faster when working, slower when idle)
    unsigned long batteryInterval = (api.lastState == "working") 
        ? BATTERY_SEND_INTERVAL 
        : BATTERY_SEND_INTERVAL_IDLE;
    
    if (millis() - _lastBatteryCheck > batteryInterval) {
        _lastBatteryCheck = millis();
        int pct = battery.getPercentage();
        float volt = battery.getVoltage();
        
        Serial.print("[BATTERY] Voltage: ");
        Serial.print(volt, 1);
        Serial.print("V, Percentage: ");
        Serial.print(pct);
        Serial.print("% (interval: ");
        Serial.print(batteryInterval / 1000);
        Serial.println("s)");
        
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
    // LOW BATTERY AUTO STOP
    // =========================================

    if (battery.getPercentage() <= LOW_BATTERY_PERCENT) {

        api.lastState = "idle";

        Serial.println("[ROBOT] LOW BATTERY -> IDLE");

        stopAll();

        return;
    }

    // =========================================
    // MAIN STATE MACHINE
    // =========================================

    if (targetState == "working") {

        handleCleaning();
    }
    else {

        stopAll();
    }
}

// =====================================================
// CLEANING MODE
// =====================================================

void RobotController::handleCleaning() {

    brush.forward();
    vacuum.setPower(api.lastPowerValue);

    // =========================================
    // CLIFF SAFETY
    // =========================================

    if (sensors.isCliffDetected()) {

        Serial.println("[SAFETY] CLIFF DETECTED");

        wheels.moveBackward();
        delay(BACKWARD_DELAY);

        // Belok menjauhi sisi cliff yang terdeteksi
        if (sensors.isCliffLeft()) {

            wheels.turnRight();
            Serial.println("[SAFETY] Cliff kiri -> belok kanan");
        }
        else if (sensors.isCliffRight()) {

            wheels.turnLeft();
            Serial.println("[SAFETY] Cliff kanan -> belok kiri");
        }
        else {

            // Cliff depan -> belok kanan default
            wheels.turnRight();
            Serial.println("[SAFETY] Cliff depan -> belok kanan");
        }

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

        // Trigger escape mode
        if (millis() - _loopStartTime > LOOP_TIMEOUT) {

            Serial.println("[ANTI-LOOP] ESCAPE MODE");

            _escapeMode = true;
        }

        if (_escapeMode) {

            // Alternating kiri-kanan
            if (_escapeToggle) {

                wheels.turnLeft();
                Serial.println("[ANTI-LOOP] Escape -> KIRI");
            }
            else {

                wheels.turnRight();
                Serial.println("[ANTI-LOOP] Escape -> KANAN");
            }

            _escapeToggle  = !_escapeToggle;
            _escapeMode    = false;
            _loopStartTime = 0;

            delay(ESCAPE_DELAY);
        }
        else {

            // Normal turn: hindari sisi yang terblokir
            if (sensors.isLeftBlocked() && !sensors.isRightBlocked()) {

                wheels.turnRight();
                Serial.println("[NAVIGATION] Obs kiri -> belok kanan");
            }
            else if (sensors.isRightBlocked() && !sensors.isLeftBlocked()) {

                wheels.turnLeft();
                Serial.println("[NAVIGATION] Obs kanan -> belok kiri");
            }
            else {

                // Kedua sisi blokir atau kosong -> ikut wall following (kiri)
                wheels.turnLeft();
                Serial.println("[NAVIGATION] Default -> belok kiri");
            }

            delay(TURN_DELAY);
        }

        return;
    }

    // =========================================
    // WALL FOLLOWING (LEFT)
    // =========================================

    if (sensors.isLeftBlocked()) {

        // Dinding kiri terdeteksi -> tetap sejajar, sedikit geser kanan
        wheels.turnRight();
        delay(100);
    }
    else {

        // Tidak ada dinding kiri -> maju
        wheels.moveForward();
    }

    // Reset loop timer saat jalan normal
    _loopStartTime = 0;
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