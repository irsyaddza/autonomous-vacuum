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
    const float _calibrationFactor = 2.0; // Example: Equal resistors R1=R2 -> 2x multiplier
};

#endif
