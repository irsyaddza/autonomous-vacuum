#ifndef CONFIG_H
#define CONFIG_H

// ===== PIN DEFINITIONS =====

// ===== MOTOR DRIVER 1 (L298N #1) - BRUSH & VACUUM =====
// Motor Sapu (Brush) terhubung ke OUT1 & OUT2
#define PIN_BRUSH_FWD       25  // IO23 → IN1 (Brush Motor Forward)
#define PIN_BRUSH_REV       23  // IO25 → IN2 (Brush Motor Reverse)

// Motor Vakum terhubung ke OUT3 & OUT4
// PWM value diambil dari website (eco: 150, normal: 200, strong: 255)
#define PIN_VACUUM_PWM_1    26  // IO26 → IN3
#define PIN_VACUUM_PWM_2    32  // IO32 → IN4

// ===== MOTOR DRIVER 2 (L298N #2) - DRIVE WHEELS =====
// Motor Roda - dikontrol dari website (maju, mundur, belok kiri, belok kanan)
// Motor Roda Kiri terhubung ke OUT1 & OUT2
#define PIN_WHEEL_LEFT_FWD   13  // IO13 → IN2 (Left Wheel Forward) [swapped]
#define PIN_WHEEL_LEFT_REV   12  // IO12 → IN1 (Left Wheel Reverse) [swapped]
// Motor Roda Kanan terhubung ke OUT3 & OUT4
#define PIN_WHEEL_RIGHT_FWD  14  // IO14 → IN3 (Right Wheel Forward)
#define PIN_WHEEL_RIGHT_REV  15  // IO15 → IN4 (Right Wheel Reverse)

// Sensors (IR Obstacle Avoidance) - Digital Input
#define PIN_IR_LEFT         33  // IO33 → IR1 Kiri
#define PIN_IR_FRONT        34  // IO34 → IR2 Tengah
#define PIN_IR_RIGHT        18  // IO18 → IR3 Kanan

// Sensors (IR Cliff Detection) - Digital Input
#define PIN_CLIFF_LEFT      19  // IO19 → IR4 Kiri
#define PIN_CLIFF_FRONT     21  // IO21 → IR5 Tengah
#define PIN_CLIFF_RIGHT     22  // IO22 → IR6 Kanan

// Battery (Voltage Divider) - Analog Input
#define PIN_BATTERY_ADC     36  // VP pin

// Function Buttons
#define PIN_WIFI_RESET      4   // Tombol tambahan untuk ESP32. Tekan tahan 5 detik.

// Buzzer (Active Buzzer)
#define PIN_BUZZER          27  // IO27 → Buzzer

// ===== WIFI AP CONFIG =====
// Nama dan password WiFi Access Point saat mode setup
#define WIFI_AP_NAME        "VacuumRobot"       // Ganti sesuai keinginan
#define WIFI_AP_PASSWORD    "VacuumRobot123"    // Minimal 8 karakter

// ===== API CONFIG =====
#define DEFAULT_API_BASE_URL "http://192.168.1.2:8000/v1/vacuum"
#define API_POLL_INTERVAL   2000    // ms - polling status dari server (fallback only)
#define BATTERY_SEND_INTERVAL 60000 // ms - kirim data battery ke server

// ===== DIRECT HTTP SERVER CONFIG =====
#define ESP32_HTTP_PORT     80      // Port for receiving direct commands from browser
#define FIRMWARE_VERSION    "2.0.0" // Firmware version for device registration

// ===== MOTOR SETTINGS =====
#define VACUUM_POWER_NORMAL 200     // Default PWM untuk vacuum NORMAL mode (0-255)
#define BRUSH_SPEED         150     // PWM untuk brush motor (0-255)
#define WHEEL_MOTOR_SPEED   170     // Default PWM untuk drive wheel motor (0-255)

// ===== CLEANING ALGORITHM SETTINGS =====
// Obstacle avoidance timing
#define BACKUP_DURATION         500     // ms - durasi mundur saat ada obstacle
#define TURN_DURATION_MIN       500     // ms - durasi belok minimum (~90°)
#define TURN_DURATION_MAX       1000    // ms - durasi belok maksimum (~180°)
#define TURN_DURATION_SMALL     350     // ms - belok kecil (~60°) untuk obstacle samping

// Cliff avoidance timing (prioritas lebih tinggi)
#define CLIFF_BACKUP_DURATION   600     // ms - durasi mundur saat cliff (lebih lama)
#define CLIFF_TURN_DURATION     800     // ms - durasi putar 180° saat cliff

// Spiral pattern settings
#define SPIRAL_INITIAL_DURATION 500     // ms - durasi lurus awal spiral
#define SPIRAL_INCREMENT        100     // ms - penambahan durasi setiap putaran spiral
#define SPIRAL_MAX_DURATION     3000    // ms - durasi lurus maksimum sebelum pindah ke random bounce
#define SPIRAL_TURN_DURATION    300     // ms - durasi belok 90° saat spiral

// Stuck detection
#define STUCK_OBSTACLE_COUNT    3       // Jumlah obstacle berturut-turut sebelum escape
#define STUCK_TIME_WINDOW       4000    // ms - jendela waktu untuk stuck detection
#define ESCAPE_TURN_DURATION    1500    // ms - durasi putar escape (~360°)

// Sensor debounce
#define SENSOR_DEBOUNCE_COUNT   2       // Jumlah consecutive reads untuk konfirmasi sensor

// ===== SOFT START SETTINGS =====
// PWM dinaikkan bertahap saat motor mulai (mengurangi inrush current)
#define SOFT_START_RAMP_STEP       5    // PWM increment per step (1-50)
#define SOFT_START_RAMP_INTERVAL   10   // ms between steps (~500ms full ramp 0→255)
// Catatan: Total waktu ramp ≈ (target_pwm / RAMP_STEP) * RAMP_INTERVAL
// Contoh Normal mode: (200 / 5) * 10 = 400ms

#endif
