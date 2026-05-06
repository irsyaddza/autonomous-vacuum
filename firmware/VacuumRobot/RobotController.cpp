#include "RobotController.h"
#include "config.h"
#include "BrushMotor.h"
#include "VacuumMotor.h"
#include "WheelMotor.h"
#include "SensorArray.h"
#include "BatteryMonitor.h"
#include "ApiClient.h"

// External objects
extern BrushMotor brush;
extern VacuumMotor vacuum;
extern WheelMotor wheels;
extern SensorArray sensors;
extern BatteryMonitor battery;
extern ApiClient api;

void RobotController::begin() {
    _cleaner.begin();
    Serial.println("[ROBOT] Controller initialized - Manual + Autonomous modes");
    stopAll();
}

void RobotController::update() {
    // 1. Battery Reporting
    if (millis() - _lastBatteryCheck > BATTERY_SEND_INTERVAL) {
        _lastBatteryCheck = millis();
        int pct = battery.getPercentage();
        float volt = battery.getVoltage();
        api.sendBattery(pct, volt);
    }

    // 2. Read state from API
    String targetState = api.lastState;
    int targetPower = api.lastPowerValue;
    String direction = api.lastDirection;
    
    // ===== BATTERY PROTECTION =====
    // Only check when robot is actively working
    if (targetState == "working") {
        int pct = battery.getPercentage();
        float volt = battery.getVoltage();
        
        // CRITICAL: Auto-stop at 0% battery
        if (battery.isCritical() && !_autoStopped) {
            _autoStopped = true;
            
            Serial.println("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
            Serial.println("[ROBOT] BATTERY CRITICAL (0%) - AUTO STOPPING!");
            Serial.print("[ROBOT] Voltage: ");
            Serial.print(volt);
            Serial.print("V, Percent: ");
            Serial.print(pct);
            Serial.println("%");
            Serial.println("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
            
            // Stop everything
            if (_wasAutonomous) {
                _cleaner.stop();
                _wasAutonomous = false;
            }
            stopAll();
            
            // Notify server
            api.sendAutoStop(pct, volt);
            
            return;  // Skip all motor control
        }
        
        // WARNING: Low battery at 15%
        if (battery.isLowBattery() && !_lowBatteryWarned) {
            _lowBatteryWarned = true;
            
            Serial.println("==========================================");
            Serial.print("[ROBOT] LOW BATTERY WARNING! ");
            Serial.print(pct);
            Serial.print("% (");
            Serial.print(volt);
            Serial.println("V)");
            Serial.println("==========================================");
            
            // Send warning event to server
            api.sendBatteryEvent("low_battery_warning", pct, volt);
        }
    }
    
    // Reset battery flags when not working (charged/recovered)
    if (targetState != "working") {
        if (_autoStopped && battery.getPercentage() > BATTERY_BLOCK_START_PCT) {
            _autoStopped = false;
            _lowBatteryWarned = false;
            Serial.println("[ROBOT] Battery recovered - protection flags reset");
        }
    }
    // ===== END BATTERY PROTECTION =====
    
    // Log state changes
    if (targetState != _prevState) {
        Serial.print("[ROBOT] State changed: '");
        Serial.print(_prevState);
        Serial.print("' -> '");
        Serial.print(targetState);
        Serial.println("'");
        _prevState = targetState;
    }
    
    // Log power changes
    if (targetPower != _prevPowerValue) {
        Serial.print("[ROBOT] Power changed: ");
        Serial.print(_prevPowerValue);
        Serial.print(" -> ");
        Serial.println(targetPower);
        _prevPowerValue = targetPower;
    }

    // 3. MAIN LOGIC
    if (targetState == "working") {
        
        // Set vacuum power dari website
        vacuum.setPower(targetPower);
        
        // Start brush motor
        brush.forward();
        
        // === MODE SELECTION ===
        if (direction == "autonomous") {
            // ==============================
            // MODE AUTONOMOUS (Random Bounce + Spiral)
            // ==============================
            if (!_wasAutonomous) {
                Serial.println("==================");
                Serial.println("[ROBOT] MODE = AUTONOMOUS CLEANING");
                Serial.print("[ROBOT] Vacuum Power = ");
                Serial.println(targetPower);
                Serial.println("==================");
                _cleaner.start();
                _wasAutonomous = true;
            }
            
            // Update cleaning algorithm (handles sensors + wheel control)
            _cleaner.update();
            
        } else {
            // ==============================
            // MODE MANUAL (dikontrol dari website)
            // ==============================
            if (_wasAutonomous) {
                // Baru pindah dari autonomous ke manual
                _cleaner.stop();
                _wasAutonomous = false;
                Serial.println("[ROBOT] Switched from AUTONOMOUS to MANUAL");
            }
            
            // === SAFETY: Cliff check di mode manual juga ===
            sensors.readCliffs();
            if (sensors.isCliffDetected()) {
                Serial.println("!!! [ROBOT] CLIFF DETECTED IN MANUAL MODE - STOPPING !!!");
                wheels.stop();
                return;  // Jangan gerak sampai cliff hilang
            }
            
            // Manual direction control
            static bool wasWorking = false;
            if (!wasWorking) {
                Serial.println("==================");
                Serial.println("[ROBOT] MODE = MANUAL CONTROL");
                Serial.print("[ROBOT] Direction = '");
                Serial.print(direction);
                Serial.println("'");
                Serial.println("==================");
                wasWorking = true;
            }
            
            if (direction == "backward") {
                wheels.moveBackward();
            } else if (direction == "left") {
                wheels.turnLeft();
            } else if (direction == "right") {
                wheels.turnRight();
            } else if (direction == "stop") {
                wheels.stop();
            } else {
                // Default: forward
                wheels.moveForward();
            }
        }
        
    } else {
        // ANY other state -> STOP everything
        static bool loggedStop = false;
        static String lastLoggedState = "";
        if (!loggedStop || lastLoggedState != targetState) {
            Serial.print("[ROBOT] STATE=");
            Serial.print(targetState);
            Serial.println(" -> ALL MOTORS STOP");
            loggedStop = true;
            lastLoggedState = targetState;
        }
        
        // Stop autonomous if it was running
        if (_wasAutonomous) {
            _cleaner.stop();
            _wasAutonomous = false;
        }
        
        stopAll();
    }
}

void RobotController::stopAll() {
    wheels.stop();
    brush.stop();
    vacuum.stop();
}

void RobotController::handleCleaning() {
    // Handled by CleaningAlgorithm
}

void RobotController::handleReturning() {
    // Not used - no position tracking available
}

void RobotController::checkSafety() {
    // Cliff safety is handled inline in update() and by CleaningAlgorithm
}
