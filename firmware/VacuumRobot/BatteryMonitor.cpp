#include "BatteryMonitor.h"
#include "config.h"

void BatteryMonitor::begin() {
    pinMode(PIN_BATTERY_ADC, INPUT);
}

float BatteryMonitor::getVoltage() {
    int raw = analogRead(PIN_BATTERY_ADC);
    // ESP32 ADC is 12-bit (0-4095)
    // 3.3V ref
    float voltage = (raw / 4095.0) * 3.3 * _calibrationFactor;
    return voltage;
}

int BatteryMonitor::getPercentage() {
    float v = getVoltage();
    // 2S Li-ion: 6.0V (0%) -> 8.4V (100%)
    int pct = map((long)(v * 100), 600, 840, 0, 100);
    return constrain(pct, 0, 100);
}
