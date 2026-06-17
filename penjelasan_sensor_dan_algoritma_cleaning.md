# Dokumentasi Sistem Sensor & Algoritma Cleaning

## 1. Pendahuluan

Sistem sensor dan algoritma cleaning pada robot vacuum otonom menggunakan 6 sensor infrared (IR) untuk mendeteksi halangan dan jurang, serta algoritma **Random Bounce + Spiral** untuk membersihkan ruangan secara otonom. Algoritma ini terinspirasi dari pola pembersihan reaktif iRobot Roomba 500-series.

---

## 2. Spesifikasi Hardware Sensor

### 2.1 Sensor Obstacle Avoidance (3 sensor)

Sensor IR yang mendeteksi objek padat (dinding, furniture, dll) di depan dan samping robot.

| Sensor | Label | Pin ESP32 | Posisi |
|:-------|:------|:----------|:-------|
| IR1 | PIN_IR_LEFT | IO33 | Depan-kiri |
| IR2 | PIN_IR_FRONT | IO34 | Depan-tengah |
| IR3 | PIN_IR_RIGHT | IO18 | Depan-kanan |

**Tipe Output:** Active Low
- `LOW` (0) = Ada halangan (LED sensor menyala)
- `HIGH` (1) = Tidak ada halangan (LED sensor mati)

**Konversi di firmware:**
```
rawReading = (digitalRead(PIN) == LOW)  →  true = ada obstacle
```

### 2.2 Sensor Cliff Detection (3 sensor)

Sensor IR yang menghadap ke bawah untuk mendeteksi jurang atau tepi meja.

| Sensor | Label | Pin ESP32 | Posisi |
|:-------|:------|:----------|:-------|
| IR4 | PIN_CLIFF_LEFT | IO19 | Bawah-kiri |
| IR5 | PIN_CLIFF_FRONT | IO21 | Bawah-tengah |
| IR6 | PIN_CLIFF_RIGHT | IO22 | Bawah-kanan |

**Tipe Output:** Active High
- `HIGH` (1) = Jurang terdeteksi (tidak ada pantulan dari lantai)
- `LOW` (0) = Lantai ada (IR terpantul dari permukaan)

**Konversi di firmware:**
```
rawReading = (digitalRead(PIN) == HIGH)  →  true = ada cliff
```

### 2.3 Layout Sensor pada Robot

```
            ┌─────────────────────┐
            │    IR2 (Depan)      │  ← Obstacle sensor
  IR1 (Kiri)│                     │IR3 (Kanan)
            │     ROBOT BODY      │
            │                     │
            │ IR4    IR5    IR6   │  ← Cliff sensor (menghadap bawah)
            └─────────────────────┘
                    ▲ Arah maju
```

---

## 3. Algoritma Debounce Sensor

### 3.1 Tujuan
Menghindari pembacaan sensor palsu akibat noise listrik atau objek yang hanya sekilas terdeteksi. Sensor harus membaca kondisi yang sama secara **berturut-turut** sebelum mengubah state.

### 3.2 Mekanisme
Setiap sensor memiliki **counter** dan **state** tersendiri. Logika debounce bersifat simetris (sama untuk ON dan OFF):

```
Fungsi: _debounce(rawReading, counter, state)

Jika rawReading == true (terdeteksi):
    counter++ (maksimum SENSOR_DEBOUNCE_COUNT)
    Jika counter >= SENSOR_DEBOUNCE_COUNT → state = true

Jika rawReading == false (tidak terdeteksi):
    counter-- (minimum 0)
    Jika counter <= 0 → state = false
```

### 3.3 Parameter
```
SENSOR_DEBOUNCE_COUNT = 2
```

### 3.4 Contoh Timeline
```
Cycle:   1    2    3    4    5    6    7    8
Raw:     0    1    1    1    0    0    0    1
Counter: 0    1    2    2    1    0    0    1
State:   F    F    T    T    T    F    F    F
                   ↑                ↑
            State ON (2 kali)  State OFF (2 kali)
```

State hanya berubah setelah counter mencapai threshold (2), mencegah perubahan state karena noise sesaat.

---

## 4. Algoritma Cleaning

### 4.1 Gambaran Umum
Algoritma menggunakan pendekatan **reaktif** (bukan pemetaan). Robot tidak membangun peta ruangan, melainkan bereaksi terhadap sensor secara real-time. Strategi ini terdiri dari 2 fase:

