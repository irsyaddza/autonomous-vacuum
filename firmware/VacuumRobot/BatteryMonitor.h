#ifndef BATTERY_MONITOR_H
#define BATTERY_MONITOR_H

#include <Arduino.h>

// ===== BATTERY CONFIGURATION =====
// 3S Li-ion: 9.0V (empty) → 12.6V (full)
#define BATTERY_VOLTAGE_MAX     12.6    // Tegangan penuh (3S Li-ion fully charged)
#define BATTERY_VOLTAGE_MIN     9.0     // Tegangan minimum (cutoff)
#define BATTERY_SAMPLE_COUNT    20      // Jumlah sampel untuk moving average
#define BATTERY_FULL_RUNTIME_MIN 120    // Estimasi runtime penuh dalam menit (diisi setelah discharge test)

class BatteryMonitor {
public:
    void begin();
    float getVoltage();         // Returns smoothed voltage
    float getRawVoltage();      // Returns single-read voltage (for diagnostics)
    int getPercentage();
    String getEstimatedTime();  // Returns "Xh Ym" format
    
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
