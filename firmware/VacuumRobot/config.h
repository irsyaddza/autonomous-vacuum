#ifndef CONFIG_H
#define CONFIG_H

// =====================================================
// MOTOR DRIVER 1 (BRUSH + VACUUM)
// =====================================================

// Brush Motor
#define PIN_BRUSH_FWD       23
#define PIN_BRUSH_REV       25

// Vacuum Motor
#define PIN_VACUUM_PWM_1    26
#define PIN_VACUUM_PWM_2    32

// =====================================================
// MOTOR DRIVER 2 (WHEELS)
// =====================================================

// Left Wheel
#define PIN_WHEEL_LEFT_FWD   12
#define PIN_WHEEL_LEFT_REV   13

// Right Wheel
#define PIN_WHEEL_RIGHT_FWD  14
#define PIN_WHEEL_RIGHT_REV  15

// =====================================================
// IR OBSTACLE SENSORS
// =====================================================

#define PIN_OB_LEFT         16
#define PIN_OB_FRONT        17
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

#define PIN_WIFI_RESET      0
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

#define API_POLL_INTERVAL     2000
#define BATTERY_SEND_INTERVAL 60000

// =====================================================
// HTTP SERVER
// =====================================================

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

// =====================================================
// SOFT START SETTINGS
// =====================================================
// PWM dinaikkan bertahap saat motor mulai (mengurangi inrush current)
#define SOFT_START_RAMP_STEP       5    // PWM increment per step (1-50)
#define SOFT_START_RAMP_INTERVAL   10   // ms between steps (~500ms full ramp 0->255)
// Catatan: Total waktu ramp ≈ (target_pwm / RAMP_STEP) * RAMP_INTERVAL
// Contoh Normal mode: (200 / 5) * 10 = 400ms

#endif