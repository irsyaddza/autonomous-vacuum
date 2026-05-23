#include "BrushMotor.h"
#include "config.h"

// PWM Configuration for ESP32 LEDC
const int BRUSH_PWM_FREQ = 5000;      // 5kHz
const int BRUSH_PWM_RESOLUTION = 8;   // 8-bit (0-255)

void BrushMotor::begin() {
    pinMode(PIN_BRUSH_FWD, OUTPUT);
    pinMode(PIN_BRUSH_REV, OUTPUT);
    
    // Setup LEDC PWM for ESP32
    ledcAttach(PIN_BRUSH_FWD, BRUSH_PWM_FREQ, BRUSH_PWM_RESOLUTION);
    ledcAttach(PIN_BRUSH_REV, BRUSH_PWM_FREQ, BRUSH_PWM_RESOLUTION);
    
    Serial.println("[BRUSH] Motor Sapu initialized (with Soft Start):");
    Serial.print("  - Forward Pin: IO");
    Serial.println(PIN_BRUSH_FWD);
    Serial.print("  - Reverse Pin: IO");
    Serial.println(PIN_BRUSH_REV);
    
    stop();
}

void BrushMotor::setSpeed(int pwm) {
    _targetSpeed = constrain(pwm, 0, 255);
}

void BrushMotor::forward() {
    _isRunning = true;
    _isForward = true;
    _targetSpeed = (_targetSpeed > 0) ? _targetSpeed : BRUSH_SPEED;
    
    if (_currentSpeed == _targetSpeed && !_ramping) {
        return; // Already at target
    }
    
    // Start ramping
    _ramping = true;
    _lastRampTime = millis();
    
    Serial.print("[BRUSH] Soft Start: Forward ramp ");
    Serial.print(_currentSpeed);
    Serial.print(" → ");
    Serial.println(_targetSpeed);
}

void BrushMotor::reverse() {
    _isRunning = true;
    _isForward = false;
    _targetSpeed = (_targetSpeed > 0) ? _targetSpeed : BRUSH_SPEED;
    
    if (_currentSpeed == _targetSpeed && !_ramping) {
        return; // Already at target
    }
    
    // Start ramping
    _ramping = true;
    _lastRampTime = millis();
    
    Serial.print("[BRUSH] Soft Start: Reverse ramp ");
    Serial.print(_currentSpeed);
    Serial.print(" → ");
    Serial.println(_targetSpeed);
}

void BrushMotor::stop() {
    _isRunning = false;
    _ramping = false;
    _currentSpeed = 0;
    _targetSpeed = 0;
    
    // Immediate stop
    ledcWrite(PIN_BRUSH_FWD, 0);
    ledcWrite(PIN_BRUSH_REV, 0);
    Serial.println("[BRUSH] Motor Sapu STOPPED");
}

// Non-blocking soft start update - call every loop()
void BrushMotor::updateSoftStart() {
    if (!_ramping) return;
    
    unsigned long now = millis();
    if (now - _lastRampTime < SOFT_START_RAMP_INTERVAL) return;
    _lastRampTime = now;
    
    if (_currentSpeed < _targetSpeed) {
        // Ramping UP
        _currentSpeed += SOFT_START_RAMP_STEP;
        if (_currentSpeed >= _targetSpeed) {
            _currentSpeed = _targetSpeed;
            _ramping = false;
            Serial.print("[BRUSH] Soft Start: Ramp complete → PWM ");
            Serial.println(_currentSpeed);
        }
    } else if (_currentSpeed > _targetSpeed) {
        // Ramping DOWN
        _currentSpeed -= SOFT_START_RAMP_STEP;
        if (_currentSpeed <= _targetSpeed) {
            _currentSpeed = _targetSpeed;
            _ramping = false;
            Serial.print("[BRUSH] Soft Start: Ramp down complete → PWM ");
            Serial.println(_currentSpeed);
        }
    } else {
        _ramping = false;
    }
    
    _applySpeed();
}

bool BrushMotor::isRamping() {
    return _ramping;
}

// Internal: write current speed to hardware pins based on direction
void BrushMotor::_applySpeed() {
    if (_isForward) {
        ledcWrite(PIN_BRUSH_FWD, _currentSpeed);  // Forward with PWM
        ledcWrite(PIN_BRUSH_REV, 0);               // Reverse OFF
    } else {
        ledcWrite(PIN_BRUSH_FWD, 0);               // Forward OFF
        ledcWrite(PIN_BRUSH_REV, _currentSpeed);   // Reverse with PWM
    }
}
