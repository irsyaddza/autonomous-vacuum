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
    
    _speed = WHEEL_MOTOR_SPEED;
    
    Serial.println("[WHEEL] Drive Wheel Motors initialized:");
    Serial.print("  - Left FWD: IO");
    Serial.print(PIN_WHEEL_LEFT_FWD);
    Serial.print(", REV: IO");
    Serial.println(PIN_WHEEL_LEFT_REV);
    Serial.print("  - Right FWD: IO");
    Serial.print(PIN_WHEEL_RIGHT_FWD);
    Serial.print(", REV: IO");
    Serial.println(PIN_WHEEL_RIGHT_REV);
    Serial.print("  - Speed: ");
    Serial.println(_speed);
    
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
    _speed = constrain(pwm, 0, 255);
    Serial.print("[WHEEL] Speed set to: ");
    Serial.println(_speed);
}

void WheelMotor::moveForward() {
    // Kedua motor maju
    ledcWrite(PIN_WHEEL_LEFT_FWD, _speed);
    ledcWrite(PIN_WHEEL_LEFT_REV, 0);
    ledcWrite(PIN_WHEEL_RIGHT_FWD, _speed);
    ledcWrite(PIN_WHEEL_RIGHT_REV, 0);
    
    Serial.print("[WHEEL] Moving FORWARD @ PWM: ");
    Serial.println(_speed);
}

void WheelMotor::moveBackward() {
    // Kedua motor mundur
    ledcWrite(PIN_WHEEL_LEFT_FWD, 0);
    ledcWrite(PIN_WHEEL_LEFT_REV, _speed);
    ledcWrite(PIN_WHEEL_RIGHT_FWD, 0);
    ledcWrite(PIN_WHEEL_RIGHT_REV, _speed);
    
    Serial.print("[WHEEL] Moving BACKWARD @ PWM: ");
    Serial.println(_speed);
}

void WheelMotor::turnLeft() {
    // Roda kiri mundur, roda kanan maju (belok kiri di tempat)
    ledcWrite(PIN_WHEEL_LEFT_FWD, 0);
    ledcWrite(PIN_WHEEL_LEFT_REV, _speed);
    ledcWrite(PIN_WHEEL_RIGHT_FWD, _speed);
    ledcWrite(PIN_WHEEL_RIGHT_REV, 0);
    
    Serial.print("[WHEEL] Turning LEFT @ PWM: ");
    Serial.println(_speed);
}

void WheelMotor::turnRight() {
    // Roda kiri maju, roda kanan mundur (belok kanan di tempat)
    ledcWrite(PIN_WHEEL_LEFT_FWD, _speed);
    ledcWrite(PIN_WHEEL_LEFT_REV, 0);
    ledcWrite(PIN_WHEEL_RIGHT_FWD, 0);
    ledcWrite(PIN_WHEEL_RIGHT_REV, _speed);
    
    Serial.print("[WHEEL] Turning RIGHT @ PWM: ");
    Serial.println(_speed);
}

void WheelMotor::stop() {
    ledcWrite(PIN_WHEEL_LEFT_FWD, 0);
    ledcWrite(PIN_WHEEL_LEFT_REV, 0);
    ledcWrite(PIN_WHEEL_RIGHT_FWD, 0);
    ledcWrite(PIN_WHEEL_RIGHT_REV, 0);
    
    Serial.println("[WHEEL] All wheels STOPPED");
}
