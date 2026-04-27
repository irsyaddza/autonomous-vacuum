#ifndef BATTERY_MONITOR_H
#define BATTERY_MONITOR_H

#include <Arduino.h>

class BatteryMonitor {
public:
    void begin();
    float getVoltage();
    int getPercentage();
    
private:
    // Calibration factor for Voltage Divider
    // Vout = Vin * (R2 / (R1 + R2))
    // ADC reads Vout. Vin = ADC * (3.3 / 4095) * ((R1+R2)/R2)
    // Using R1=30kΩ, R2=10kΩ: ideal = (30k + 10k) / 10k = 4.0
    // Calibrated with multimeter: actual 12.2V, ADC VP pin 3.054V
    // Actual Ratio: 12.2 / 3.054 = 3.9947
    // 3S max 12.6V → Vout = 12.6 * 10/40 = 3.15V (safe for ESP32 ADC)
    const float _calibrationFactor = 3.995;
};

#endif
