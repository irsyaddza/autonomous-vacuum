#ifndef CONFIG_H
#define CONFIG_H

// =====================================================
// MOTOR DRIVER 1 (BRUSH + VACUUM)
// =====================================================

// Brush Motor
#define PIN_BRUSH_FWD       25
#define PIN_BRUSH_REV       23

// Vacuum Motor
#define PIN_VACUUM_PWM_1    26
#define PIN_VACUUM_PWM_2    32

// ===== MOTOR DRIVER 2 (L298N #2) - DRIVE WHEELS =====
// Motor Roda - dikontrol dari website (maju, mundur, belok kiri, belok kanan)
// Motor Roda Kiri terhubung ke OUT1 & OUT2
#define PIN_WHEEL_LEFT_FWD   12  // IO13 → IN2 (Left Wheel Forward) [swapped]
#define PIN_WHEEL_LEFT_REV   13  // IO12 → IN1 (Left Wheel Reverse) [swapped]
// Motor Roda Kanan terhubung ke OUT3 & OUT4
#define PIN_WHEEL_RIGHT_FWD  14  // IO15 → IN4 (Right Wheel Forward) [swapped]
#define PIN_WHEEL_RIGHT_REV  15  // IO14 → IN3 (Right Wheel Reverse) [swapped]

// Left Wheel
#define PIN_WHEEL_LEFT_FWD   12
#define PIN_WHEEL_LEFT_REV   13

// Right Wheel
#define PIN_WHEEL_RIGHT_FWD  14
#define PIN_WHEEL_RIGHT_REV  15

// =====================================================
// IR OBSTACLE SENSORS
// =====================================================

#define PIN_OB_LEFT         33
#define PIN_OB_FRONT        34
#define PIN_OB_RIGHT        18

// =====================================================
// IR CLIFF SENSORS
// =====================================================

#define PIN_CLIFF_LEFT      19
#define PIN_CLIFF_FRONT     21
#define PIN_CLIFF_RIGHT     22

// =====================================================
// BATTERY
// =====================================================

#define PIN_BATTERY_ADC     36

// =====================================================
// BUTTON & BUZZER
// =====================================================

#define PIN_WIFI_RESET      4
#define PIN_BUZZER          27

// =====================================================
// WIFI
// =====================================================

#define WIFI_AP_NAME        "VacuumRobot"
#define WIFI_AP_PASSWORD    "VacuumRobot123"

// =====================================================
// API
// =====================================================

#define DEFAULT_API_BASE_URL "http://192.168.1.2:8000/v1/vacuum"
#define BATTERY_SEND_INTERVAL       120000  // ms - kirim data battery ke server (saat aktif/working)
#define BATTERY_SEND_INTERVAL_IDLE  300000 // ms - kirim data battery saat standby (5 menit, hemat baterai)

// ===== DIRECT HTTP SERVER CONFIG =====
#define ESP32_HTTP_PORT     80      // Port for receiving direct commands from browser
#define FIRMWARE_VERSION    "2.2.5" // Firmware version for device registration

// ===== MAIN LOOP TIMING =====
#define ACTIVE_LOOP_DELAY   10      // ms - delay loop saat robot aktif (working)
#define IDLE_LOOP_DELAY     50      // ms - delay loop saat standby (hemat CPU & baterai)

// ===== MOTOR SETTINGS =====
#define VACUUM_POWER_NORMAL 200     // Default PWM untuk vacuum NORMAL mode (0-255)
#define BRUSH_SPEED         150     // PWM untuk brush motor (0-255)
#define WHEEL_MOTOR_SPEED   170     // Default PWM untuk drive wheel motor (0-255)
#define WHEEL_LEFT_SPEED    210     // Default PWM roda kiri (tune jika robot menyerong)
#define WHEEL_RIGHT_SPEED   170     // Default PWM roda kanan (tune jika robot menyerong)

#define ESP32_HTTP_PORT     80
#define FIRMWARE_VERSION    "2.1.0"

// =====================================================
// MOTOR SETTINGS
// =====================================================

#define VACUUM_POWER_NORMAL 200
#define BRUSH_SPEED         150
#define WHEEL_MOTOR_SPEED   170

// =====================================================
// NAVIGATION SETTINGS
// =====================================================

#define TURN_DELAY          450
#define BACKWARD_DELAY      400

#define LOOP_TIMEOUT        3000
#define ESCAPE_DELAY        1000

// =====================================================
// HOMING SETTINGS
// =====================================================

#define LOW_BATTERY_PERCENT 20

// =====================================================
// CLEANING ALGORITHM SETTINGS
// =====================================================
// Obstacle avoidance timing
#define BACKUP_DURATION         600     // ms - durasi mundur saat ada obstacle
#define TURN_DURATION_MIN       3700     // ms - durasi belok minimum (~90°)
#define TURN_DURATION_MAX       7400    // ms - durasi belok maksimum (~180°)
#define TURN_DURATION_SMALL     1850     // ms - belok kecil (~60°) untuk obstacle samping

// Cliff avoidance timing (prioritas lebih tinggi)
#define CLIFF_BACKUP_DURATION   1500     // ms - durasi mundur saat cliff (lebih lama)
#define CLIFF_TURN_DURATION     7400     // ms - durasi putar 180° saat cliff

// Spiral pattern settings
#define SPIRAL_INITIAL_DURATION 3000     // ms - durasi lurus awal spiral
#define SPIRAL_INCREMENT        2000     // ms - penambahan durasi setiap putaran spiral
#define SPIRAL_MAX_DURATION     3000000    // ms - durasi lurus maksimum sebelum pindah ke random bounce
#define SPIRAL_TURN_DURATION    3700     // ms - durasi belok 90° saat spiral

// Re-spiral trigger
#define RESPIRAL_IDLE_TIME      30000    // ms - durasi tanpa obstacle di open area sebelum re-trigger spiral

// Stuck detection
#define STUCK_OBSTACLE_COUNT    3       // Jumlah obstacle berturut-turut sebelum escape
#define STUCK_TIME_WINDOW       4000    // ms - jendela waktu untuk stuck detection
#define ESCAPE_TURN_DURATION    7400    // ms - durasi putar escape 

// Sensor debounce
#define SENSOR_DEBOUNCE_COUNT   2       // Jumlah consecutive reads untuk konfirmasi sensor

// =====================================================
// SOFT START SETTINGS
// =====================================================
// PWM dinaikkan bertahap saat motor mulai (mengurangi inrush current)
#define SOFT_START_RAMP_STEP       5    // PWM increment per step (1-50)
#define SOFT_START_RAMP_INTERVAL   10   // ms between steps (~500ms full ramp 0->255)
// Catatan: Total waktu ramp ≈ (target_pwm / RAMP_STEP) * RAMP_INTERVAL
// Contoh Normal mode: (200 / 5) * 10 = 400ms

#endif