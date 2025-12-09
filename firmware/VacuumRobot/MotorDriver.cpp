#include "MotorDriver.h"
#include "config.h"

void MotorDriver::begin() {
    pinMode(PIN_MOTOR_DIR_1, OUTPUT);
    pinMode(PIN_MOTOR_DIR_2, OUTPUT);
    stop();
}

void MotorDriver::stop() {
    digitalWrite(PIN_MOTOR_DIR_1, LOW);
    digitalWrite(PIN_MOTOR_DIR_2, LOW);
}

void MotorDriver::moveForward() {
    digitalWrite(PIN_MOTOR_DIR_1, HIGH);
    digitalWrite(PIN_MOTOR_DIR_2, LOW);
}

void MotorDriver::moveBackward() {
    digitalWrite(PIN_MOTOR_DIR_1, LOW);
    digitalWrite(PIN_MOTOR_DIR_2, HIGH);
}

void MotorDriver::turnLeft() {
    // Belok kiri = mundur (untuk robot dengan satu motor)
    digitalWrite(PIN_MOTOR_DIR_1, LOW);
    digitalWrite(PIN_MOTOR_DIR_2, HIGH);
}

void MotorDriver::turnRight() {
    // Belok kanan = maju (untuk robot dengan satu motor)
    digitalWrite(PIN_MOTOR_DIR_1, HIGH);
    digitalWrite(PIN_MOTOR_DIR_2, LOW);
}