1. **Fase 1: Spiral Expanding** — Membersihkan area lokal dengan pola spiral mengembang
2. **Fase 2: Random Bounce** — Menjelajah ruangan dengan memantul acak dari halangan

**Random Number Generator:** Algoritma menggunakan `esp_random()` (hardware RNG bawaan ESP32) untuk seed acak. Tidak menggunakan `analogRead(0)` karena GPIO 0 adalah pin tombol WiFi Reset — memanggil `analogRead()` pada pin tersebut akan mengubah mode pin ke analog dan merusak fungsi `digitalRead()` di `checkResetButton()`.

### 4.2 State Machine

Seluruh logika dikelola oleh **Finite State Machine (FSM)** dengan 6 state:

```
┌──────────────────────────────────────────────────────────────────┐
│                                                                  │
│   ┌────────────┐                                                 │
│   │ CLEAN_IDLE │ ──── start() ────▶ ┌──────────────┐            │
│   └────────────┘                    │ CLEAN_SPIRAL │            │
│         ▲                           └──────┬───────┘            │
│         │                                  │                     │
│      stop()                    obstacle atau max duration        │
│         │                        ┌─────────┴──────────┐          │
│         │                        │                    │          │
│         │                    obstacle             max duration   │
│         │                        │                    │          │
│         │                        ▼                    ▼          │
│         │            ┌────────────────────┐  ┌───────────────┐   │
│         │            │ CLEAN_BACKUP_      │  │ CLEAN_FORWARD │   │
│         │            │     OBSTACLE       │  └───────┬───────┘   │
│         │            └────────┬───────────┘          │           │
│         │                     │               obstacle detected  │
│         │            mundur selesai                   │           │
│         │                     │                      ▼           │
│         │                     │         ┌────────────────────┐   │
│         │                     │         │ CLEAN_BACKUP_      │   │
│         │                     │         │     OBSTACLE       │   │
│         │                     │         └────────┬───────────┘   │
│         │                     │                  │               │
│         │                     └────────┬─────────┘               │
│         │                              │                         │
│         │                              ▼                         │
│         │                      ┌──────────────┐                  │
│         ├──────────────────────│ CLEAN_TURNING │                 │
│         │                      └──────┬───────┘                  │
│         │                             │                          │
│         │                  ┌──────────┴──────────┐               │
│         │                  │                     │               │
│         │           depan clear           depan blocked          │
│         │                  │                     │               │
│         │                  ▼                     ▼               │
│         │          CLEAN_FORWARD      CLEAN_BACKUP_OBSTACLE     │
│         │                                                        │
│   ════════════════ CLIFF EMERGENCY (prioritas tertinggi) ═══════ │
│         │                                                        │
│         │    Cliff terdeteksi di state MANAPUN                    │
│         │    (kecuali CLEAN_BACKUP_CLIFF):                        │
│         │                      │                                 │
│         │                      ▼                                 │
│         │             ┌─────────────────┐                        │
│         │             │ CLEAN_BACKUP_   │                        │
│         │             │     CLIFF       │                        │
│         │             └────────┬────────┘                        │
│         │                      │                                 │
│         │                      ▼                                 │
│         │              CLEAN_TURNING (180°)                      │
│         │                      │                                 │
│         │                      ▼                                 │
│         │              CLEAN_FORWARD                             │
│         │                                                        │
└──────────────────────────────────────────────────────────────────┘
```

**Catatan penting pada diagram:**
- SPIRAL juga mengecek obstacle setiap cycle — jika obstacle terdeteksi saat spiral, langsung transisi ke BACKUP_OBSTACLE dan robot berpindah ke fase Random Bounce (tidak kembali ke spiral)
- Cliff emergency **tidak** meng-interrupt state CLEAN_BACKUP_CLIFF (menghindari interrupt ganda)
- Stuck detection terjadi di dalam `_startBackupObstacle()` — jika terdeteksi stuck, langsung skip ke TURNING dengan escape duration (~360°), bypass BACKUP_OBSTACLE

---

## 5. Fase 1: Spiral Expanding

### 5.1 Cara Kerja
Robot bergerak dalam pola spiral yang mengembang — setiap putaran, robot maju lebih jauh sebelum belok lagi. Ini membersihkan area lokal secara efisien.

### 5.2 Alur Per Putaran
```
Putaran 1: Maju 500ms  → Belok 90° (300ms)
Putaran 2: Maju 600ms  → Belok 90° (300ms)    (+100ms)
Putaran 3: Maju 700ms  → Belok 90° (300ms)    (+100ms)
...
Putaran N: Maju 3000ms → Belok 90° (300ms)    (mencapai MAX)
→ Spiral selesai → pindah ke Random Bounce
```

