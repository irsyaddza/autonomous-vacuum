#include "CleaningAlgorithm.h"
#include "config.h"
#include "SensorArray.h"
#include "WheelMotor.h"
#include "BrushMotor.h"
#include "VacuumMotor.h"

// External objects (defined in VacuumRobot.ino)
extern SensorArray sensors;
extern WheelMotor wheels;
extern BrushMotor brush;
extern VacuumMotor vacuum;

void CleaningAlgorithm::begin() {
    // Seed random number generator from ESP32 hardware RNG
    // WARNING: Do NOT use analogRead(0) here!
    // GPIO 0 is the WiFi reset button - analogRead(0) changes its pin mode
    // to analog, which breaks digitalRead(0) in checkResetButton()
    randomSeed(esp_random());
    Serial.println("[CLEAN] Cleaning Algorithm initialized (Random Bounce + Spiral)");
}

void CleaningAlgorithm::start() {
    if (_active) return;
    
    _active = true;
    _spiralForwardDuration = SPIRAL_INITIAL_DURATION;
    _spiralTurnRight = true;
    
    // Mulai dengan fase spiral
    _setState(CLEAN_SPIRAL);
    
    Serial.println("==========================================");
    Serial.println("[CLEAN] >>> AUTONOMOUS CLEANING STARTED <<<");
    Serial.println("[CLEAN] Phase 1: Spiral Expanding");
    Serial.println("==========================================");
}

void CleaningAlgorithm::stop() {
    if (!_active) return;
    
    _active = false;
    _setState(CLEAN_IDLE);
    wheels.stop();
    
    Serial.println("[CLEAN] >>> AUTONOMOUS CLEANING STOPPED <<<");
}

bool CleaningAlgorithm::isActive() {
    return _active;
}

String CleaningAlgorithm::getStateName() {
    switch (_state) {
        case CLEAN_IDLE:             return "IDLE";
        case CLEAN_SPIRAL:           return "SPIRAL";
        case CLEAN_FORWARD:          return "FORWARD";
        case CLEAN_BACKUP_OBSTACLE:  return "BACKUP_OBSTACLE";
        case CLEAN_BACKUP_CLIFF:     return "BACKUP_CLIFF";
        case CLEAN_TURNING:          return "TURNING";
        default:                     return "UNKNOWN";
    }
}

// ===== MAIN UPDATE LOOP =====
bool CleaningAlgorithm::update() {
    if (!_active) return false;
    
    // Read all sensors every cycle
    sensors.readObstacles();
    sensors.readCliffs();
    
    // PRIORITY 1: Cliff emergency check (overrides everything)
    if (_checkCliffEmergency()) {
        return true;  // Cliff handled, skip normal logic
    }
    
    // State machine
    switch (_state) {
        case CLEAN_SPIRAL:
            _handleSpiral();
            break;
        case CLEAN_FORWARD:
            _handleForward();
            break;
        case CLEAN_BACKUP_OBSTACLE:
            _handleBackupObstacle();
            break;
        case CLEAN_BACKUP_CLIFF:
            _handleBackupCliff();
            break;
        case CLEAN_TURNING:
            _handleTurning();
            break;
        default:
            break;
    }
    
    return true;
}

// ===== STATE HANDLERS =====

// Fase 1: Spiral Expanding
// Robot bergerak lurus, belok 90°, lurus lebih panjang, belok 90°, repeat
// Spiral mengembang sampai hit obstacle atau durasi max
void CleaningAlgorithm::_handleSpiral() {
    unsigned long elapsed = millis() - _stateStartTime;
    
    // Cek obstacle kapan saja selama spiral (lurus maupun belok)
    if (sensors.isAnyObstacle()) {
        Serial.println("[CLEAN] Spiral: Obstacle detected → switching to Random Bounce");
        _startBackupObstacle();
        return;
    }
    
    // Bergerak lurus selama _spiralForwardDuration
    if (elapsed < _spiralForwardDuration) {
        wheels.moveForward();
    } else {
        // Waktu lurus habis → belok 90°
        if (_spiralTurnRight) {
            wheels.turnRight();
        } else {
            wheels.turnLeft();
        }
        
        // Tunggu belok selesai
        if (elapsed < _spiralForwardDuration + SPIRAL_TURN_DURATION) {
            // Still turning, wait
            return;
        }
        
        // Belok selesai, tambah durasi lurus untuk spiral mengembang
        _spiralForwardDuration += SPIRAL_INCREMENT;
        _spiralTurnRight = !_spiralTurnRight;  // Alternate direction for spiral
        
        // Cek apakah spiral sudah cukup besar → pindah ke Random Bounce
        if (_spiralForwardDuration >= SPIRAL_MAX_DURATION) {
            Serial.println("[CLEAN] Spiral complete → Phase 2: Random Bounce");
            _setState(CLEAN_FORWARD);
            wheels.moveForward();
            return;
        }
        
        // Reset timer untuk segment spiral berikutnya
        _stateStartTime = millis();
        wheels.moveForward();
        
        Serial.print("[CLEAN] Spiral: next segment = ");
        Serial.print(_spiralForwardDuration);
        Serial.println("ms");
    }
}

