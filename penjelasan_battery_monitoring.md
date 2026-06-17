# Dokumentasi Sistem Battery Monitoring

## 1. Pendahuluan

Sistem Battery Monitoring pada robot vacuum otonom berfungsi untuk memantau tegangan baterai 3S Li-ion secara real-time, menghitung persentase kapasitas, mengestimasi sisa waktu operasi, dan menampilkan seluruh informasi tersebut di dashboard website. Sistem ini dilengkapi dengan **Battery Protection System** yang mencakup auto-stop saat baterai habis, peringatan low battery, dan pencegahan start saat baterai terlalu rendah. Sistem ini juga dirancang untuk mengatasi fenomena **voltage sag** (penurunan tegangan sementara saat motor beroperasi) agar data yang ditampilkan kepada pengguna tetap akurat dan stabil.

---

## 2. Spesifikasi Hardware

### 2.1 Baterai
- **Tipe:** 3S Li-ion (3 sel seri)
- **Tegangan Penuh (Fully Charged):** 12.6V (4.2V per sel)
- **Tegangan Minimum (Cutoff):** 9.0V (3.0V per sel)
- **Tegangan Nominal:** ~11.1V (3.7V per sel)

### 2.2 Rangkaian Voltage Divider
Karena tegangan baterai (hingga 12.6V) melebihi batas ADC ESP32 (3.3V), digunakan rangkaian **voltage divider** untuk menurunkan tegangan ke rentang yang aman:

```
Baterai (+) ─── [ R1 = 30kΩ ] ─── Titik Tengah ─── [ R2 = 10kΩ ] ─── GND
                                        │
                                   GPIO 36 (VP)
                                   ESP32 ADC Input
```

#### Rumus Voltage Divider:
```
V_out = V_in × R2 / (R1 + R2)
V_out = V_in × 10kΩ / (30kΩ + 10kΩ)
V_out = V_in × 0.25
```

#### Verifikasi Keamanan:
```
V_out_max = 12.6V × 0.25 = 3.15V → Aman untuk ESP32 ADC (batas 3.3V)
```

### 2.3 Pin ESP32
- **ADC Input:** GPIO 36 (VP) — Pin `input-only` dengan ADC1 Channel 0
- **Resolusi ADC:** 12-bit (rentang nilai: 0–4095)
- **Tegangan Referensi ADC:** 3.3V

---

## 3. Kalibrasi

### 3.1 Faktor Kalibrasi (Calibration Factor)
Faktor kalibrasi digunakan untuk mengkonversi pembacaan ADC (dalam volt di pin VP) kembali menjadi tegangan baterai asli.

#### Rumus Ideal:
```
Rasio = (R1 + R2) / R2 = (30kΩ + 10kΩ) / 10kΩ = 4.0
```

#### Kalibrasi Aktual (dengan Multimeter):
Karena toleransi resistor, nilai ideal 4.0 tidak 100% akurat. Dilakukan pengukuran langsung:
```
Tegangan baterai (multimeter): 12.2V
Tegangan di pin VP (multimeter): 3.054V
Rasio aktual = 12.2V / 3.054V = 3.995
```

#### Nilai Final:
```cpp
const float _calibrationFactor = 3.995;
```

### 3.2 Rumus Konversi ADC ke Tegangan Baterai
```
V_baterai = (ADC_raw / 4095) × 3.3V × _calibrationFactor
```

**Contoh perhitungan:**
```
ADC_raw = 2400
V_baterai = (2400 / 4095) × 3.3 × 3.995
V_baterai = 0.5861 × 3.3 × 3.995
V_baterai = 7.727V
```

---

## 4. Algoritma Pembacaan

### 4.1 Multi-Sample ADC Averaging (Filter Noise)
Setiap kali tegangan dibaca, ESP32 mengambil **20 sampel ADC** secara berurutan dan merata-ratakannya. Ini menghilangkan noise listrik dari kabel dan komponen elektronik.

```
Fungsi: _readFilteredVoltage()
```

#### Proses:
1. Ambil 20 pembacaan `analogRead(PIN_BATTERY_ADC)` dengan jeda 500μs antar sampel
2. Jumlahkan semua pembacaan
3. Hitung rata-rata: `avgRaw = sum / 20`
4. Konversi ke tegangan: `voltage = (avgRaw / 4095.0) × 3.3 × 3.995`

```
Parameter:
- BATTERY_ADC_SAMPLES = 20
- Jeda antar sampel = 500 μs
- Total waktu per pembacaan ≈ 10 ms
```

### 4.2 Peak-Hold with Slow Decay (Anti Voltage Sag)
Ini adalah algoritma utama yang mengatasi masalah **voltage sag**. Saat motor beroperasi dan menarik arus besar, tegangan baterai turun sementara (bisa 1–2V). Saat motor berhenti, tegangan kembali naik. Tanpa algoritma ini, dashboard akan menunjukkan fluktuasi tegangan yang membingungkan.