### 5.3 Parameter

| Parameter | Default | Fungsi |
|:----------|:--------|:-------|
| `spiralInitialDuration` | 500ms | Durasi maju pertama kali |
| `spiralIncrement` | 100ms | Penambahan durasi maju setiap putaran |
| `spiralMaxDuration` | 3000ms | Jika durasi maju mencapai ini → spiral selesai |
| `spiralTurnDuration` | 300ms | Durasi belok 90° (tetap setiap putaran) |

### 5.4 Kondisi Keluar Spiral
Spiral berakhir jika salah satu terjadi:
1. **Obstacle terdeteksi** → langsung pindah ke backup + Random Bounce
2. **Durasi maju ≥ spiralMaxDuration** → spiral selesai → Random Bounce

### 5.5 Arah Belok
Arah belok saat spiral **bergantian** (alternating) setiap putaran:
```
Putaran 1: Belok KANAN
Putaran 2: Belok KIRI
Putaran 3: Belok KANAN
...
```

---

## 6. Fase 2: Random Bounce

### 6.1 Cara Kerja
Robot jalan lurus terus-menerus sampai menabrak halangan. Saat halangan terdeteksi, robot mundur, belok ke arah acak, lalu jalan lurus lagi. Pola ini secara statistik akan membersihkan seluruh ruangan jika diberikan waktu cukup.

### 6.2 Alur
```
Jalan lurus → Sensor deteksi obstacle → Berhenti → Mundur → Belok → Jalan lurus → ...
```

---

## 7. Obstacle Avoidance

### 7.1 Prosedur Mundur (CLEAN_BACKUP_OBSTACLE)
Saat obstacle terdeteksi:
1. Robot **berhenti** segera
2. Robot **mundur** selama `backupDuration` (500ms)
3. Setelah mundur selesai, robot menentukan **arah belok** dan **durasi belok**

### 7.2 Algoritma Penentuan Arah Belok

Fungsi `_decideTurnDirection()` menentukan arah belok berdasarkan kombinasi sensor:

| Prioritas | Kondisi Sensor | Keputusan | Alasan |
|:---------:|:---------------|:----------|:-------|
| 1 | Depan + Kiri + Kanan | Random | Pojok mati, tidak ada sisi kosong |
| 2 | Depan + Kiri | Belok KANAN | Sisi kanan kosong |
| 3 | Depan + Kanan | Belok KIRI | Sisi kiri kosong |
| 4 | Kiri + Kanan (depan kosong) | Random | Lorong sempit |
| 5 | Hanya Kiri | Belok KANAN | Menjauhi obstacle |
| 6 | Hanya Kanan | Belok KIRI | Menjauhi obstacle |
| 7 | Hanya Depan | Random | Tidak ada preferensi sisi |

### 7.3 Algoritma Penentuan Durasi Belok

Durasi belok ditentukan berdasarkan seberapa parah kondisi obstacle:

| Kondisi | Durasi | Keterangan |
|:--------|:-------|:-----------|
| Depan + Kiri + Kanan (pojok) | `turnDurationMax` (1000ms) | Putar besar ~180° |
| Depan saja | Random antara `turnDurationMin`–`turnDurationMax` (500–1000ms) | Belok sedang-besar |
| Hanya satu sisi (depan clear) | `turnDurationSmall` (350ms) | Belok kecil ~60° |
| Lainnya | Random antara `turnDurationMin`–`turnDurationMax` | Default |

### 7.4 Perilaku Saat Belok (CLEAN_TURNING)

**Aturan penting:** Robot **TIDAK mengecek sensor obstacle saat sedang belok**. Ini mencegah loop mundur-belok yang berulang. Robot harus menyelesaikan putarannya terlebih dahulu.

Setelah belok selesai:
- Jika **depan masih blocked** → mundur dan belok lagi
- Jika **depan clear** → jalan lurus (CLEAN_FORWARD)

### 7.5 Cliff Safety di Mode Manual

Selain mode autonomous, cliff detection juga aktif di **mode manual** (dikontrol dari website). Implementasi di `RobotController.cpp`:

```cpp
// Manual direction control
sensors.readCliffs();
if (sensors.isCliffDetected()) {
    wheels.stop();
    return;  // Jangan gerak sampai cliff hilang
}
```

