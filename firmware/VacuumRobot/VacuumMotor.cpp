#include "VacuumMotor.h"
#include "config.h"

void VacuumMotor::begin() {
    pinMode(PIN_VACUUM_PWM, OUTPUT);
    stop();
}

void VacuumMotor::setPower(int pwm) {
    _currentPower = constrain(pwm, 0, 255);
    analogWrite(PIN_VACUUM_PWM, _currentPower);
}

void VacuumMotor::start() {
    setPower(VACUUM_POWER_NORMAL);
}

void VacuumMotor::stop() {
    setPower(0);
}