```
Fungsi: getVoltage()
Variabel: _stableVoltage (tegangan yang dilaporkan)
```

#### Logika:

**Kondisi 1 — Tegangan NAIK (motor mati / baterai pulih):**
```
Jika pembacaan_baru >= _stableVoltage:
    _stableVoltage = pembacaan_baru    ← Langsung adopt
```

**Kondisi 2 — Tegangan TURUN (motor sag / baterai menurun):**
```
Jika pembacaan_baru < _stableVoltage:
    _stableVoltage = _stableVoltage × (1 - decay_rate) + pembacaan_baru × decay_rate
```

#### Rumus Exponential Weighted Moving Average (EWMA):
```
S(t) = S(t-1) × (1 - α) + X(t) × α

Dimana:
  S(t)   = Stable voltage saat ini
  S(t-1) = Stable voltage sebelumnya
  X(t)   = Pembacaan tegangan saat ini
  α      = BATTERY_DECAY_RATE = 0.002
```

#### Contoh Simulasi:
```
Stable voltage awal: 10.70V
Motor nyala, pembacaan turun ke 9.90V:

Update 1: 10.70 × 0.998 + 9.90 × 0.002 = 10.68V  (turun 0.02V)
Update 2: 10.68 × 0.998 + 9.90 × 0.002 = 10.66V  (turun 0.02V)
Update 3: 10.66 × 0.998 + 9.90 × 0.002 = 10.64V  (turun 0.02V)
...

Motor mati, pembacaan naik ke 10.65V:
→ Langsung adopt: _stableVoltage = 10.65V
```

**Dampak:** Tegangan yang ditampilkan hanya turun ~0.02V per update saat motor sag, bukan langsung jatuh 1–2V.

#### Parameter:
```
BATTERY_DECAY_RATE = 0.002
- Makin kecil → makin lambat turun → makin stabil
- Makin besar → makin responsif → kurang stabil
```

---

## 5. Perhitungan Persentase

### 5.1 Rumus Pemetaan Linear
```
Persentase = map(V × 100, V_min × 100, V_max × 100, 0, 100)
Persentase = constrain(Persentase, 0, 100)
```

#### Parameter:
```
V_min = 9.0V  (BATTERY_VOLTAGE_MIN) → 0%
V_max = 12.6V (BATTERY_VOLTAGE_MAX) → 100%
```

#### Contoh:
```
V_baterai = 10.8V
Persentase = map(1080, 900, 1260, 0, 100)
Persentase = (1080 - 900) / (1260 - 900) × 100
Persentase = 180 / 360 × 100 = 50%
```

### 5.2 Tabel Referensi Tegangan ↔ Persentase

| Tegangan | Persentase | Status |
|:---------|:-----------|:-------|
| 12.6V | 100% | Penuh |
| 12.0V | 83% | Tinggi |
| 11.1V | 58% | Sedang |
| 10.5V | 42% | Rendah |
| 9.9V | 25% | Kritis |
| 9.0V | 0% | Habis (Cutoff) |

---

## 6. Estimasi Sisa Waktu

### 6.1 Rumus
Estimasi sisa waktu sekarang **dinamis berdasarkan power mode** yang aktif:
```
Sisa_Menit = (Runtime_Mode × Persentase) / 100
```

#### Parameter Runtime per Mode:
| Mode | PWM | Runtime (menit) | Sumber |
|:-----|:----|:-----------------|:-------|
| ECO | 150 | 98 | **Diuji** (93% → 0% dalam 91 menit) |
| NORMAL | 200 | 80 | Estimasi |
| STRONG | 255 | 65 | Estimasi |

```cpp
#define RUNTIME_ECO_MIN         98   // TESTED
#define RUNTIME_NORMAL_MIN      80   // Estimasi
#define RUNTIME_STRONG_MIN      65   // Estimasi
```

#### Contoh:
```
Mode: ECO, Persentase = 50%
Sisa_Menit = (98 × 50) / 100 = 49 menit
Output: "49m"

Mode: STRONG, Persentase = 50%
Sisa_Menit = (65 × 50) / 100 = 32 menit
Output: "32m"
```

### 6.2 Format Output
| Kondisi | Format |
|:--------|:-------|
| ≤ 0 menit | "< 1m" |
| < 10 menit | "< 10m" |
| < 60 menit | "45m" |
| ≥ 60 menit | "1h 30m" |

---

## 7. Alur Data (Data Flow)

### 7.1 Diagram Alur