**Perilaku:** Jika cliff terdeteksi saat user mengontrol manual, roda **langsung berhenti**. Robot tidak bisa bergerak ke arah manapun selama cliff masih terdeteksi. Berbeda dengan mode autonomous yang melakukan backup+belok otomatis, di mode manual user harus mengangkat robot secara fisik atau menunggu cliff hilang.

---

## 8. Cliff Avoidance

### 8.1 Prioritas Tertinggi
Cliff detection memiliki **prioritas tertinggi** — berjalan di **setiap loop cycle** dan bisa meng-override state apapun kecuali `CLEAN_BACKUP_CLIFF` (menghindari interrupt ganda).

### 8.2 Snapshot Cliff State
Saat cliff pertama terdeteksi, posisi cliff (kiri/depan/kanan) **disimpan sebagai snapshot**:
```cpp
_cliffWasFront = sensors.isCliffFront();
_cliffWasLeft  = sensors.isCliffLeft();
_cliffWasRight = sensors.isCliffRight();
```

**Alasan:** Setelah robot mundur, sensor cliff tidak lagi mendeteksi jurang (karena sudah menjauh). Tanpa snapshot, robot tidak tahu ke arah mana harus belok.

### 8.3 Prosedur
1. Cliff terdeteksi → **berhenti segera**
2. Snapshot disimpan
3. **Mundur** selama `cliffBackupDuration` (600ms) — lebih lama dari obstacle
4. **Belok 180°** selama `cliffTurnDuration` (800ms) berdasarkan snapshot:
   - Cliff di kiri → belok kanan
   - Cliff di kanan → belok kiri
   - Cliff di depan / multi → random
5. Kembali ke CLEAN_FORWARD

### 8.4 Parameter

| Parameter | Default | Fungsi |
|:----------|:--------|:-------|
| `cliffBackupDuration` | 600ms | Durasi mundur (lebih lama dari obstacle) |
| `cliffTurnDuration` | 800ms | Durasi putar 180° |

---

## 9. Stuck Detection & Escape

### 9.1 Masalah
Di ruang sempit atau pojok, robot bisa terjebak dalam pola **mundur-belok-maju-mundur** berulang tanpa bisa lepas.

### 9.2 Mekanisme Deteksi
Setiap kali `_startBackupObstacle()` dipanggil, sistem mencatat:
- **Waktu** obstacle terakhir (`_lastObstacleTime`)
- **Jumlah** obstacle berturut-turut (`_obstacleCount`)

```
Jika obstacle terjadi dalam jendela waktu stuckTimeWindow:
    _obstacleCount++
Jika tidak (lebih dari stuckTimeWindow sejak obstacle terakhir):
    _obstacleCount = 1  (reset)

Jika _obstacleCount >= stuckObstacleCount:
    → STUCK DETECTED! → Escape maneuver
```

### 9.3 Escape Maneuver
Saat stuck terdeteksi:
1. Counter di-reset ke 0
2. Pilih arah acak (kiri/kanan)
3. **Putar besar** selama `escapeTurnDuration` (1500ms, ~360°)
4. Setelah putar selesai → cek depan → lanjut maju atau mundur lagi

### 9.4 Parameter

| Parameter | Default | Fungsi |
|:----------|:--------|:-------|
| `stuckObstacleCount` | 3 | Jumlah obstacle berturut-turut sebelum escape |
| `stuckTimeWindow` | 4000ms | Jendela waktu (3 obstacle dalam 4 detik = stuck) |
| `escapeTurnDuration` | 1500ms | Durasi putar escape (~360°) |

### 9.5 Contoh Skenario
```
t=0s:   Obstacle → mundur → belok → maju          (count=1)
t=1s:   Obstacle → mundur → belok → maju          (count=2)
t=2s:   Obstacle → count=3 → STUCK! → Putar 360°  (escape!)
t=4s:   Maju, berhasil keluar dari pojok
```

---

## 10. Motor dan Pergerakan

### 10.1 Konfigurasi Motor

Roda dikontrol oleh 2 motor DC melalui driver L298N #2. Setiap motor memiliki 2 pin (forward/reverse):

| Motor | Pin FWD | Pin REV |
|:------|:--------|:--------|
| Roda Kiri | IO13 | IO12 |
| Roda Kanan | IO14 | IO15 |

### 10.2 Metode Pergerakan