// Fase 2: Random Bounce - jalan lurus sampai ada obstacle
void CleaningAlgorithm::_handleForward() {
    // Cek obstacle
    if (sensors.isAnyObstacle()) {
        Serial.print("[CLEAN] Obstacle detected! Front=");
        Serial.print(sensors.isFrontBlocked());
        Serial.print(" Left=");
        Serial.print(sensors.isLeftBlocked());
        Serial.print(" Right=");
        Serial.println(sensors.isRightBlocked());
        
        _startBackupObstacle();
        return;
    }
    
    // Tidak ada halangan → jalan terus
    wheels.moveForward();
}

// Mundur karena obstacle
void CleaningAlgorithm::_handleBackupObstacle() {
    unsigned long elapsed = millis() - _stateStartTime;
    
    if (elapsed < BACKUP_DURATION) {
        wheels.moveBackward();
    } else {
        // Mundur selesai → mulai belok
        TurnDirection dir = _decideTurnDirection();
        
        // Pilih durasi belok berdasarkan kondisi obstacle:
        // - Side-only (tanpa front) → belok kecil saja
        // - Front / multi-side → random turn normal
        bool front = sensors.isFrontBlocked();
        bool left  = sensors.isLeftBlocked();
        bool right = sensors.isRightBlocked();
        
        unsigned long duration;
        if (!front && (left != right)) {
            // Hanya satu sisi terblokir, depan clear → belok kecil cukup
            duration = TURN_DURATION_SMALL;
        } else {
            duration = _randomTurnDuration();
        }
        
        _startTurn(dir, duration);
        
        Serial.print("[CLEAN] Backup done → Turn ");
        Serial.print(dir == TURN_LEFT ? "LEFT" : "RIGHT");
        Serial.print(" for ");
        Serial.print(duration);
        Serial.println("ms");
    }
}

// Mundur karena cliff (emergency - lebih lama)
void CleaningAlgorithm::_handleBackupCliff() {
    unsigned long elapsed = millis() - _stateStartTime;
    
    if (elapsed < CLIFF_BACKUP_DURATION) {
        wheels.moveBackward();
    } else {
        // Cliff → selalu putar 180° (durasi lebih lama)
        // Gunakan SNAPSHOT cliff state (disimpan saat pertama terdeteksi)
        // karena setelah mundur, sensor cliff sudah tidak detect lagi
        TurnDirection dir;
        if (_cliffWasLeft && !_cliffWasRight) {
            dir = TURN_RIGHT;  // Cliff di kiri → belok kanan
        } else if (_cliffWasRight && !_cliffWasLeft) {
            dir = TURN_LEFT;   // Cliff di kanan → belok kiri
        } else {
            dir = (random(2) == 0) ? TURN_LEFT : TURN_RIGHT; // Random
        }
        
        _startTurn(dir, CLIFF_TURN_DURATION);
        
        Serial.print("[CLEAN] Cliff backup done → Turn ");
        Serial.println(dir == TURN_LEFT ? "LEFT 180°" : "RIGHT 180°");
    }
}

// Belok setelah mundur
void CleaningAlgorithm::_handleTurning() {
    unsigned long elapsed = millis() - _stateStartTime;
    
    // Cek obstacle saat belok - hindari berputar menuju obstacle lain
    if (sensors.isAnyObstacle()) {
        Serial.println("[CLEAN] Obstacle during turn → backup again");
        _startBackupObstacle();
        return;
    }
    
    if (elapsed < _turnDuration) {
        if (_turnDir == TURN_LEFT) {
            wheels.turnLeft();
        } else {
            wheels.turnRight();
        }
    } else {
        // Belok selesai → jalan lurus lagi
        _setState(CLEAN_FORWARD);
        wheels.moveForward();
        Serial.println("[CLEAN] Turn complete → Moving forward");
    }
}