```
┌─────────────────────────────────────────────────────────────────────┐
│                          ESP32 FIRMWARE                             │
│                                                                     │
│  ┌──────────┐     ┌──────────────────┐     ┌───────────────────┐   │
│  │ Baterai  │────▶│  Voltage Divider │────▶│  GPIO 36 (VP)     │   │
│  │ 3S Li-ion│     │  R1=30k, R2=10k  │     │  ADC Input        │   │
│  └──────────┘     └──────────────────┘     └────────┬──────────┘   │
│                                                      │              │
│                                                      ▼              │
│                                          ┌──────────────────────┐  │
│                                          │  BatteryMonitor.cpp  │  │
│                                          │                      │  │
│                                          │  1. analogRead ×20   │  │
│                                          │  2. Rata-rata ADC    │  │
│                                          │  3. × 3.3 × 3.995   │  │
│                                          │  4. Peak-Hold Decay  │  │
│                                          │  5. → Voltage        │  │
│                                          │  6. → Percentage     │  │
│                                          │  7. → Estimated Time │  │
│                                          └────────┬─────────────┘  │
│                                                   │                 │
│                              ┌────────────────────┼──────┐         │
│                              ▼                    ▼      ▼         │
│                    ┌──────────────┐      ┌────────────────────┐    │
│                    │ Serial Log   │      │  ApiClient.cpp     │    │
│                    │ (Debug)      │      │  sendBattery()     │    │
│                    └──────────────┘      └────────┬───────────┘    │
│                                                   │                 │
└───────────────────────────────────────────────────┼─────────────────┘
                                                    │
                                          HTTP POST │ setiap 60 detik
                                                    │
                                                    ▼
┌─────────────────────────────────────────────────────────────────────┐
│                       LARAVEL BACKEND                               │
│                                                                     │
│  POST /v1/vacuum/battery                                            │
│  ┌──────────────────────────────────────────────────────────────┐   │
│  │  VacuumAPIController::updateBattery()                        │   │
│  │                                                              │   │
│  │  Menerima JSON:                                              │   │
│  │  {                                                           │   │
│  │    "battery_percent": 50,                                    │   │
│  │    "battery_voltage": 10.8,                                  │   │
│  │    "estimated_time": "1h 0m"                                 │   │
│  │  }                                                           │   │
│  │                                                              │   │
│  │  → Simpan ke tabel `battery_logs` (MySQL)                    │   │
│  └──────────────────────────────────────────────────────────────┘   │
│                                                                     │
│  GET /v1/vacuum/battery/latest                                      │
│  GET /v1/vacuum/battery/history                                     │
│  GET /v1/vacuum/full-status (termasuk data baterai)                 │
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘
                                                    │
                                          HTTP GET  │ polling dari browser
                                                    │
                                                    ▼
┌─────────────────────────────────────────────────────────────────────┐
│                      WEBSITE (BROWSER)                              │
│                                                                     │
│  main.blade.php — Dashboard                                         │
│  ┌──────────────────────────────────────────────────────────────┐   │
│  │  Poll /v1/vacuum/full-status setiap beberapa detik           │   │
│  │  → updateBatteryUI(data)                                     │   │
│  │                                                              │   │
│  │  Menampilkan:                                                │   │
│  │  • Persentase (%) + progress bar warna dinamis               │   │
│  │  • Tegangan (V) — diambil dari battery_voltage               │   │
│  │  • Estimated time — diambil dari estimated_time              │   │
│  │  • Ikon baterai berubah sesuai level                         │   │
│  └──────────────────────────────────────────────────────────────┘   │
│                                                                     │
│  diagnostic.blade.php — Diagnostic                                  │
│  ┌──────────────────────────────────────────────────────────────┐   │
│  │  Poll langsung ke ESP32 GET /diagnostic                      │   │
│  │  → Menampilkan voltage dan percent real-time                 │   │
│  └──────────────────────────────────────────────────────────────┘   │
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘
```

### 7.2 Interval Pengiriman Data
| Proses | Interval | Keterangan |
|:-------|:---------|:-----------|
| ADC Sampling | ~10ms (20 sampel × 500μs) | Setiap kali `getVoltage()` dipanggil |
| Kirim ke Laravel | 60.000ms (1 menit) | `BATTERY_SEND_INTERVAL` di `config.h` |
| Polling dari Browser | ~3.000ms | JavaScript di `main.blade.php` |
| Diagnostic Polling | 300ms | JavaScript di `diagnostic.blade.php` |

---

## 8. Database

### 8.1 Tabel `battery_logs`
| Kolom | Tipe | Keterangan |
|:------|:-----|:-----------|
| `id` | int (PK) | Auto increment |
| `battery_percent` | int | 0–100 |
| `battery_voltage` | float | Dalam volt (contoh: 10.71) |
| `estimated_time` | string | Contoh: "1h 30m" |
| `created_at` | timestamp | Waktu data diterima |
| `updated_at` | timestamp | Sama dengan created_at |

### 8.2 API Endpoints Terkait Baterai

