#include "WheelMotor.h"
#include "config.h"

// PWM CONFIG
const int WHEEL_PWM_FREQ = 5000;
const int WHEEL_PWM_RESOLUTION = 8;

void WheelMotor::begin() {

    // Setup pin
    pinMode(PIN_WHEEL_LEFT_FWD, OUTPUT);
    pinMode(PIN_WHEEL_LEFT_REV, OUTPUT);

    pinMode(PIN_WHEEL_RIGHT_FWD, OUTPUT);
    pinMode(PIN_WHEEL_RIGHT_REV, OUTPUT);

    // Attach PWM
    ledcAttach(PIN_WHEEL_LEFT_FWD, WHEEL_PWM_FREQ, WHEEL_PWM_RESOLUTION);
    ledcAttach(PIN_WHEEL_LEFT_REV, WHEEL_PWM_FREQ, WHEEL_PWM_RESOLUTION);

    ledcAttach(PIN_WHEEL_RIGHT_FWD, WHEEL_PWM_FREQ, WHEEL_PWM_RESOLUTION);
    ledcAttach(PIN_WHEEL_RIGHT_REV, WHEEL_PWM_FREQ, WHEEL_PWM_RESOLUTION);

    _speed = WHEEL_MOTOR_SPEED;

    Serial.println("[WHEEL] Wheel motors initialized");

    stop();
}

void WheelMotor::setSpeed(int pwm) {

    _speed = constrain(pwm, 0, 255);

    Serial.print("[WHEEL] Speed = ");
    Serial.println(_speed);
}

// =====================================================
// FORWARD
// =====================================================

void WheelMotor::moveForward() {

    ledcWrite(PIN_WHEEL_LEFT_FWD, _speed);
    ledcWrite(PIN_WHEEL_LEFT_REV, 0);

    // sedikit dikurangi supaya lurus
    ledcWrite(PIN_WHEEL_RIGHT_FWD, _speed - 10);
    ledcWrite(PIN_WHEEL_RIGHT_REV, 0);

    Serial.println("[WHEEL] FORWARD");
}

// =====================================================
// BACKWARD
// =====================================================

void WheelMotor::moveBackward() {

    ledcWrite(PIN_WHEEL_LEFT_FWD, 0);
    ledcWrite(PIN_WHEEL_LEFT_REV, _speed);

    ledcWrite(PIN_WHEEL_RIGHT_FWD, 0);
    ledcWrite(PIN_WHEEL_RIGHT_REV, _speed - 10);

    Serial.println("[WHEEL] BACKWARD");
}

// =====================================================
// TURN LEFT
// =====================================================

void WheelMotor::turnLeft() {

    ledcWrite(PIN_WHEEL_LEFT_FWD, 0);
    ledcWrite(PIN_WHEEL_LEFT_REV, _speed);

    ledcWrite(PIN_WHEEL_RIGHT_FWD, _speed);
    ledcWrite(PIN_WHEEL_RIGHT_REV, 0);

    Serial.println("[WHEEL] TURN LEFT");
}

// =====================================================
// TURN RIGHT
// =====================================================

void WheelMotor::turnRight() {

    ledcWrite(PIN_WHEEL_LEFT_FWD, _speed);
    ledcWrite(PIN_WHEEL_LEFT_REV, 0);

    ledcWrite(PIN_WHEEL_RIGHT_FWD, 0);
    ledcWrite(PIN_WHEEL_RIGHT_REV, _speed);

    Serial.println("[WHEEL] TURN RIGHT");
}

// =====================================================
// STOP
// =====================================================

void WheelMotor::stop() {

    ledcWrite(PIN_WHEEL_LEFT_FWD, 0);
    ledcWrite(PIN_WHEEL_LEFT_REV, 0);

    ledcWrite(PIN_WHEEL_RIGHT_FWD, 0);
    ledcWrite(PIN_WHEEL_RIGHT_REV, 0);

    Serial.println("[WHEEL] STOP");
}