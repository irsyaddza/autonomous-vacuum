#include "MotorDriver.h"
#include "config.h"

void MotorDriver::begin() {
    pinMode(PIN_MOTOR_DIR_1, OUTPUT);
    pinMode(PIN_MOTOR_DIR_2, OUTPUT);
    pinMode(PIN_MOTOR_PWM, OUTPUT);
    stop();
}

void MotorDriver::setSpeed(int speed) {
    _currentSpeed = constrain(speed, 0, 255);
    analogWrite(PIN_MOTOR_PWM, _currentSpeed);
}

void MotorDriver::stop() {
    digitalWrite(PIN_MOTOR_DIR_1, LOW);
    digitalWrite(PIN_MOTOR_DIR_2, LOW);
    analogWrite(PIN_MOTOR_PWM, 0);
}

void MotorDriver::moveForward() {
    // Assuming Motor Config. Swap HIGH/LOW if reversed.
    digitalWrite(PIN_MOTOR_DIR_1, HIGH);
    digitalWrite(PIN_MOTOR_DIR_2, LOW);
    setSpeed(SPEED_CLEAN);
}

void MotorDriver::moveBackward() {
    digitalWrite(PIN_MOTOR_DIR_1, LOW);
    digitalWrite(PIN_MOTOR_DIR_2, HIGH);
    setSpeed(SPEED_CLEAN);
}

// "Bump and Go" style turning: Reverse then Turn logic
// Since we have a single drive motor (assumed from user description of 'Front Geared Motor')
// If it's effectively a tricycle with rear static wheels, "turning" might rely on 
// differential slip or a specific mechanism. 
// For now, I'll implement standard differential logic placeholders.

void MotorDriver::turnLeft() {
    digitalWrite(PIN_MOTOR_DIR_1, LOW); // Stop/Reverse one side logic if 2 motors
    digitalWrite(PIN_MOTOR_DIR_2, HIGH);
    setSpeed(SPEED_TURN);
}

void MotorDriver::turnRight() {
    digitalWrite(PIN_MOTOR_DIR_1, HIGH);
    digitalWrite(PIN_MOTOR_DIR_2, LOW);
    setSpeed(SPEED_TURN);
}
