#include "WheelMotor.h"
#include "config.h"

// PWM Configuration for ESP32 LEDC
const int WHEEL_PWM_FREQ = 5000;      // 5kHz
const int WHEEL_PWM_RESOLUTION = 8;   // 8-bit (0-255)

void WheelMotor::begin() {
    // Setup all wheel motor pins
    pinMode(PIN_WHEEL_LEFT_FWD, OUTPUT);
    pinMode(PIN_WHEEL_LEFT_REV, OUTPUT);
    pinMode(PIN_WHEEL_RIGHT_FWD, OUTPUT);
    pinMode(PIN_WHEEL_RIGHT_REV, OUTPUT);
    
    // Setup LEDC PWM for ESP32 - all 4 pins
    ledcAttach(PIN_WHEEL_LEFT_FWD, WHEEL_PWM_FREQ, WHEEL_PWM_RESOLUTION);
    ledcAttach(PIN_WHEEL_LEFT_REV, WHEEL_PWM_FREQ, WHEEL_PWM_RESOLUTION);
    ledcAttach(PIN_WHEEL_RIGHT_FWD, WHEEL_PWM_FREQ, WHEEL_PWM_RESOLUTION);
    ledcAttach(PIN_WHEEL_RIGHT_REV, WHEEL_PWM_FREQ, WHEEL_PWM_RESOLUTION);
    
    _leftSpeed = WHEEL_LEFT_SPEED;
    _rightSpeed = WHEEL_RIGHT_SPEED;
    
    Serial.println("[WHEEL] Drive Wheel Motors initialized:");
    Serial.print("  - Left FWD: IO");
    Serial.print(PIN_WHEEL_LEFT_FWD);
    Serial.print(", REV: IO");
    Serial.println(PIN_WHEEL_LEFT_REV);
    Serial.print("  - Right FWD: IO");
    Serial.print(PIN_WHEEL_RIGHT_FWD);
    Serial.print(", REV: IO");
    Serial.println(PIN_WHEEL_RIGHT_REV);
    Serial.print("  - Left Speed: ");
    Serial.print(_leftSpeed);
    Serial.print(", Right Speed: ");
    Serial.println(_rightSpeed);
    
    // === STARTUP BUZZER MELODY (Samsung Smart Lock style) ===
    Serial.println("[WHEEL] >>> Playing startup melody <<<");
    
    tone(PIN_BUZZER, 1319);  // E6
    delay(80);
    noTone(PIN_BUZZER);
    delay(30);
    
    tone(PIN_BUZZER, 1661);  // G#6
    delay(80);
    noTone(PIN_BUZZER);
    delay(30);
    
    tone(PIN_BUZZER, 1976);  // B6
    delay(80);
    noTone(PIN_BUZZER);
    delay(30);
    
    tone(PIN_BUZZER, 2637);  // E7 (final note - longer)
    delay(200);
    noTone(PIN_BUZZER);
    
    Serial.println("[WHEEL] >>> Startup melody complete <<<");
    stop();
}

void WheelMotor::setSpeed(int pwm) {
    int val = constrain(pwm, 0, 255);
    _leftSpeed = val;
    _rightSpeed = val;
    Serial.print("[WHEEL] Speed (both) set to: ");
    Serial.println(val);
}

void WheelMotor::setLeftSpeed(int pwm) {
    _leftSpeed = constrain(pwm, 0, 255);
    Serial.print("[WHEEL] Left speed set to: ");
    Serial.println(_leftSpeed);
}

void WheelMotor::setRightSpeed(int pwm) {
    _rightSpeed = constrain(pwm, 0, 255);
    Serial.print("[WHEEL] Right speed set to: ");
    Serial.println(_rightSpeed);
}

void WheelMotor::moveForward() {
    // Kedua motor maju (independent speed)
    ledcWrite(PIN_WHEEL_LEFT_FWD, _leftSpeed);
    ledcWrite(PIN_WHEEL_LEFT_REV, 0);
    ledcWrite(PIN_WHEEL_RIGHT_FWD, _rightSpeed);
    ledcWrite(PIN_WHEEL_RIGHT_REV, 0);
    
    Serial.print("[WHEEL] Moving FORWARD @ L:");
    Serial.print(_leftSpeed);
    Serial.print(" R:");
    Serial.println(_rightSpeed);
}

void WheelMotor::moveBackward() {
    // Kedua motor mundur (independent speed)
    ledcWrite(PIN_WHEEL_LEFT_FWD, 0);
    ledcWrite(PIN_WHEEL_LEFT_REV, _leftSpeed);
    ledcWrite(PIN_WHEEL_RIGHT_FWD, 0);
    ledcWrite(PIN_WHEEL_RIGHT_REV, _rightSpeed);
    
    Serial.print("[WHEEL] Moving BACKWARD @ L:");
    Serial.print(_leftSpeed);
    Serial.print(" R:");
    Serial.println(_rightSpeed);
}

void WheelMotor::turnLeft() {
    // Roda kiri mundur, roda kanan maju (belok kiri di tempat)
    ledcWrite(PIN_WHEEL_LEFT_FWD, 0);
    ledcWrite(PIN_WHEEL_LEFT_REV, _leftSpeed);
    ledcWrite(PIN_WHEEL_RIGHT_FWD, _rightSpeed);
    ledcWrite(PIN_WHEEL_RIGHT_REV, 0);
    
    Serial.print("[WHEEL] Turning LEFT @ L:");
    Serial.print(_leftSpeed);
    Serial.print(" R:");
    Serial.println(_rightSpeed);
}

void WheelMotor::turnRight() {
    // Roda kiri maju, roda kanan mundur (belok kanan di tempat)
    ledcWrite(PIN_WHEEL_LEFT_FWD, _leftSpeed);
    ledcWrite(PIN_WHEEL_LEFT_REV, 0);
    ledcWrite(PIN_WHEEL_RIGHT_FWD, 0);
    ledcWrite(PIN_WHEEL_RIGHT_REV, _rightSpeed);
    
    Serial.print("[WHEEL] Turning RIGHT @ L:");
    Serial.print(_leftSpeed);
    Serial.print(" R:");
    Serial.println(_rightSpeed);
}

void WheelMotor::stop() {
    ledcWrite(PIN_WHEEL_LEFT_FWD, 0);
    ledcWrite(PIN_WHEEL_LEFT_REV, 0);
    ledcWrite(PIN_WHEEL_RIGHT_FWD, 0);
    ledcWrite(PIN_WHEEL_RIGHT_REV, 0);
    
    Serial.println("[WHEEL] All wheels STOPPED");
}