| Perintah | Roda Kiri | Roda Kanan | Keterangan |
|:---------|:----------|:-----------|:-----------|
| `moveForward()` | Maju | Maju | Kedua roda maju |
| `moveBackward()` | Mundur | Mundur | Kedua roda mundur |
| `turnLeft()` | Mundur | Maju | Putar di tempat (pivot turn) |
| `turnRight()` | Maju | Mundur | Putar di tempat (pivot turn) |
| `stop()` | Stop | Stop | Semua pin = 0 |

### 10.3 PWM
- **Metode:** ESP32 LEDC (LED Control PWM)
- **Frekuensi:** 5000 Hz
- **Resolusi:** 8-bit (0–255)
- **Kecepatan default:** `WHEEL_MOTOR_SPEED = 170`

---

## 11. Runtime Timing Settings

### 11.1 Arsitektur
Semua parameter timing disimpan dalam struct `TimingSettings` yang dapat diubah saat runtime melalui website tanpa flash ulang firmware. Nilai disimpan secara **persisten** di NVS (Non-Volatile Storage) ESP32, sehingga tetap tersimpan meskipun ESP32 di-restart atau mati listrik.

```
config.h (#define)          → Nilai default (compile-time, fallback)
        ↓
TimingSettings.h (struct)   → Variabel runtime (bisa diubah)
        ↓
TimingSettings.cpp          → load() dari NVS saat boot, save() ke NVS saat update
        ↓
CleaningAlgorithm.cpp       → Menggunakan timing.xxx
```

### 11.2 Persistensi (NVS)

Saat **boot** (`setup()`), firmware memanggil `timing.load()` yang membaca nilai dari NVS flash:
```cpp
void TimingSettings::load() {
    timingPrefs.begin("vacuum-timing", true); // read-only
    backupDuration = timingPrefs.getInt("backupDuration", BACKUP_DURATION);
    // ... dst
    timingPrefs.end();
}
```

Parameter kedua `getInt()` (misalnya `BACKUP_DURATION`) adalah **fallback default** dari `config.h` — hanya dipakai jika belum pernah ada data tersimpan di NVS.

Saat **save** dari website, firmware memanggil `timing.save()` yang menulis ke NVS flash:
```cpp
void TimingSettings::save() {
    timingPrefs.begin("vacuum-timing", false); // read-write
    timingPrefs.putInt("backupDuration", backupDuration);
    // ... dst
    timingPrefs.end();
}
```

**Reset ke default:** Fungsi `resetDefaults()` mengembalikan semua nilai ke `#define` di `config.h`, kemudian `save()` menyimpannya ke NVS.

### 11.3 Tabel Lengkap Parameter

| Parameter | Default | Min | Max | Satuan | Fungsi |
|:----------|:--------|:----|:----|:-------|:-------|
| `backupDuration` | 500 | 100 | 2000 | ms | Mundur saat obstacle |
| `turnDurationMin` | 500 | 100 | 2000 | ms | Belok minimum (~90°) |
| `turnDurationMax` | 1000 | 200 | 3000 | ms | Belok maksimum (~180°) |
| `turnDurationSmall` | 350 | 50 | 1000 | ms | Belok kecil (~60°) |
| `cliffBackupDuration` | 600 | 200 | 2000 | ms | Mundur saat cliff |
| `cliffTurnDuration` | 800 | 300 | 3000 | ms | Putar 180° saat cliff |
| `spiralInitialDuration` | 500 | 200 | 2000 | ms | Maju awal spiral |
| `spiralIncrement` | 100 | 50 | 500 | ms | Penambahan per putaran |
| `spiralMaxDuration` | 3000 | 1000 | 10000 | ms | Batas akhir spiral |
| `spiralTurnDuration` | 300 | 100 | 1000 | ms | Belok 90° saat spiral |
| `stuckObstacleCount` | 3 | 2 | 10 | kali | Threshold stuck |
| `stuckTimeWindow` | 4000 | 1000 | 10000 | ms | Jendela waktu stuck |
| `escapeTurnDuration` | 1500 | 500 | 3000 | ms | Putar escape (~360°) |

---

## 12. Alur Eksekusi Lengkap

### 12.1 Dari Start Sampai Berjalan

