#include "BatteryMonitor.h"
#include "config.h"

void BatteryMonitor::begin() {
    pinMode(PIN_BATTERY_ADC, INPUT);
    
    // Pre-fill buffer dengan pembacaan awal agar smoothing langsung efektif
    // Tanpa ini, beberapa detik pertama nilainya akan fluktuatif
    for (int i = 0; i < BATTERY_SAMPLE_COUNT; i++) {
        _addSample(_readSingleVoltage());
        delay(10);
    }
    
    Serial.print("[BATTERY] Monitor initialized. Voltage: ");
    Serial.print(getVoltage(), 1);
    Serial.print("V (");
    Serial.print(getPercentage());
    Serial.println("%)");
}

// Baca tegangan mentah sekali (tanpa smoothing)
float BatteryMonitor::_readSingleVoltage() {
    int raw = analogRead(PIN_BATTERY_ADC);
    // ESP32 ADC is 12-bit (0-4095), 3.3V reference
    float voltage = (raw / 4095.0) * 3.3 * _calibrationFactor;
    return voltage;
}

// Tambahkan sample ke buffer circular
void BatteryMonitor::_addSample(float voltage) {
    _samples[_sampleIndex] = voltage;
    _sampleIndex = (_sampleIndex + 1) % BATTERY_SAMPLE_COUNT;
    
    if (!_bufferFull) {
        _sampleCount++;
        if (_sampleCount >= BATTERY_SAMPLE_COUNT) {
            _bufferFull = true;
        }
    }
}

// Hitung rata-rata dari semua sampel di buffer
float BatteryMonitor::_getSmoothedVoltage() {
    int count = _bufferFull ? BATTERY_SAMPLE_COUNT : _sampleCount;
    if (count == 0) return 0.0;
    
    float sum = 0.0;
    for (int i = 0; i < count; i++) {
        sum += _samples[i];
    }
    return sum / count;
}

// Public: Baca dan return tegangan yang sudah di-smoothing
// Setiap panggilan menambah 1 sampel baru ke buffer
float BatteryMonitor::getVoltage() {
    _addSample(_readSingleVoltage());
    return _getSmoothedVoltage();
}

// Public: Baca tegangan mentah sekali (untuk halaman diagnostic)
float BatteryMonitor::getRawVoltage() {
    return _readSingleVoltage();
}

// Hitung persentase baterai berdasarkan tegangan
// Menggunakan kurva linear sederhana: 9.0V (0%) → 12.6V (100%)
int BatteryMonitor::getPercentage() {
    float v = getVoltage();
    // Konversi ke integer untuk map(): 9.0V = 900, 12.6V = 1260
    int vInt = (int)(v * 100);
    int minV = (int)(BATTERY_VOLTAGE_MIN * 100);
    int maxV = (int)(BATTERY_VOLTAGE_MAX * 100);
    int pct = map(vInt, minV, maxV, 0, 100);
    return constrain(pct, 0, 100);
}

// Estimasi sisa waktu berdasarkan persentase
// Format: "Xh Ym" atau "< 10m" untuk baterai hampir habis
String BatteryMonitor::getEstimatedTime() {
    int pct = getPercentage();
    
    // Estimasi linear: runtime penuh * persentase / 100
    int totalMinutes = (BATTERY_FULL_RUNTIME_MIN * pct) / 100;
    
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

// R1 30kΩ, R2 10kΩ