| Method | Endpoint | Fungsi |
|:-------|:---------|:-------|
| POST | `/v1/vacuum/battery` | ESP32 mengirim data baterai periodik |
| GET | `/v1/vacuum/battery/latest` | Ambil data terbaru |
| GET | `/v1/vacuum/battery/history` | Ambil histori (default 60 menit terakhir) |
| GET | `/v1/vacuum/full-status` | Ambil semua status termasuk baterai |
| POST | `/v1/vacuum/battery-event` | ESP32 mengirim event baterai (warning/auto-stop) |
| GET | `/v1/vacuum/battery-events/latest` | Ambil event baterai terbaru (5 menit terakhir) |

---

## 9. Tampilan Website (UI)

### 9.1 Dashboard (`main.blade.php`)
Kartu Battery Monitoring menampilkan:
- **Persentase** — angka besar (contoh: "85%")
- **Progress bar** — warna berubah dinamis:
  - Hijau: > 50%
  - Kuning: 20–50%
  - Merah: < 20%
- **Tegangan** — realtime dari ESP32 (contoh: "10.7 V")
- **Estimated time** — sisa waktu berdasarkan mode aktif (contoh: "Est: 49m")
- **Ikon baterai** — berubah sesuai level (full/three-quarters/half/quarter/empty)

#### Notifikasi Battery Protection:
- **Low Battery Warning (15%)** — Notifikasi kuning persistent: "⚠️ Low battery warning! 15% remaining (10.4V). Robot will auto-stop at 0%."
- **Auto-Stop (0%)** — Notifikasi merah persistent: "🔋 Robot auto-stopped! Battery depleted (0%, 9.0V). Please charge the battery."
- **Blocked Start (<10%)** — Notifikasi kuning persistent: "🔋 Battery too low to start (8%). Need > 10%"
- **Auto-Stop State** — Status info berubah menjadi: "🔋 Auto-stopped: Battery depleted"

### 9.2 Diagnostic (`diagnostic.blade.php`)
Status bar menampilkan tegangan dan persentase dari data diagnostic ESP32 langsung (bukan dari database).

---

## 10. Battery Protection System

Sistem proteksi baterai berjalan di firmware (`RobotController.cpp`) dan terintegrasi dengan website.

### 10.1 Tiga Level Proteksi

| Level | Threshold | Aksi | Kapan |
|:------|:----------|:-----|:------|
| **Block Start** | ≤ 10% | Tolak command "start" dari website | Saat user menekan START |
| **Low Battery Warning** | ≤ 15% | Kirim event peringatan ke server + notifikasi website | Saat sedang cleaning (sekali) |
| **Auto-Stop** | ≤ 0% | Stop semua motor + kirim event ke server + update state | Saat sedang cleaning (sekali) |

### 10.2 Alur Proteksi

```
Robot dalam state "working" (cleaning aktif)
    │
    ├── Setiap loop cycle, cek baterai:
    │   │
    │   ├── Persentase ≤ 0% (CRITICAL)
    │   │   ├── Stop cleaning algorithm
    │   │   ├── Stop semua motor (wheel, brush, vacuum)
    │   │   ├── Kirim event "auto_stop_low_battery" ke server
    │   │   ├── Update state ke "stopping" di server
    │   │   └── Dashboard: notifikasi merah + status "Auto-stopped: Battery depleted"
    │   │
    │   ├── Persentase ≤ 15% (WARNING)
    │   │   ├── Kirim event "low_battery_warning" ke server (sekali)
    │   │   ├── Dashboard: notifikasi kuning "Low battery warning!"
    │   │   └── Robot tetap lanjut cleaning
    │   │
    │   └── Persentase > 15%
    │       └── Normal operation
    │
Robot dalam state bukan "working" (standby/stopping)
    │
    └── Jika baterai > 10% → Reset flag proteksi (siap digunakan lagi)
```

### 10.3 Hasil Discharge Test Aktual

**Test kondisi:** Mode ECO (PWM 150), autonomous cleaning

| Parameter | Nilai |
|:----------|:------|
| Baterai awal | 93% (~12.46V OCV) |
| Baterai akhir (under load) | 0% (~9.0V) |
| Baterai akhir (no load/OCV) | 25% (~9.9V) |
| Durasi | 1 jam 31 menit |
| Extrapolasi 100% → 0% | ~98 menit |

**Catatan penting:**
- Saat motor aktif, tegangan turun signifikan (voltage sag ~0.9V di level rendah)
- 0% under load ≈ 25% OCV — ini aman, masih jauh di atas BMS cutoff (~7.5–9.0V)
- Algoritma Peak-Hold Decay setelah operasi lama akan konvergen ke tegangan under-load

---

## 11. Konfigurasi yang Dapat Diubah

Semua parameter berada di `BatteryMonitor.h`:

