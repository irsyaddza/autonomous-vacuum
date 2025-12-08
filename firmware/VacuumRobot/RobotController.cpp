#include "RobotController.h"
#include "config.h"
#include "MotorDriver.h"
#include "VacuumMotor.h"
#include "SensorArray.h"
#include "BatteryMonitor.h"
#include "ApiClient.h"

// External objects
extern MotorDriver motor;
extern VacuumMotor vacuum;
extern SensorArray sensors;
extern BatteryMonitor battery;
extern ApiClient api;

void RobotController::begin() {
    stopAll();
}

void RobotController::update() {
    // 1. Safety Check (High Priority)
    checkSafety();
    if (sensors.isCliffDetected()) {
        stopAll();
        // Maybe reverse a bit?
        motor.moveBackward();
        delay(500);
        motor.stop();
        return; 
    }

    // 2. Battery Reporting
    if (millis() - _lastBatteryCheck > BATTERY_SEND_INTERVAL) {
        _lastBatteryCheck = millis();
        int pct = battery.getPercentage();
        float volt = battery.getVoltage();
        api.sendBattery(pct, volt);
    }

    // 3. State Machine
    String targetState = api.lastState; // "standby", "working", "returning"

    if (targetState == "working") {
        handleCleaning();
    } else if (targetState == "returning") {
        handleReturning();
    } else {
        stopAll();
    }
}

void RobotController::stopAll() {
    motor.stop();
    vacuum.stop();
}

void RobotController::handleCleaning() {
    // Set Power Mode
    vacuum.setPower(api.lastPowerValue);

    // Obstacle Avoidance
    if (sensors.readObstacles()) {
        // Simple Bounce Logic
        motor.stop();
        delay(200);
        motor.moveBackward();
        delay(500);
        
        // Random Turn
        if (random(0, 2) == 0) motor.turnLeft();
        else motor.turnRight();
        
        delay(800);
    } else {
        motor.moveForward();
    }
}

void RobotController::handleReturning() {
    // Placeholder for BLE Homing
    // For now, just stop
    stopAll();
}

void RobotController::checkSafety() {
    sensors.readCliffs();
}
