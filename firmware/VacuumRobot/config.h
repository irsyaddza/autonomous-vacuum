#ifndef CONFIG_H
#define CONFIG_H

// ===== PIN DEFINITIONS =====

// ===== MOTOR DRIVER 1 (L298N #1) - BRUSH & VACUUM =====
// Motor Sapu (Brush) terhubung ke OUT1 & OUT2
#define PIN_BRUSH_FWD       23  // IO23 → IN1 (Brush Motor Forward)
#define PIN_BRUSH_REV       25  // IO25 → IN2 (Brush Motor Reverse)

// Motor Vakum terhubung ke OUT3 & OUT4
// PWM value diambil dari website (eco: 150, normal: 200, strong: 255)
#define PIN_VACUUM_PWM_1    26  // IO26 → IN3 (Vacuum Motor Speed)
#define PIN_VACUUM_PWM_2    32  // IO32 → IN4 (Vacuum Motor Direction)

// ===== MOTOR DRIVER 2 (L298N #2) - DRIVE WHEELS =====
// Motor Roda - dikontrol dari website (maju, mundur, belok kiri, belok kanan)
// Motor Roda Kiri terhubung ke OUT1 & OUT2
#define PIN_WHEEL_LEFT_FWD   12  // IO12 → IN1 (Left Wheel Forward)
#define PIN_WHEEL_LEFT_REV   13  // IO13 → IN2 (Left Wheel Reverse)
// Motor Roda Kanan terhubung ke OUT3 & OUT4
#define PIN_WHEEL_RIGHT_FWD  14  // IO14 → IN3 (Right Wheel Forward)
#define PIN_WHEEL_RIGHT_REV  15  // IO15 → IN4 (Right Wheel Reverse)

// Sensors (IR Obstacle Avoidance) - Digital Input
#define PIN_IR_LEFT         16  // IO16 → IR1 Kiri
#define PIN_IR_FRONT        17  // IO17 → IR2 Tengah
#define PIN_IR_RIGHT        18  // IO18 → IR3 Kanan

// Sensors (IR Cliff Detection) - Digital Input
#define PIN_CLIFF_LEFT      19  // IO19 → IR4 Kiri
#define PIN_CLIFF_FRONT     21  // IO21 → IR5 Tengah
#define PIN_CLIFF_RIGHT     22  // IO22 → IR6 Kanan

// Battery (Voltage Divider) - Analog Input
#define PIN_BATTERY_ADC     36  // VP pin

// Function Buttons
#define PIN_WIFI_RESET      0   // Tombol BOOT bawaan ESP32. Tekan tahan 5 detik.

// Buzzer (Active Buzzer)
#define PIN_BUZZER          27  // IO27 → Buzzer untuk feedback WiFi reset

// ===== WIFI AP CONFIG =====
// Nama dan password WiFi Access Point saat mode setup
#define WIFI_AP_NAME        "VacuumRobot"       // Ganti sesuai keinginan
#define WIFI_AP_PASSWORD    "VacuumRobot123"    // Minimal 8 karakter

// ===== API CONFIG =====
#define API_BASE_URL        "http://10.253.103.67:8000/v1/vacuum"
#define API_POLL_INTERVAL   2000    // ms - polling status dari server
#define BATTERY_SEND_INTERVAL 60000 // ms - kirim data battery ke server

// ===== MOTOR SETTINGS =====
#define VACUUM_POWER_NORMAL 200     // Default PWM untuk vacuum NORMAL mode (0-255)
#define BRUSH_SPEED         150     // PWM untuk brush motor (0-255)
#define WHEEL_MOTOR_SPEED   170     // Default PWM untuk drive wheel motor (0-255)

#endif