// ===== SAFETY =====

// Cliff emergency check - HIGHEST PRIORITY
// Runs every cycle, overrides any current state
bool CleaningAlgorithm::_checkCliffEmergency() {
    if (!sensors.isCliffDetected()) return false;
    
    // Jangan interrupt jika sudah sedang handling cliff
    if (_state == CLEAN_BACKUP_CLIFF) return false;
    
    // Simpan snapshot cliff state SEKARANG
    // Setelah robot mundur, sensor cliff akan clear → data hilang
    _cliffWasFront = sensors.isCliffFront();
    _cliffWasLeft  = sensors.isCliffLeft();
    _cliffWasRight = sensors.isCliffRight();
    
    Serial.println("!!! [CLEAN] CLIFF EMERGENCY !!!");
    Serial.print("    Front=");
    Serial.print(_cliffWasFront);
    Serial.print(" Left=");
    Serial.print(_cliffWasLeft);
    Serial.print(" Right=");
    Serial.println(_cliffWasRight);
    
    // Immediately stop and backup
    wheels.stop();
    _setState(CLEAN_BACKUP_CLIFF);
    
    return true;
}

// ===== HELPERS =====

void CleaningAlgorithm::_setState(CleanState newState) {
    if (newState != _state) {
        _prevState = _state;
        _state = newState;
        _stateStartTime = millis();
    }
}

void CleaningAlgorithm::_startBackupObstacle() {
    wheels.stop();
    
    // Stuck detection: jika terlalu sering obstacle dalam waktu singkat
    if (millis() - _lastObstacleTime < 3000) {
        _obstacleCount++;
    } else {
        _obstacleCount = 1;
    }
    _lastObstacleTime = millis();
    
    if (_obstacleCount >= 5) {
        // Robot terjebak! Putar besar untuk escape
        Serial.println("[CLEAN] !!! STUCK DETECTED (5 obstacles in 3s) → Escape turn");
        _obstacleCount = 0;
        TurnDirection escapeDir = (random(2) == 0) ? TURN_LEFT : TURN_RIGHT;
        _startTurn(escapeDir, CLIFF_TURN_DURATION * 2);  // Putar ~360° untuk escape
        return;
    }
    
    _setState(CLEAN_BACKUP_OBSTACLE);
}

void CleaningAlgorithm::_startBackupCliff() {
    wheels.stop();
    _setState(CLEAN_BACKUP_CLIFF);
}

void CleaningAlgorithm::_startTurn(TurnDirection dir, unsigned long duration) {
    _turnDir = dir;
    _turnDuration = duration;
    _setState(CLEAN_TURNING);
}

// Random turn duration antara TURN_DURATION_MIN dan TURN_DURATION_MAX
unsigned long CleaningAlgorithm::_randomTurnDuration() {
    return random(TURN_DURATION_MIN, TURN_DURATION_MAX + 1);
}

// Tentukan arah belok berdasarkan sensor mana yang triggered
TurnDirection CleaningAlgorithm::_decideTurnDirection() {
    bool front = sensors.isFrontBlocked();
    bool left  = sensors.isLeftBlocked();
    bool right = sensors.isRightBlocked();
    
    if (left && right) {
        // Kedua sisi terblokir → lorong sempit, putar random
        // Durasi belok akan lebih panjang (ditangani di _handleBackupObstacle)
        return (random(2) == 0) ? TURN_LEFT : TURN_RIGHT;
    } else if (left && !right) {
        // Obstacle di kiri saja → belok kanan
        return TURN_RIGHT;
    } else if (right && !left) {
        // Obstacle di kanan saja → belok kiri
        return TURN_LEFT;
    } else if (front && left) {
        // Depan + kiri → belok kanan
        return TURN_RIGHT;
    } else if (front && right) {
        // Depan + kanan → belok kiri
        return TURN_LEFT;
    } else {
        // Front only → random
        return (random(2) == 0) ? TURN_LEFT : TURN_RIGHT;
    }
}
