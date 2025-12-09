#include "VacuumMotor.h"
#include "config.h"

void VacuumMotor::begin() {
    pinMode(PIN_VACUUM_PWM_1, OUTPUT);
    pinMode(PIN_VACUUM_PWM_2, OUTPUT);
    stop();
}

void VacuumMotor::setPower(int pwm) {
    _currentPower = constrain(pwm, 0, 255);
    // Control both vacuum motors simultaneously
    // PWM value diambil dari website (eco: 150, normal: 200, strong: 255)
    analogWrite(PIN_VACUUM_PWM_1, _currentPower);
    analogWrite(PIN_VACUUM_PWM_2, _currentPower);
}

void VacuumMotor::start() {
    setPower(VACUUM_POWER_NORMAL);
}

void VacuumMotor::stop() {
    setPower(0);
}