| Parameter | Default | Satuan | Fungsi |
|:----------|:--------|:-------|:-------|
| `BATTERY_VOLTAGE_MAX` | 12.6 | Volt | Tegangan penuh (100%) |
| `BATTERY_VOLTAGE_MIN` | 9.0 | Volt | Tegangan minimum (0%) |
| `RUNTIME_ECO_MIN` | 98 | Menit | Runtime mode ECO (diuji) |
| `RUNTIME_NORMAL_MIN` | 80 | Menit | Runtime mode NORMAL (estimasi) |
| `RUNTIME_STRONG_MIN` | 65 | Menit | Runtime mode STRONG (estimasi) |
| `BATTERY_WARNING_PCT` | 15 | % | Threshold low battery warning |
| `BATTERY_CRITICAL_PCT` | 0 | % | Threshold auto-stop |
| `BATTERY_BLOCK_START_PCT` | 10 | % | Minimum untuk izinkan start |
| `BATTERY_ADC_SAMPLES` | 20 | - | Jumlah sampel ADC per pembacaan |
| `BATTERY_DECAY_RATE` | 0.002 | - | Kecepatan decay saat voltage sag |

Dan di `config.h`:

| Parameter | Default | Satuan | Fungsi |
|:----------|:--------|:-------|:-------|
| `PIN_BATTERY_ADC` | 36 (VP) | - | Pin ADC untuk voltage divider |
| `BATTERY_SEND_INTERVAL` | 60000 | ms | Interval kirim data ke server |

---

## 12. File Terkait

| File | Lokasi | Peran |
|:-----|:-------|:------|
| `BatteryMonitor.h` | `firmware/VacuumRobot/` | Header class + konfigurasi parameter + threshold proteksi |
| `BatteryMonitor.cpp` | `firmware/VacuumRobot/` | Pembacaan ADC, smoothing, persentase, estimasi waktu per mode, proteksi |
| `RobotController.cpp` | `firmware/VacuumRobot/` | Logika utama battery protection (auto-stop, warning, recovery) |
| `config.h` | `firmware/VacuumRobot/` | Pin ADC dan interval pengiriman |
| `VacuumRobot.ino` | `firmware/VacuumRobot/` | Loop utama: memanggil `getVoltage()` dan `sendBattery()` periodik |
| `ApiClient.cpp` | `firmware/VacuumRobot/` | `sendBattery()`, `sendBatteryEvent()`, `sendAutoStop()`, block start |
| `VacuumAPIController.php` | `app/Http/Controllers/` | Endpoint Laravel: battery data, battery events, latest event polling |
| `BatteryLog.php` | `app/Models/` | Model Eloquent untuk tabel `battery_logs` |
| `CommandLog.php` | `app/Models/` | Model Eloquent untuk tabel `command_logs` (termasuk battery events) |
| `main.blade.php` | `resources/views/` | Dashboard UI: data baterai + notifikasi proteksi + blocked start |
| `diagnostic.blade.php` | `resources/views/` | Diagnostic UI: data real-time langsung dari ESP32 |
| `web.php` | `routes/` | Route definitions termasuk battery-event endpoints |

---

## 13. Battery Protection System (Detail)

Sistem proteksi baterai bertujuan melindungi baterai Li-ion dari **over-discharge** yang dapat merusak sel secara permanen, serta memberikan pengalaman pengguna yang aman. Sistem ini berjalan sepenuhnya di **firmware ESP32** (`RobotController.cpp`) dan terintegrasi dengan **website dashboard** (`main.blade.php`).

### 13.1 Mengapa Diperlukan?

Baterai Li-ion 3S memiliki batas cutoff 9.0V (3.0V per sel). Jika tegangan turun di bawah ini:
- Sel baterai bisa rusak permanen
- Kapasitas baterai menurun drastis secara ireversibel
- Dalam kasus ekstrem, baterai bisa membengkak atau terbakar

Oleh karena itu, firmware **harus** menghentikan robot sebelum tegangan mencapai level berbahaya.

### 13.2 Tiga Level Proteksi

```
┌──────────────────────────────────────────────────────────────────────┐
│                    BATTERY PROTECTION LEVELS                         │
│                                                                      │
│  100% ████████████████████████████████████████  Fully Charged        │
│                                                                      │
│   50% ████████████████████                      Normal Operation     │
│                                                                      │
│   15% ████████  ← ⚠️ LOW BATTERY WARNING                            │
│                    Kirim notifikasi ke dashboard (sekali)             │
│                    Robot TETAP lanjut cleaning                        │
│                                                                      │
│   10% ██████  ← 🚫 BLOCK START                                      │
│                  Tolak perintah START dari website                    │
│                  "Battery too low to start (8%). Need > 10%"         │
│                                                                      │
│    0% █  ← 🛑 AUTO-STOP (CRITICAL)                                  │
│            Stop SEMUA motor segera                                   │
│            Kirim event ke server                                     │
│            Dashboard: notifikasi merah                               │
│                                                                      │
│  9.0V ─── Cutoff voltage (0% under load)                             │
└──────────────────────────────────────────────────────────────────────┘
```

