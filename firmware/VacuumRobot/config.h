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

#endif