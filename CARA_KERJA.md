# Cara Kerja Robot Vacuum Cleaner (Autonomous Vacuum)

Dokumen ini menjelaskan alur kerja, algoritma pembersihan, dan logika navigasi robot vacuum cleaner berdasarkan kode sumber (*source code*) firmware ESP32 yang ada di dalam repositori ini.

Secara umum, terdapat **dua algoritma pembersihan** yang didefinisikan dalam kode:
1. **Algoritma Wall Following (Penyusuran Dinding Kiri)** – Diimplementasikan di dalam [RobotController.cpp](file:///c:/laragon/www/autonomous-vacuum/firmware/VacuumRobot/RobotController.cpp).
2. **Algoritma Spiral + Random Bounce (Pola iRobot Roomba)** – Diimplementasikan di dalam [CleaningAlgorithm.cpp](file:///c:/laragon/www/autonomous-vacuum/firmware/VacuumRobot/CleaningAlgorithm.cpp).

---

## 1. Saat Robot Mulai Dijalankan (Start)
Ketika robot menerima perintah untuk mulai membersihkan (`targetState == "working"`):
- Motor penyedot (*vacuum motor*) akan menyala sesuai tingkat daya yang dipilih melalui API: **ECO** (PWM 150), **NORMAL** (PWM 200), atau **STRONG** (PWM 255) yang dikonfigurasi pada [config.h](file:///c:/laragon/www/autonomous-vacuum/firmware/VacuumRobot/config.h).
- Motor sikat (*brush motor*) akan berputar maju (`brush.forward()`).
- Perilaku pergerakan robot pada saat pertama kali berjalan bergantung pada algoritma mana yang aktif/dipanggil:

### A. Jika Menggunakan Algoritma Utama Saat Ini ([RobotController.cpp](file:///c:/laragon/www/autonomous-vacuum/firmware/VacuumRobot/RobotController.cpp))
Pergerakan dimulai dari **Pinggir ke Tengah (atau mengikuti dinding kiri)**:
1. Robot langsung memeriksa sensor rintangan inframerah di sebelah kiri (`sensors.isLeftBlocked()`).
2. Jika tidak ada dinding di sebelah kiri robot, robot akan langsung bergerak maju secara lurus (`wheels.moveForward()`). Karena tidak ada dinding penahan di kiri, pergerakan maju ini akan cenderung mengarah ke kiri hingga ia mendeteksi adanya dinding di sisi kirinya.
3. Begitu dinding kiri terdeteksi, robot akan berbelok ke kanan sedikit (selama 100 milidetik) untuk menjaga posisinya tetap sejajar dan tidak menabrak dinding kiri tersebut, lalu melanjutkan maju.
4. Pola ini dinamakan **Left Wall Following**, di mana robot berputar menyusuri batas luar ruangan (pinggir ruangan) terlebih dahulu sebelum akhirnya berbelok jika menemui sudut atau rintangan depan.

### B. Jika Menggunakan Algoritma Tambahan/Alternatif ([CleaningAlgorithm.cpp](file:///c:/laragon/www/autonomous-vacuum/firmware/VacuumRobot/CleaningAlgorithm.cpp))
Pergerakan dimulai dari **Tengah ke Pinggir (Spiral Expanding)**:
1. Ketika pertama kali dimulai (`start()`), robot langsung masuk ke **Fase 1: Spiral Expanding (CLEAN_SPIRAL)**. Ini adalah pola pembersihan yang dimulai dari titik tengah (tempat robot diletakkan) dan mengembang keluar ke area pinggir.
2. Robot akan bergerak lurus ke depan selama durasi tertentu (`_spiralForwardDuration` dimulai dari `SPIRAL_INITIAL_DURATION` = 500 ms).
3. Setelah waktu habis, robot akan berbelok 90 derajat selama `SPIRAL_TURN_DURATION` (300 ms).
4. Robot kemudian bergerak lurus lagi dengan durasi yang lebih lama (ditambah `SPIRAL_INCREMENT` = 100 ms setiap kali berputar).
5. Proses ini berulang secara bergantian sehingga radius area yang dilewati semakin lama semakin luas (membentuk pola spiral/kotak mengembang dari tengah ke pinggir).
6. Fase spiral ini akan selesai dan berganti ke fase **Random Bounce** jika:
   - Durasi jalan lurus telah mencapai batas maksimal (`SPIRAL_MAX_DURATION` = 3000 ms).
   - Robot mendeteksi adanya rintangan sebelum spiral selesai.

---

## 2. Navigasi & Penghindaran Rintangan (Obstacle Avoidance)

Jika robot mendeteksi adanya halangan di depannya saat sedang berjalan:

### A. Pada Logika [RobotController.cpp](file:///c:/laragon/www/autonomous-vacuum/firmware/VacuumRobot/RobotController.cpp) (Wall Following)
- Robot akan berhenti, lalu bergerak mundur selama `BACKWARD_DELAY` (400 ms).
- Robot akan mengevaluasi sensor kiri dan kanan untuk memutuskan arah putaran:
  - Jika sisi kiri terblokir tetapi kanan aman: Belok kanan.
  - Jika sisi kanan terblokir tetapi kiri aman: Belok kiri.
  - Jika kedua sisi terblokir atau kosong: Default berbelok ke kiri (agar tetap sinkron dengan metode menyusuri dinding kiri).
- **Deteksi Anti-Loop (Terjebak):** Jika robot mendeteksi rintangan depan terus-menerus selama lebih dari `LOOP_TIMEOUT` (3 detik), robot akan masuk ke **Escape Mode**. Dalam mode ini, robot akan berbelok ke kiri dan ke kanan secara bergantian selama `ESCAPE_DELAY` (1000 ms) untuk melarikan diri dari jebakan sudut mati.

### B. Pada Logika [CleaningAlgorithm.cpp](file:///c:/laragon/www/autonomous-vacuum/firmware/VacuumRobot/CleaningAlgorithm.cpp) (Random Bounce)
Setelah keluar dari Fase Spiral (atau menabrak halangan saat spiral), robot masuk ke **Fase 2: Random Bounce (CLEAN_FORWARD)**.
- Robot berjalan lurus secara acak hingga mendeteksi rintangan.
- Begitu rintangan terdeteksi, robot langsung mundur selama `BACKUP_DURATION` (500 ms) lalu masuk ke mode berbelok (`CLEAN_TURNING`).
- **Arah Belok Cerdas:**
  - Terblokir depan + kiri: Belok kanan.
  - Terblokir depan + kanan: Belok kiri.
  - Terblokir semua sisi (sudut mati): Arah belok dipilih secara acak (kiri/kanan) dengan durasi putar maksimal (`TURN_DURATION_MAX` = 1000 ms).
  - Terblokir hanya satu sisi: Belok menjauhi rintangan dengan durasi belok kecil (`TURN_DURATION_SMALL` = 350 ms).
- **Deteksi Terjebak (Stuck Detection):** Jika robot mendeteksi `stuckObstacleCount` (3 kali) tabrakan/rintangan dalam rentang waktu `stuckTimeWindow` (4 detik), robot dianggap terjebak. Robot akan langsung melakukan putaran penyelamatan (*escape turn*) yang besar sekitar 360 derajat selama `ESCAPE_TURN_DURATION` (1500 ms) untuk membebaskan diri.

---

## 3. Sistem Keamanan Tebing (Cliff Safety)
Keamanan tebing memiliki **prioritas tertinggi** di kedua algoritma untuk mencegah robot jatuh dari tangga atau area tinggi:
- **Deteksi:** Sensor inframerah tebing (`Cliff Sensors`) di kiri, depan, dan kanan terus-menerus dipindai.
- **Tindakan Darurat:**
  - Robot segera menghentikan rodanya.
  - Bergerak mundur (selama `BACKWARD_DELAY` = 400 ms pada `RobotController` atau `CLIFF_BACKUP_DURATION` = 600 ms pada `CleaningAlgorithm`).
  - Berbelok menjauhi tebing berdasarkan sensor yang mendeteksi:
    - Tebing di kiri: Belok kanan.
    - Tebing di kanan: Belok kiri.
    - Tebing di depan: Belok kanan (default) atau secara acak berputar 180 derajat menjauhi tebing selama `CLIFF_TURN_DURATION` (800 ms).

---

## 4. Perlindungan Baterai (Battery Protection)
Baterai dimonitor secara real-time oleh `BatteryMonitor`:
- **Pengiriman Data:** Persentase dan tegangan baterai dikirim ke Laravel API setiap 60 detik (`BATTERY_SEND_INTERVAL`).
- **Auto-Stop Low Battery:** Jika baterai turun hingga atau di bawah `LOW_BATTERY_PERCENT` (20%), robot secara otomatis mengubah statusnya menjadi `idle`, menghentikan semua motor (roda, sikat, dan vakum), dan mengirim sinyal berhenti ke server. Robot tidak akan diizinkan untuk memulai pembersihan kembali jika baterai berada di bawah `BATTERY_BLOCK_START_PCT` (10%).

---

## Ringkasan Perbandingan Aliran Gerak Awal

| Karakteristik | `RobotController.cpp` (Aktif Saat Ini) | `CleaningAlgorithm.cpp` (Algoritma Cadangan/Alternatif) |
|---|---|---|
| **Pola Awal** | **Menyusuri Dinding Kiri (Edge Following)** | **Spiral Mengembang (Spiral Expanding)** |
| **Arah Gerak Awal** | Pinggir $\rightarrow$ Tengah (Mulai dengan mencari dinding terdekat lalu mengitarinya) | Tengah $\rightarrow$ Pinggir (Mulai spiral dari tengah ruangan hingga menyentuh rintangan) |
| **Navigasi Lanjutan** | Berbelok saat terhalang untuk tetap berada dekat dinding | Memantul secara acak (*Random Bounce*) dengan arah & durasi cerdas |
| **Penanganan Terjebak** | Escape mode bergantian (kiri-kanan) jika stuck > 3 detik | Escape turn 360° jika menabrak 3 kali dalam waktu 4 detik |

---

## 5. Panduan Presentasi Prototipe (PPT) untuk Dosen

Berikut adalah rekomendasi **poin-poin slide (Outline PPT)** beserta materi penjelasannya yang dapat Anda gunakan untuk presentasi di depan dosen:

### Slide 1: Judul Presentasi & Identitas Tim
* **Judul:** Pengembangan Prototipe Smart Autonomous Vacuum Robot Berbasis ESP32 dan Laravel IoT Dashboard
* **Sub-judul:** Sistem navigasi cerdas dengan dual-algorithm pembersihan, pengaman tebing (*cliff detection*), dan monitoring real-time.
* **Poin Penjelasan untuk Dosen:**
  * Jelaskan bahwa proyek ini mengintegrasikan antara *Embedded System* (ESP32) dengan *Web Application* (Laravel) untuk mewujudkan konsep IoT (*Internet of Things*).

### Slide 2: Latar Belakang & Rumusan Masalah
* **Poin Slide:**
  * Kebutuhan akan alat pembersih otomatis yang terjangkau dan dapat dipantau dari jarak jauh.
  * Masalah navigasi robot vacuum murah yang sering terjebak (*stuck*) di sudut ruangan atau terjatuh dari tangga (*cliff*).
  * Perlunya integrasi sistem pemantauan baterai agar robot tidak mati mendadak di tengah ruangan.
* **Poin Penjelasan untuk Dosen:**
  * Prototipe ini dirancang untuk mengatasi kelemahan robot pembersih konvensional melalui algoritma deteksi rintangan aktif, perlindungan dari tangga, dan dashboard pemantauan berbasis web.

### Slide 3: Arsitektur Sistem (Hardware & Software)
* **Poin Slide:**
  * **Hardware (Node):** ESP32, Motor Driver L298N (Roda + Sikat/Vakum), Sensor IR Obstacle & Cliff, Battery Monitor.
  * **Software (Server):** Laravel Backend, MySQL Database, REST API (JSON).
  * **Konektivitas:** Komunikasi nirkabel via WiFi lokal (REST HTTP Requests).
* **Poin Penjelasan untuk Dosen:**
  * Tunjukkan bagaimana ESP32 bertindak sebagai pengendali fisik (*hardware controller*) yang secara periodik mengirimkan status baterai dan meminta instruksi (*polling*) dari Laravel Server.

### Slide 4: Konfigurasi Pinout & Sensor
* **Poin Slide:**
  * **Motor Driver 1:** Brush Motor (GPIO 23, 25) & Vacuum Motor PWM (GPIO 26, 32).
  * **Motor Driver 2 (Wheels):** Left Wheel (GPIO 12, 13) & Right Wheel (GPIO 14, 15).
  * **Sensor Array:** 3x IR Obstacle (Left, Front, Right) & 3x IR Cliff (Left, Front, Right).
* **Poin Penjelasan untuk Dosen:**
  * Sebutkan bahwa sensor diposisikan secara strategis di bagian depan, samping, dan bawah robot untuk memastikan pembacaan lingkungan 360 derajat secara horizontal dan vertikal ke arah bawah.

### Slide 5: Aliran API & Komunikasi Data (IoT)
* **Poin Slide:**
  * Polling Interval: ESP32 melakukan request setiap 2 detik ke `/v1/vacuum/status` untuk mengambil status perintah dari web (working/idle).
  * Telemetri: ESP32 mengirimkan voltase baterai aktual ke server setiap 60 detik.
  * Pengaturan Daya: Mode daya hisap (**ECO**, **NORMAL**, **STRONG**) diatur dari web dan diubah menjadi nilai PWM motor vakum oleh ESP32.
* **Poin Penjelasan untuk Dosen:**
  * Ini menunjukkan implementasi komunikasi dua arah: dari Web ke Robot (kontrol perintah dan daya) dan dari Robot ke Web (telemetri baterai dan status kerja).

### Slide 6: Logika Navigasi 1 - Wall Following (Aktif Saat Ini)
* **Poin Slide:**
  * **Pola Awal:** Menyusuri dinding kiri (Pinggir ke Tengah).
  * **Cara Kerja:** Robot akan berjalan mencari dinding kiri terdekat. Ketika dinding kiri terdeteksi, robot mempertahankan jarak sejajar dengan membelok kanan sedikit demi sedikit (100ms).
  * **Fungsi:** Sangat efektif untuk membersihkan kotoran yang biasanya menumpuk di sepanjang sudut/tepi ruangan.
* **Poin Penjelasan untuk Dosen:**
  * Logika ini ditulis dalam `RobotController.cpp`. Robot menggunakan data dari sensor kiri untuk menjaga jarak paralel dengan dinding.

### Slide 7: Logika Navigasi 2 - Spiral & Random Bounce (Algoritma Cerdas)
* **Poin Slide:**
  * **Fase 1 (Spiral Expanding):** Berputar membentuk lingkaran/kotak yang meluas dari tengah ruangan ke pinggir. Durasi jalan lurus bertambah 100ms setiap segmen.
  * **Fase 2 (Random Bounce):** Bergerak lurus hingga menabrak halangan, kemudian berbelok ke arah yang aman secara acak/cerdas.
  * **Deteksi Terjebak:** Jika menabrak rintangan 3 kali dalam waktu 4 detik, robot otomatis melakukan putaran penyelamatan (*escape turn*) 360°.
* **Poin Penjelasan untuk Dosen:**
  * Pola spiral (tengah ke pinggir) memastikan area awal dibersihkan secara merata sebelum menyebar secara acak ke seluruh ruangan.

### Slide 8: Fitur Keamanan (Safety Features)
* **Poin Slide:**
  * **Deteksi Tebing (Cliff Detection):** Sensor bawah diprogram secara *Active HIGH*. Jika tidak mendeteksi permukaan lantai, robot langsung menghentikan motor, mundur, dan memutar arah 180° menjauhi tebing.
  * **Auto-Stop Low Battery:** Jika baterai $\le 20\%$, robot langsung berhenti darurat dan beralih ke mode `idle` untuk mencegah kerusakan baterai akibat *over-discharge*.
* **Poin Penjelasan untuk Dosen:**
  * Fitur ini menjamin *durabilitas* hardware. Kami memperbaiki logika deteksi tebing menjadi *Active HIGH* agar robot lebih sensitif mendeteksi ketiadaan permukaan jalan.

### Slide 9: Demo Dashboard Web Laravel
* **Poin Slide:**
  * Kontrol Jarak Jauh: Start/Stop dan penggantian Power Mode secara *real-time*.
  * Indikator Baterai: Progres bar baterai dengan kalkulasi sisa waktu kerja berdasarkan mode daya yang digunakan.
  * Log Status: Pencatatan riwayat transisi state robot dari Idle $\leftrightarrow$ Working.
* **Poin Penjelasan untuk Dosen:**
  * Dashboard memberikan kemudahan bagi pengguna akhir (*end-user*) untuk mengendalikan dan memantau kinerja alat dari browser mana pun.

### Slide 10: Kesimpulan & Rencana Pengembangan
* **Poin Slide:**
  * Prototipe berhasil mengintegrasikan kendali hardware ESP32, sensor rintangan & tebing, serta dashboard Laravel.
  * Navigasi dasar menyusuri dinding dan deteksi tebing berjalan sesuai parameter waktu yang diatur.
  * **Rencana Selanjutnya:** Implementasi pemetaan ruangan (*Slam/Mapping*) sederhana dan fitur pengisian daya otomatis (*auto-docking*).