### 13.3 Level 1: Block Start (≤ 10%)

**Kapan aktif:** Saat user menekan tombol START di website, dan baterai ≤ 10%.

**Tujuan:** Mencegah robot beroperasi dengan baterai yang sudah sangat rendah, karena robot akan segera auto-stop dalam hitungan menit yang tidak berguna.

**Implementasi di firmware (`ApiClient.cpp`):**
```cpp
// Di handler /command POST
if (command == "start") {
    if (!battery.canStart()) {
        // Tolak command, kirim response error
        errDoc["message"] = "Battery too low to start (X%). Need > 10%";
        errDoc["blocked"] = true;
        server.send(400, ...);
        return;
    }
}
```

**Fungsi `canStart()` di `BatteryMonitor.cpp`:**
```cpp
bool BatteryMonitor::canStart() {
    return getPercentage() > BATTERY_BLOCK_START_PCT;  // > 10%
}
```

**Yang terjadi di website:**
- Tombol START ditekan → ESP32 menolak dengan HTTP 400
- Dashboard menampilkan notifikasi kuning:
  `"🔋 Battery too low to start (8%). Need > 10%"`
- User harus mengisi daya terlebih dahulu

### 13.4 Level 2: Low Battery Warning (≤ 15%)

**Kapan aktif:** Saat robot sedang cleaning (state = "working") dan baterai turun ke ≤ 15%.

**Tujuan:** Memberikan peringatan awal agar user bersiap (misalnya mendekatkan charger atau menghentikan robot secara manual).

**Implementasi di firmware (`RobotController.cpp`):**
```cpp
if (battery.isLowBattery() && !_lowBatteryWarned) {
    _lowBatteryWarned = true;  // Hanya sekali per sesi
    api.sendBatteryEvent("low_battery_warning", pct, volt);
}
```

**Perilaku:**
- Warning hanya dikirim **sekali** per sesi cleaning (flag `_lowBatteryWarned`)
- Robot **tetap lanjut cleaning** — tidak berhenti
- Dashboard menampilkan notifikasi kuning persistent:
  `"⚠️ Low battery warning! 15% remaining (10.4V). Robot will auto-stop at 0%."`

### 13.5 Level 3: Auto-Stop (≤ 0%)

**Kapan aktif:** Saat robot sedang cleaning dan baterai turun ke 0% (≈ 9.0V under load).

**Tujuan:** Menghentikan robot secara otomatis untuk melindungi baterai dari over-discharge.

**Implementasi di firmware (`RobotController.cpp`):**
```cpp
if (battery.isCritical() && !_autoStopped) {
    _autoStopped = true;
    
    // Stop autonomous cleaning algorithm
    _cleaner.stop();
    
    // Stop SEMUA motor (wheel, brush, vacuum)
    stopAll();
    
    // Notify server
    api.sendAutoStop(pct, volt);
    
    return;  // Skip all motor control
}
```

**Alur `sendAutoStop()` di `ApiClient.cpp`:**
1. Kirim battery event `"auto_stop_low_battery"` ke endpoint `/battery-event`
2. Kirim command log ke endpoint `/command-log` dengan command `"auto_stop_low_battery"`
3. Update local state ke `"stopping"`

**Yang terjadi di website:**
- Dashboard menampilkan notifikasi merah persistent:
  `"🔋 Robot auto-stopped! Battery depleted (0%, 9.0V). Please charge the battery."`
- Status info berubah menjadi: `"🔋 Auto-stopped: Battery depleted"`

### 13.6 Recovery (Reset Flag Proteksi)

Setelah auto-stop, flag proteksi (`_autoStopped` dan `_lowBatteryWarned`) perlu di-reset agar robot bisa digunakan lagi setelah diisi daya.

**Kondisi reset:**
```cpp
// Hanya reset saat BUKAN sedang working DAN baterai sudah > 10%
if (targetState != "working") {
    if (_autoStopped && battery.getPercentage() > BATTERY_BLOCK_START_PCT) {
        _autoStopped = false;
        _lowBatteryWarned = false;
    }
}
```

**Artinya:**
- Robot harus dalam state selain "working" (misalnya "stopping" atau "standby")
- Baterai harus sudah diisi sampai > 10%
- Setelah kedua kondisi terpenuhi, robot siap digunakan lagi

### 13.7 Alur Lengkap Battery Protection

