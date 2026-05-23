#include "BatteryMonitor.h"
#include "config.h"

void BatteryMonitor::begin() {
    pinMode(PIN_BATTERY_ADC, INPUT);
    
    // Inisialisasi stable voltage dengan pembacaan pertama
    _stableVoltage = _readFilteredVoltage();
    _initialized = true;
    
    Serial.print("[BATTERY] Monitor initialized. Voltage: ");
    Serial.print(_stableVoltage, 1);
    Serial.print("V (");
    Serial.print(getPercentage());
    Serial.println("%)");
}

// ===== PEMBACAAN ADC DENGAN MULTI-SAMPLE (filter noise) =====
// Mengambil BATTERY_ADC_SAMPLES pembacaan dan merata-ratakannya
// Ini menghilangkan noise listrik, BUKAN voltage sag
float BatteryMonitor::_readFilteredVoltage() {
    long sum = 0;
    for (int i = 0; i < BATTERY_ADC_SAMPLES; i++) {
        sum += analogRead(PIN_BATTERY_ADC);
        delayMicroseconds(500);  // Jeda antar sampel ADC
    }
    float avgRaw = (float)sum / BATTERY_ADC_SAMPLES;
    
    // ESP32 ADC 12-bit (0-4095), 3.3V reference
    float voltage = (avgRaw / 4095.0) * 3.3 * _calibrationFactor;
    return voltage;
}

// ===== PEAK-HOLD WITH SLOW DECAY =====
// Teknik yang dipakai produk baterai komersial:
//
// - Saat tegangan NAIK (motor mati, baterai pulih):
//   → Langsung adopt nilai tinggi (responsive)
//
// - Saat tegangan TURUN (motor nyala, sag terjadi):
//   → Turunkan SANGAT PERLAHAN (ignore sag)
//
// Hasilnya: voltage yang ditampilkan stabil dan merepresentasikan
// kapasitas baterai sebenarnya, bukan tegangan sesaat saat motor sedang menarik arus.
//
float BatteryMonitor::getVoltage() {
    float currentReading = _readFilteredVoltage();
    
    if (!_initialized) {
        _stableVoltage = currentReading;
        _initialized = true;
        return _stableVoltage;
    }
    
    if (currentReading >= _stableVoltage) {
        // Tegangan naik atau sama → langsung adopt
        // Ini terjadi saat motor mati dan baterai pulih ke OCV
        _stableVoltage = currentReading;
    } else {
        // Tegangan lebih rendah (sag karena motor)
        // → Turunkan perlahan, jangan langsung ikut turun
        // Formula: stable = stable * (1 - rate) + reading * rate
        _stableVoltage = _stableVoltage * (1.0 - BATTERY_DECAY_RATE) 
                       + currentReading * BATTERY_DECAY_RATE;
    }
    
    return _stableVoltage;
}

// Tegangan mentah saat ini (untuk halaman diagnostic)
float BatteryMonitor::getRawVoltage() {
    return _readFilteredVoltage();
}

// Hitung persentase baterai dari stable voltage
int BatteryMonitor::getPercentage() {
    float v = getVoltage();
    int vInt = (int)(v * 100);
    int minV = (int)(BATTERY_VOLTAGE_MIN * 100);
    int maxV = (int)(BATTERY_VOLTAGE_MAX * 100);
    int pct = map(vInt, minV, maxV, 0, 100);
    return constrain(pct, 0, 100);
}

// Estimasi sisa waktu berdasarkan persentase dan power mode
String BatteryMonitor::getEstimatedTime() {
    // Default: gunakan NORMAL mode
    return getEstimatedTime("normal");
}

String BatteryMonitor::getEstimatedTime(String mode) {
    int pct = getPercentage();
    
    // Pilih runtime berdasarkan mode
    int fullRuntime;
    if (mode == "eco") {
        fullRuntime = RUNTIME_ECO_MIN;
    } else if (mode == "strong") {
        fullRuntime = RUNTIME_STRONG_MIN;
    } else {
        fullRuntime = RUNTIME_NORMAL_MIN;  // default: normal
    }
    
    // Estimasi linear: runtime penuh × persentase / 100
    int totalMinutes = (fullRuntime * pct) / 100;
    
    if (totalMinutes <= 0) {
        return "< 1m";
    } else if (totalMinutes < 10) {
        return "< 10m";
    } else {
        int hours = totalMinutes / 60;
        int mins = totalMinutes % 60;
        if (hours > 0) {
            return String(hours) + "h " + String(mins) + "m";
        } else {
            return String(mins) + "m";
        }
    }
}

// ===== BATTERY PROTECTION =====

bool BatteryMonitor::isLowBattery() {
    return getPercentage() <= BATTERY_WARNING_PCT;
}

bool BatteryMonitor::isCritical() {
    return getPercentage() <= BATTERY_CRITICAL_PCT;
}

bool BatteryMonitor::canStart() {
    return getPercentage() > BATTERY_BLOCK_START_PCT;
}

// R1 30kΩ, R2 10kΩ