#include "VacuumMotor.h"
#include "config.h"

// PWM Configuration for ESP32 LEDC
const int PWM_FREQ = 5000;      // 5kHz - same as user's working code
const int PWM_RESOLUTION = 8;   // 8-bit (0-255)

void VacuumMotor::begin() {
    pinMode(PIN_VACUUM_PWM_1, OUTPUT);
    pinMode(PIN_VACUUM_PWM_2, OUTPUT);
    
    // Setup LEDC PWM for ESP32 (NEW API)
    // This replaces ledcSetup + ledcAttachPin with single ledcAttach
    ledcAttach(PIN_VACUUM_PWM_1, PWM_FREQ, PWM_RESOLUTION);
    ledcAttach(PIN_VACUUM_PWM_2, PWM_FREQ, PWM_RESOLUTION);
    
    Serial.println("[VACUUM] Motor Vakum initialized (with Soft Start):");
    Serial.print("  - Speed Pin: IO");
    Serial.println(PIN_VACUUM_PWM_1);
    Serial.print("  - Direction Pin: IO");
    Serial.println(PIN_VACUUM_PWM_2);
    Serial.print("  - Ramp Step: ");
    Serial.print(SOFT_START_RAMP_STEP);
    Serial.print(" PWM every ");
    Serial.print(SOFT_START_RAMP_INTERVAL);
    Serial.println("ms");
    
    stop();
}

// Set target power WITH soft start (non-blocking)
void VacuumMotor::setPower(int pwm) {
    int newTarget = constrain(pwm, 0, 255);
    
    if (newTarget == _targetPower && !_ramping) {
        return; // Already at target, skip
    }
    
    _targetPower = newTarget;
    
    if (_targetPower == 0) {
        // Stop immediately for safety (no ramp down)
        _applyPower(0);
        _currentPower = 0;
        _ramping = false;
        Serial.println("[VACUUM] Soft Start: Target=0 → Immediate stop");
        return;
    }
    
    // Start ramping
    _ramping = true;
    _lastRampTime = millis();
    
    Serial.print("[VACUUM] Soft Start: Ramping ");
    Serial.print(_currentPower);
    Serial.print(" → ");
    Serial.print(_targetPower);
    Serial.print(" (est. ");
    int steps = abs(_targetPower - _currentPower) / SOFT_START_RAMP_STEP;
    Serial.print(steps * SOFT_START_RAMP_INTERVAL);
    Serial.println("ms)");
}

// Set power IMMEDIATELY (bypass soft start)
// Used internally for safety stops
void VacuumMotor::setPowerDirect(int pwm) {
    _currentPower = constrain(pwm, 0, 255);
    _targetPower = _currentPower;
    _ramping = false;
    _applyPower(_currentPower);
}

// Non-blocking soft start update - call every loop()
void VacuumMotor::updateSoftStart() {
    if (!_ramping) return;
    
    unsigned long now = millis();
    if (now - _lastRampTime < SOFT_START_RAMP_INTERVAL) return;
    _lastRampTime = now;
    
    if (_currentPower < _targetPower) {
        // Ramping UP
        _currentPower += SOFT_START_RAMP_STEP;
        if (_currentPower >= _targetPower) {
            _currentPower = _targetPower;
            _ramping = false;
            Serial.print("[VACUUM] Soft Start: Ramp complete → PWM ");
            Serial.println(_currentPower);
        }
    } else if (_currentPower > _targetPower) {
        // Ramping DOWN (e.g., STRONG → ECO)
        _currentPower -= SOFT_START_RAMP_STEP;
        if (_currentPower <= _targetPower) {
            _currentPower = _targetPower;
            _ramping = false;
            Serial.print("[VACUUM] Soft Start: Ramp down complete → PWM ");
            Serial.println(_currentPower);
        }
    } else {
        _ramping = false;
    }
    
    _applyPower(_currentPower);
}

bool VacuumMotor::isRamping() {
    return _ramping;
}

void VacuumMotor::start() {
    setPower(VACUUM_POWER_NORMAL);
}

void VacuumMotor::stop() {
    // Safety: immediate stop, no ramp down
    setPowerDirect(0);
}

// Internal: write PWM value to hardware pins
void VacuumMotor::_applyPower(int pwm) {
    // *** CRITICAL FIX for L298N Motor Driver ***
    // For L298N: ONE pin gets PWM (speed), OTHER pin gets 0 (direction)
    // PIN_VACUUM_PWM_1 (IO26/IN3) = PWM speed
    // PIN_VACUUM_PWM_2 (IO32/IN4) = 0 (forward direction)
    ledcWrite(PIN_VACUUM_PWM_1, pwm);  // Speed control
    ledcWrite(PIN_VACUUM_PWM_2, 0);    // Direction = forward
}
