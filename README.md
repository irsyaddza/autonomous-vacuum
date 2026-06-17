# Autonomous Vacuum Robot

Web dashboard and firmware for an ESP32-based autonomous vacuum. Control the robot, monitor sensors, and tune cleaning parameters from a browser.

---

## What it does

The robot moves in an expanding spiral pattern, then switches to random bounce navigation. Six IR sensors handle obstacle avoidance and cliff detection. A Laravel backend logs commands and battery events. The web dashboard communicates directly with the ESP32 over HTTP for low-latency control.

**Dashboard**
- Live robot status with battery percentage and voltage
- Start/stop commands and suction power selection (eco, normal, strong)
- Command history with response times
- Responsive layout for desktop and mobile

**Diagnostic**
- Real-time polling of all six IR sensors (raw + debounced values)
- Live timing parameter tuning without reflashing firmware
- Raw JSON log for debugging

**Firmware**
- Spiral expanding + random bounce cleaning algorithm
- Obstacle avoidance with stuck detection and escape maneuvers
- Cliff protection with immediate motor halt
- Battery monitoring with auto-stop at depletion
- WiFi AP mode for initial network setup

---

## Stack

| Layer | Technology |
|-------|------------|
| Microcontroller | ESP32 (Arduino framework) |
| Backend | Laravel 12 (PHP 8.3) |
| Frontend | Blade templates, Bootstrap 5.3, vanilla JS (fetch API) |
| Database | MySQL 8 |
| API | RESTful JSON, Scramble-documented |

---

## Project structure

```
├── app/
│   ├── Http/Controllers/
│   │   └── VacuumAPIController.php       # REST API endpoints
│   ├── Models/
│   │   ├── BatteryLog.php                # Battery event log model
│   │   ├── CommandLog.php                # Command history model
│   │   ├── Esp32Device.php               # Registered ESP32 devices
│   │   └── VacuumStatus.php              # Robot status snapshots
│   └── Console/Commands/
│       ├── CompileFirmware.php            # Artisan firmware build command
│       └── TestCommandAccuracy.php        # API accuracy test
├── firmware/VacuumRobot/
│   ├── VacuumRobot.ino                   # Main Arduino sketch
│   ├── config.h                          # Pin definitions and WiFi config
│   ├── RobotController.cpp/h             # Main robot state machine
│   ├── CleaningAlgorithm.cpp/h           # Spiral + random bounce logic
│   ├── ApiClient.cpp/h                   # HTTP client for Laravel API
│   ├── SensorArray.cpp/h                 # IR obstacle and cliff sensors
│   ├── BatteryMonitor.cpp/h              # ADC voltage reading
│   ├── VacuumMotor.cpp/h                 # PWM suction control
│   ├── BrushMotor.cpp/h                  # Brush rotation
│   ├── WheelMotor.cpp/h                  # Differential drive
│   └── TimingSettings.cpp/h              # Runtime-tunable timing values
├── resources/views/
│   ├── main.blade.php                    # Dashboard
│   ├── diagnostic.blade.php              # Sensor diagnostic page
│   └── about.blade.php                   # About page
├── routes/
│   └── web.php                           # All routes (web + API)
└── public/css/
    └── custom-dark.css                   # Dashboard styling
```

---

## Setup

### Prerequisites

- PHP 8.3+
- Composer
- MySQL 8+
- Arduino IDE or arduino-cli (for ESP32 firmware)

### Web server

```bash
git clone https://github.com/irsyaddza/autonomous-vacuum
cd autonomous-vacuum

composer install

cp .env.example .env
php artisan key:generate

# Edit .env with your MySQL credentials, then:
php artisan migrate --seed
php artisan serve --host=0.0.0.0
```

Dashboard is at `http://localhost:8000` or `http://your-ip:8000`.

### ESP32 firmware

1. Open `firmware/VacuumRobot/VacuumRobot.ino` in Arduino IDE
2. Edit `config.h` — set esp32's WiFi SSID, password, or keep it default, and change your Laravel API URL
3. Upload to ESP32
4. On first boot, connect to the "VacuumRobot" WiFi AP to register the device

Alternatively, use the Artisan compile command:
```bash
php artisan firmware:compile --port COM3
```

---

## Hardware pinout

### Motor driver 1 (L298N) — Brush and vacuum

| Function | ESP32 GPIO | L298N |
|----------|------------|-------|
| Brush forward | 23 | IN1 |
| Brush reverse | 25 | IN2 |
| Vacuum PWM 1 | 26 | IN3 |
| Vacuum PWM 2 | 32 | IN4 |

### Motor driver 2 (L298N) — Wheels

| Function | ESP32 GPIO | L298N |
|----------|------------|-------|
| Left wheel forward | 12 | IN1 |
| Left wheel reverse | 13 | IN2 |
| Right wheel forward | 14 | IN3 |
| Right wheel reverse | 15 | IN4 |

### Sensors and other

| Component | ESP32 GPIO |
|-----------|------------|
| IR obstacle left | 33 |
| IR obstacle front | 34 |
| IR obstacle right | 18 |
| IR cliff left | 19 |
| IR cliff front | 21 |
| IR cliff right | 22 |
| Battery ADC (voltage divider) | 36 (VP) |
| WiFi reset button | 4 |
| Buzzer | 27 |

---

## API

Full interactive documentation is available at `/docs/api` (Scramble).

| Method | Endpoint | Purpose |
|--------|----------|--------|
| GET | `/v1/vacuum/full-status` | Combined robot status + battery |
| GET | `/v1/vacuum/status` | Latest robot status |
| GET | `/v1/vacuum/battery/latest` | Latest battery reading |
| GET | `/v1/vacuum/battery-events/latest` | Latest battery event (warning/auto-stop) |
| POST | `/v1/vacuum/command` | Send command to robot |
| POST | `/v1/vacuum/command-log` | Log a command entry |
| GET | `/v1/vacuum/command-logs` | Fetch command history |
| GET | `/v1/vacuum/device` | Get registered ESP32 device info |
| POST | `/v1/vacuum/reset-devices` | Clear registered ESP32 IP |

### Suction modes

| Mode | PWM value | Use case |
|------|-----------|----------|
| Eco | 150 | Hard floors, quiet operation |
| Normal | 200 | Standard cleaning |
| Strong | 255 | Carpet, deep cleaning |

---

## License

MIT