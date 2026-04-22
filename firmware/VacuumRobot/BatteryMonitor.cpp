#include "BatteryMonitor.h"
#include "config.h"

void BatteryMonitor::begin() {
    pinMode(PIN_BATTERY_ADC, INPUT);
}

float BatteryMonitor::getVoltage() {
    // Take 10 samples to average out the noise
    long sum = 0;
    for(int i = 0; i < 10; i++) {
        sum += analogRead(PIN_BATTERY_ADC);
        delay(2);
    }
    float raw = sum / 10.0;
    
    // ESP32 ADC is 12-bit (0-4095)
    // 3.3V ref
    float voltage = (raw / 4095.0) * 3.3 * _calibrationFactor;
    return voltage;
}

int BatteryMonitor::getPercentage() {
    float v = getVoltage();
    // 3S Li-ion: 9.0V (0%) -> 12.6V (100%)
    int pct = map((long)(v * 100), 900, 1260, 0, 100);
    return constrain(pct, 0, 100);
}

//R1 30kΩ, R2 10kΩ