```
Robot START
    │
    ├── Baterai ≤ 10%?
    │   ├── YA → ❌ BLOCKED! "Battery too low" (HTTP 400)
    │   └── TIDAK → ✅ Robot mulai cleaning
    │
    └── Robot sedang CLEANING (loop cycle)
        │
        ├── Baterai ≤ 0%? (isCritical)
        │   ├── YA & belum autoStopped → 🛑 AUTO-STOP
        │   │   ├── Stop cleaner algorithm
        │   │   ├── Stop ALL motors
        │   │   ├── Send event ke server
        │   │   └── Set _autoStopped = true
        │   └── TIDAK ↓
        │
        ├── Baterai ≤ 15%? (isLowBattery)
        │   ├── YA & belum warned → ⚠️ WARNING
        │   │   ├── Send event ke server
        │   │   ├── Dashboard notifikasi kuning
        │   │   ├── Set _lowBatteryWarned = true
        │   │   └── Robot TETAP lanjut cleaning
        │   └── TIDAK ↓
        │
        └── Normal operation (baterai > 15%)

Setelah AUTO-STOP, user mengisi daya:
    │
    ├── Baterai > 10% & state ≠ "working"
    │   └── Reset flags → Robot siap digunakan lagi
    │
    └── Baterai masih ≤ 10%
        └── Flags tetap aktif, START masih diblokir
```

### 13.8 Parameter Proteksi

| Parameter | Nilai | Lokasi | Fungsi |
|:----------|:------|:-------|:-------|
| `BATTERY_WARNING_PCT` | 15% | `BatteryMonitor.h` | Threshold low battery warning |
| `BATTERY_CRITICAL_PCT` | 0% | `BatteryMonitor.h` | Threshold auto-stop |
| `BATTERY_BLOCK_START_PCT` | 10% | `BatteryMonitor.h` | Minimum % untuk izinkan start |

---

## 14. Soft Start System

### 14.1 Apa itu Soft Start?

Soft Start adalah mekanisme **menaikkan PWM motor secara bertahap** (ramping) dari 0 ke target, bukan langsung loncat dari 0 ke nilai penuh. Ini diterapkan pada **motor vakum** dan **motor brush (sapu)**.

### 14.2 Mengapa Diperlukan?

Saat motor DC langsung dinyalakan dengan PWM penuh (misalnya 200 atau 255), terjadi fenomena **inrush current** — lonjakan arus yang sangat besar sesaat karena motor belum berputar (rotor stasioner memiliki resistansi sangat rendah).

**Dampak tanpa soft start:**
- **Voltage sag besar** — tegangan baterai drop tajam, bisa menyebabkan ESP32 reset
- **Kerusakan komponen** — Driver motor (L298N) dan konektor menerima arus berlebih
- **Pembacaan baterai tidak akurat** — voltage sag besar membuat persentase seolah-olah turun drastis
- **Stress mekanis** — Motor dan belt menerima torsi mendadak

**Dengan soft start:**
```
Tanpa Soft Start:          Dengan Soft Start:
PWM                        PWM
255 ┤█████████████          255 ┤          ▄█████
    │                           │        ▄▀
    │                           │      ▄▀
    │                           │    ▄▀
    │                           │  ▄▀
  0 ┤─────┤█████████          0 ┤─▀──────────────
    t=0  (instant)               t=0    ~400ms
```

### 14.3 Cara Kerja (Non-Blocking)

Soft start menggunakan pendekatan **state-based, non-blocking** menggunakan `millis()`. Ini berarti proses ramping **tidak memblokir loop utama** — sensor, komunikasi, dan fitur lain tetap berjalan normal.

**Algoritma:**
```
1. setPower(target) dipanggil
2. Set _targetPower = target
3. Set _ramping = true
4. Setiap SOFT_START_RAMP_INTERVAL ms:
   - Jika _currentPower < _targetPower:
       _currentPower += SOFT_START_RAMP_STEP  (naik)
   - Jika _currentPower > _targetPower:
       _currentPower -= SOFT_START_RAMP_STEP  (turun)
   - Jika _currentPower == _targetPower:
       _ramping = false (selesai)
   - Tulis _currentPower ke hardware via ledcWrite()
```

### 14.4 Implementasi Motor Vakum (`VacuumMotor.cpp`)

```cpp
// Set target power DENGAN soft start (non-blocking)
void VacuumMotor::setPower(int pwm) {
    _targetPower = constrain(pwm, 0, 255);
    
    if (_targetPower == 0) {
        // STOP langsung — TIDAK pakai ramp down (safety)
        _applyPower(0);
        _currentPower = 0;
        _ramping = false;
        return;
    }
    
    // Mulai ramping
    _ramping = true;
    _lastRampTime = millis();
}

// Dipanggil setiap loop() — update ramp step
void VacuumMotor::updateSoftStart() {
    if (!_ramping) return;
    if (millis() - _lastRampTime < SOFT_START_RAMP_INTERVAL) return;
    _lastRampTime = millis();
    
    if (_currentPower < _targetPower) {
        _currentPower += SOFT_START_RAMP_STEP;
        if (_currentPower >= _targetPower) {
            _currentPower = _targetPower;
            _ramping = false;  // Selesai
        }
    }
    _applyPower(_currentPower);  // Tulis ke hardware
}
```

### 14.5 Implementasi Motor Brush (`BrushMotor.cpp`)

