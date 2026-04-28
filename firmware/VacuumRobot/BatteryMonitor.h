#ifndef BATTERY_MONITOR_H
#define BATTERY_MONITOR_H

#include <Arduino.h>

// ===== BATTERY CONFIGURATION =====
// 3S Li-ion: 9.0V (empty) → 12.6V (full)
#define BATTERY_VOLTAGE_MAX     12.6    // Tegangan penuh (3S Li-ion fully charged)
#define BATTERY_VOLTAGE_MIN     9.0     // Tegangan minimum (cutoff)
#define BATTERY_FULL_RUNTIME_MIN 120    // Estimasi runtime penuh dalam menit (isi setelah discharge test)

// Smoothing parameters
#define BATTERY_ADC_SAMPLES     20      // Jumlah sampel ADC per pembacaan (noise filter)
#define BATTERY_DECAY_RATE      0.002   // Kecepatan penurunan stable voltage per update
                                        // Makin kecil = makin lambat turun = makin stabil
                                        // 0.002 ≈ ~0.02V drop per menit saat motor sag

class BatteryMonitor {
public:
    void begin();
    float getVoltage();         // Returns stable voltage (anti-sag)
    float getRawVoltage();      // Returns current actual voltage (for diagnostics)
    int getPercentage();
    String getEstimatedTime();  // Returns "Xh Ym" format
    
private:
    // Calibration factor for Voltage Divider
    // R1=30kΩ, R2=10kΩ: ideal = 4.0
    // Calibrated: actual 12.2V, ADC 3.054V → ratio 3.995
    const float _calibrationFactor = 3.995;
    
    // Stable voltage (anti-sag) — this is what we report
    float _stableVoltage = 0.0;
    bool _initialized = false;
    
    // Internal helpers
    float _readFilteredVoltage();  // Read ADC with multi-sample averaging
};

#endif