```
User tekan START di website (Dashboard)
        │
        ▼
Browser POST http://<ESP32_IP>/command → {"command": "start"}
        │
        ▼
ESP32 WebServer menerima → ApiClient::_handleCommand()
        │
        ├── Cek baterai: battery.canStart()?  (lihat Battery Protection)
        │   ├── ≤ 10% → BLOCKED (HTTP 400, "Battery too low")
        │   └── > 10% → Lanjut ↓
        │
        ├── Set lastState = "working"
        ├── Set lastDirection = "autonomous"
        └── HTTP 200 OK
        │
        ▼
RobotController::update() (loop berikutnya)
        │
        ├── vacuum.updateSoftStart()  ← Ramp PWM (Soft Start)
        ├── brush.updateSoftStart()   ← Ramp PWM (Soft Start)
        │
        ├── Battery Protection check  ← Cek 0%, 15%
        │
        ├── targetState == "working":
        │   ├── vacuum.setPower(200)   ← Set target (soft start ramp)
        │   ├── brush.forward()        ← Set target (soft start ramp)
        │   └── cleaner.start()        ← Algoritma cleaning mulai
        │           │
        │           ▼
        │   State = CLEAN_SPIRAL
        │           │
        │           ▼
        │   Loop utama (setiap ~10ms):
        │   ┌─────────────────────────────────────────────┐
        │   │  1. vacuum.updateSoftStart()                 │
        │   │  2. brush.updateSoftStart()                  │
        │   │  3. sensors.readObstacles()                  │
        │   │  4. sensors.readCliffs()                     │
        │   │  5. _checkCliffEmergency() ← Prioritas #1   │
        │   │  6. switch(_state):                          │
        │   │     - SPIRAL → _handleSpiral()               │
        │   │     - FORWARD → _handleForward()             │
        │   │     - BACKUP_OBSTACLE → _handleBackup...()   │
        │   │     - BACKUP_CLIFF → _handleBackupCliff()    │
        │   │     - TURNING → _handleTurning()             │
        │   └─────────────────────────────────────────────┘
        │
        └── targetState != "working":
            └── stopAll() → wheels.stop(), brush.stop(), vacuum.stop()
```

**Catatan:** Komunikasi menggunakan **Direct HTTP** — browser langsung POST ke ESP32 WebServer, bukan melalui Laravel backend. Laravel hanya digunakan untuk penyimpanan log, data baterai, dan histori command.

### 12.2 Siklus Loop Sensor
```
Setiap 10ms (delay di loop utama):
    ┌──────────────────────────────┐
    │ readObstacles()              │
    │  ├── digitalRead(IO33) → IR1 │
    │  ├── digitalRead(IO34) → IR2 │
    │  ├── digitalRead(IO18) → IR3 │
    │  └── debounce × 3            │
    │                              │
    │ readCliffs()                 │
    │  ├── digitalRead(IO19) → IR4 │
    │  ├── digitalRead(IO21) → IR5 │
    │  ├── digitalRead(IO22) → IR6 │
    │  └── debounce × 3            │
    └──────────────────────────────┘
```

---

## 13. File Terkait

| File | Lokasi | Peran |
|:-----|:-------|:------|
| `config.h` | `firmware/VacuumRobot/` | Pin definitions, timing defaults, motor speeds, soft start params |
| `SensorArray.h` | `firmware/VacuumRobot/` | Header class sensor: debounce vars, getters |
| `SensorArray.cpp` | `firmware/VacuumRobot/` | Implementasi pembacaan sensor + debounce |
| `CleaningAlgorithm.h` | `firmware/VacuumRobot/` | Header class: state machine, FSM states, enums |
| `CleaningAlgorithm.cpp` | `firmware/VacuumRobot/` | Implementasi lengkap algoritma cleaning (spiral, bounce, stuck) |
| `TimingSettings.h` | `firmware/VacuumRobot/` | Struct runtime timing settings + resetDefaults() |
| `TimingSettings.cpp` | `firmware/VacuumRobot/` | Persistensi NVS: load() saat boot, save() saat update dari web |
| `RobotController.h` | `firmware/VacuumRobot/` | Header class controller: mode manual + autonomous |
| `RobotController.cpp` | `firmware/VacuumRobot/` | Logika utama: mode switching, battery protection, cliff safety manual |
| `WheelMotor.h` | `firmware/VacuumRobot/` | Header class motor roda |
| `WheelMotor.cpp` | `firmware/VacuumRobot/` | Implementasi kontrol motor (PWM, arah, pivot turn) |
| `ApiClient.cpp` | `firmware/VacuumRobot/` | Direct HTTP server: /command, /diagnostic, /settings |
| `VacuumRobot.ino` | `firmware/VacuumRobot/` | Entry point, global objects, loop() timing |
| `diagnostic.blade.php` | `resources/views/` | UI website: sensor monitoring + timing settings editor |
