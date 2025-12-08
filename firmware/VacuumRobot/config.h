#ifndef CONFIG_H
#define CONFIG_H

// ===== PIN DEFINITIONS =====

// Motors (L298N or similar)
// Note: Variable speed usually requires PWM pins
#define PIN_MOTOR_DIR_1     26 
#define PIN_MOTOR_DIR_2     27 
#define PIN_MOTOR_PWM       14 

// Vacuum Motor (MOSFET)
#define PIN_VACUUM_PWM      12

// Sensors (IR Obstacle) - Digital Input
#define PIN_IR_FRONT        34
#define PIN_IR_LEFT         35
#define PIN_IR_RIGHT        32

// Sensors (IR Cliff) - Digital Input
#define PIN_CLIFF_LEFT      33
#define PIN_CLIFF_RIGHT     25
#define PIN_CLIFF_FRONT     39 // VN pin

// Battery (Voltage Divider) - Analog Input
#define PIN_BATTERY_ADC     36 // VP pin

// Function Buttons
#define PIN_WIFI_RESET      0  // Tombol BOOT bawaan ESP32. Tekan tahan 5 detik.

// ===== CONSTANTS =====

// WiFi Credentials (IGNORED by WiFiManager)
// #define WIFI_SSID           "Arsenal"
// #define WIFI_PASS           "kotamedan2022"

// API Config
#define API_BASE_URL        "http://192.168.1.2:8000/v1/vacuum" // Menggunakan php artisan serve
#define API_POLL_INTERVAL   2000 // ms
#define BATTERY_SEND_INTERVAL 60000 // ms

// Motor Settings
#define SPEED_CLEAN         150
#define SPEED_TURN          120
#define VACUUM_POWER_NORMAL 200

#endif