Motor brush memiliki mekanisme yang sama persis, dengan tambahan dukungan **arah rotasi** (forward/reverse):

```cpp
void BrushMotor::forward() {
    _isRunning = true;
    _isForward = true;
    _targetSpeed = BRUSH_SPEED;  // 150
    _ramping = true;
    _lastRampTime = millis();
}

void BrushMotor::updateSoftStart() {
    // Logika sama dengan VacuumMotor
    // Bedanya: _applySpeed() memilih pin berdasarkan _isForward
}

void BrushMotor::_applySpeed() {
    if (_isForward) {
        ledcWrite(PIN_BRUSH_FWD, _currentSpeed);
        ledcWrite(PIN_BRUSH_REV, 0);
    } else {
        ledcWrite(PIN_BRUSH_FWD, 0);
        ledcWrite(PIN_BRUSH_REV, _currentSpeed);
    }
}
```

### 14.6 Integrasi di Loop Utama (`RobotController.cpp`)

Kedua motor di-update setiap iterasi `loop()`:

```cpp
void RobotController::update() {
    // HARUS dipanggil setiap loop untuk non-blocking ramp
    vacuum.updateSoftStart();
    brush.updateSoftStart();
    
    // ... logic lainnya (sensor, cleaning, battery protection)
}
```

### 14.7 Perbedaan setPower() vs setPowerDirect()

Motor vakum memiliki dua cara set power:

| Method | Soft Start? | Kapan Digunakan |
|:-------|:------------|:----------------|
| `setPower(pwm)` | ✅ Ya — ramp bertahap | Saat start cleaning atau ganti mode (ECO → STRONG) |
| `setPowerDirect(pwm)` | ❌ Tidak — langsung | **Emergency stop** (cliff, auto-stop baterai) |
| `stop()` | ❌ Tidak — langsung 0 | Stop motor (keselamatan lebih penting dari komponen) |

**Prinsip:** Saat nyala → pelan-pelan (soft start). Saat mati/darurat → **langsung** (safety first).

### 14.8 Simulasi Timing

**Contoh: Mode NORMAL (target PWM = 200)**
```
Config: SOFT_START_RAMP_STEP = 5, SOFT_START_RAMP_INTERVAL = 10ms

Step  0:  PWM =   0  (t = 0ms)
Step  1:  PWM =   5  (t = 10ms)
Step  2:  PWM =  10  (t = 20ms)
Step  3:  PWM =  15  (t = 30ms)
...
Step 40:  PWM = 200  (t = 400ms) ← Ramp complete!

Total waktu = (200 / 5) × 10 = 400ms
```

**Mode STRONG (target PWM = 255):**
```
Total waktu = (255 / 5) × 10 = 510ms
```

**Mode ECO (target PWM = 150):**
```
Total waktu = (150 / 5) × 10 = 300ms
```

**Ganti mode saat jalan (ECO → STRONG, dari PWM 150 ke 255):**
```
Total waktu = ((255 - 150) / 5) × 10 = 210ms
```

### 14.9 Rumus Waktu Ramp

```
Waktu_Ramp (ms) = (|PWM_target - PWM_current| / SOFT_START_RAMP_STEP) × SOFT_START_RAMP_INTERVAL
```

### 14.10 Parameter Soft Start

| Parameter | Nilai | Lokasi | Fungsi |
|:----------|:------|:-------|:-------|
| `SOFT_START_RAMP_STEP` | 5 | `config.h` | Kenaikan PWM per step (1-50) |
| `SOFT_START_RAMP_INTERVAL` | 10 ms | `config.h` | Jeda antar step |

**Catatan tuning:**
- **Ramp terlalu cepat** (step besar, interval kecil) → inrush current masih signifikan
- **Ramp terlalu lambat** (step kecil, interval besar) → robot terasa lambat merespons
- **Nilai saat ini (5 PWM / 10ms ≈ 400ms)** adalah kompromi yang baik antara perlindungan dan responsivitas

### 14.11 File Terkait Soft Start

| File | Lokasi | Peran |
|:-----|:-------|:------|
| `VacuumMotor.h` | `firmware/VacuumRobot/` | Header class motor vakum + deklarasi soft start |
| `VacuumMotor.cpp` | `firmware/VacuumRobot/` | Implementasi setPower(), updateSoftStart(), setPowerDirect() |
| `BrushMotor.h` | `firmware/VacuumRobot/` | Header class motor brush + deklarasi soft start |
| `BrushMotor.cpp` | `firmware/VacuumRobot/` | Implementasi forward(), reverse(), updateSoftStart() |
| `RobotController.cpp` | `firmware/VacuumRobot/` | Memanggil updateSoftStart() setiap loop cycle |
| `config.h` | `firmware/VacuumRobot/` | Parameter SOFT_START_RAMP_STEP dan SOFT_START_RAMP_INTERVAL |
