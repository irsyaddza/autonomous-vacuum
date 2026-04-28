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
    // R1=30kΩ, R2=10kΩ: ideal = (30k + 10k) / 10k = 4.0
    // Calibrated with multimeter: actual 12.2V, ADC VP pin 3.054V
    // Actual Ratio: 12.2 / 3.054 = 3.9947
    const float _calibrationFactor = 3.995;
    
    // Moving average buffer for voltage smoothing
    float _samples[BATTERY_SAMPLE_COUNT];
    int _sampleIndex = 0;
    int _sampleCount = 0;       // How many samples collected so far
    bool _bufferFull = false;
    
    // Internal helpers
    float _readSingleVoltage();
    void _addSample(float voltage);
    float _getSmoothedVoltage();
};

#endif
