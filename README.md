# 🤖 Autonomous Vacuum Robot

<p align="center">
  <img src="https://img.shields.io/badge/ESP32-Microcontroller-blue?style=for-the-badge&logo=espressif" alt="ESP32">
  <img src="https://img.shields.io/badge/Laravel-Framework-red?style=for-the-badge&logo=laravel" alt="Laravel">
  <img src="https://img.shields.io/badge/License-MIT-green?style=for-the-badge" alt="MIT License">
</p>

<p align="center">
  <strong>Web-controlled autonomous vacuum robot with monitoring and ESP32 integration.</strong>
</p>

---

## ✨ Features

### 🌐 Web Dashboard
- **Status Monitoring** — Live robot status (Standby, Cleaning, Returning, Charging)
- **Battery Monitoring** — Battery percentage with visual progress bar
- **Remote Control** — Start, Stop, and Return Home commands
- **Power Mode Selection** — ECO, NORMAL, and STRONG suction modes
- **Responsive Design** — Works on desktop and mobile devices

### 🔧 Hardware Control
- **Vacuum Motor** — PWM-controlled suction with 3 power levels
- **Brush Motor** — Forward/reverse brush rotation
- **Wheel Motors** — Differential drive (left/right wheels) for navigation
- **Obstacle Detection** — 3x IR sensors (left, front, right)
- **Cliff Detection** — 3x IR cliff sensors to prevent falls
- **Battery Monitoring** — Voltage divider + ADC for battery level

---

## 🛠️ Tech Stack

| Component | Technology |
|-----------|------------|
| **Backend** | Laravel 12 (PHP) |
| **Frontend** | Blade + Bootstrap 5 + jQuery |
| **Microcontroller** | ESP32 |
| **Database** | MySQL |
| **API** | RESTful JSON API |

---

## 📁 Project Structure

```
vacuum_web/
├── app/
│   └── Http/Controllers/
│       └── VacuumAPIController.php   # REST API endpoints
├── firmware/
│   └── VacuumRobot/
│       ├── VacuumRobot.ino           # Main Arduino sketch
│       ├── config.h                  # Pin & WiFi configuration
│       ├── ApiClient.cpp/h           # HTTP client for Laravel API
│       ├── RobotController.cpp/h     # Main robot logic
│       ├── VacuumMotor.cpp/h         # Vacuum motor control
│       ├── BrushMotor.cpp/h          # Brush motor control
│       ├── WheelMotor.cpp/h          # Wheel motor control
│       ├── SensorArray.cpp/h         # IR obstacle/cliff sensors
│       └── BatteryMonitor.cpp/h      # Battery voltage monitoring
├── resources/views/
│   └── main.blade.php                # Dashboard view
└── routes/
    └── api.php                       # API routes
```

---

## ⚡ Quick Start

### Prerequisites
- PHP 8.3.16+
- Composer
- MySQL 8.4.7
- Node.js (for frontend assets)
- Arduino IDE (for ESP32)

### Installation

1. **Clone the repository**
   ```bash
   git clone https://github.com/irsyaddza/autonomous-vacuum.git
   cd autonomous-vacuum
   ```

2. **Install dependencies**
   ```bash
   composer install
   npm install
   ```

3. **Configure environment**
   ```bash
   cp .env.example .env
   php artisan key:generate
   ```

4. **Setup database**
   ```bash
   # Edit .env with your MySQL credentials
   php artisan migrate
   ```

5. **Run the server**
   ```bash
   php artisan serve
   ```

6. **Access the dashboard**
   ```
   http://localhost:8000
   ```

### ESP32 Setup

1. Open `firmware/VacuumRobot/VacuumRobot.ino` in Arduino IDE
2. Edit `config.h` to set your API URL and WiFi credentials
3. Upload to ESP32
4. Connect to "VacuumRobot" WiFi AP for initial setup

---

## 🔌 Hardware Pinout

### Motor Driver 1 (L298N #1) — Brush & Vacuum
| Function | ESP32 Pin | L298N Pin |
|----------|-----------|-----------|
| Brush Forward | GPIO 23 | IN1 |
| Brush Reverse | GPIO 25 | IN2 |
| Vacuum PWM 1 | GPIO 26 | IN3 |
| Vacuum PWM 2 | GPIO 32 | IN4 |

### Motor Driver 2 (L298N #2) — Wheels
| Function | ESP32 Pin | L298N Pin |
|----------|-----------|-----------|
| Left Wheel FWD | GPIO 12 | IN1 |
| Left Wheel REV | GPIO 13 | IN2 |
| Right Wheel FWD | GPIO 14 | IN3 |
| Right Wheel REV | GPIO 15 | IN4 |

### Sensors
| Sensor | ESP32 Pin |
|--------|-----------|
| IR Left | GPIO 16 |
| IR Front | GPIO 17 |
| IR Right | GPIO 18 |
| Cliff Left | GPIO 19 |
| Cliff Front | GPIO 21 |
| Cliff Right | GPIO 22 |
| Battery ADC | GPIO 36 (VP) |
| WiFi Reset | GPIO 0 (BOOT) |

---

## 📡 API Endpoints

| Method | Endpoint | Description |
|--------|----------|-------------|
| GET | `/v1/vacuum/status` | Get robot status |
| POST | `/v1/vacuum/command` | Send command (start/stop/return_home) |
| POST | `/v1/vacuum/power-mode` | Set suction power mode |
| GET | `/v1/vacuum/battery/latest` | Get latest battery data |
| GET | `/v1/vacuum/full-status` | Get complete robot status |

---

## 🎮 Suction Power Modes

| Mode | PWM Value | Description |
|------|-----------|-------------|
| **ECO** | 150 | Energy saving, quiet operation |
| **NORMAL** | 200 | Standard suction power |
| **STRONG** | 255 | Maximum power for deep cleaning |

---

## 📄 License

MIT License

Copyright (c) 2026 Autonomous Vacuum Robot

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
