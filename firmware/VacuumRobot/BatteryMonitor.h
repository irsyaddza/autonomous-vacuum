#ifndef BATTERY_MONITOR_H
#define BATTERY_MONITOR_H

#include <Arduino.h>

// ===== BATTERY CONFIGURATION =====
// 3S Li-ion: 9.0V (empty) → 12.6V (full)
#define BATTERY_VOLTAGE_MAX     12.6    // Tegangan penuh (3S Li-ion fully charged)
#define BATTERY_VOLTAGE_MIN     9.0     // Tegangan minimum (cutoff)

// ===== BATTERY PROTECTION THRESHOLDS =====
#define BATTERY_WARNING_PCT     15      // Persentase untuk low battery warning
#define BATTERY_CRITICAL_PCT    0       // Persentase untuk auto-stop (0% under load ≈ 9.0V)
#define BATTERY_BLOCK_START_PCT 10      // Minimum persentase untuk izinkan start cleaning

// ===== RUNTIME ESTIMATION PER MODE =====
// Based on actual discharge test: ECO 93%→0% dalam 91 menit
// Extrapolated: ECO 100%→0% ≈ 98 menit
#define RUNTIME_ECO_MIN         98      // Menit runtime mode ECO (PWM 150) - TESTED
#define RUNTIME_NORMAL_MIN      80      // Menit runtime mode NORMAL (PWM 200) - estimasi
#define RUNTIME_STRONG_MIN      65      // Menit runtime mode STRONG (PWM 255) - estimasi

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
    String getEstimatedTime();              // Returns "Xh Ym" format (uses current power mode)
    String getEstimatedTime(String mode);   // Returns "Xh Ym" format for specific mode
    
    // Battery protection
    bool isLowBattery();        // True if <= BATTERY_WARNING_PCT (15%)
    bool isCritical();          // True if <= BATTERY_CRITICAL_PCT (0%)
    bool canStart();            // True if > BATTERY_BLOCK_START_PCT (10%)
    
private:
    // Calibration factor for Voltage Divider
    // R1=30kΩ, R2=10kΩ: ideal = 4.0
    // Calibrated: actual 12.2V, ADC 3.054V -> ratio 3.995
    const float _calibrationFactor = 3.995;
    
    // Stable voltage (anti-sag) - this is what we report
    float _stableVoltage = 0.0;
    bool _initialized = false;
    
    // Internal helpers
    float _readFilteredVoltage();  // Read ADC with multi-sample averaging
};

#endif