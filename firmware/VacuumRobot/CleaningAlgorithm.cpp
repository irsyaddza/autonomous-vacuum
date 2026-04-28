#include "CleaningAlgorithm.h"
#include "config.h"
#include "TimingSettings.h"
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
    _spiralForwardDuration = timing.spiralInitialDuration;
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
        if (elapsed < _spiralForwardDuration + (unsigned long)timing.spiralTurnDuration) {
            // Still turning, wait
            return;
        }
        
        // Belok selesai, tambah durasi lurus untuk spiral mengembang
        _spiralForwardDuration += timing.spiralIncrement;
        _spiralTurnRight = !_spiralTurnRight;  // Alternate direction for spiral
        
        // Cek apakah spiral sudah cukup besar → pindah ke Random Bounce
        if (_spiralForwardDuration >= (unsigned long)timing.spiralMaxDuration) {
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
    
    if (elapsed < (unsigned long)timing.backupDuration) {
        wheels.moveBackward();
    } else {
        // Mundur selesai → mulai belok
        // Simpan snapshot sensor SEKARANG karena setelah belok sensor bisa berubah
        bool front = sensors.isFrontBlocked();
        bool left  = sensors.isLeftBlocked();
        bool right = sensors.isRightBlocked();
        
        TurnDirection dir = _decideTurnDirection();
        
        // Pilih durasi belok berdasarkan kondisi obstacle:
        unsigned long duration;
        if (front && left && right) {
            // Semua sisi terblokir (pojok) → putar besar
            duration = timing.turnDurationMax;
        } else if (front) {
            // Depan terblokir → belok sedang-besar
            duration = random(timing.turnDurationMin, timing.turnDurationMax + 1);
        } else if (left != right) {
            // Hanya satu sisi terblokir, depan clear → belok kecil
            duration = timing.turnDurationSmall;
        } else {
            duration = random(timing.turnDurationMin, timing.turnDurationMax + 1);
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
    
    if (elapsed < (unsigned long)timing.cliffBackupDuration) {
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
        
        _startTurn(dir, timing.cliffTurnDuration);
        
        Serial.print("[CLEAN] Cliff backup done → Turn ");
        Serial.println(dir == TURN_LEFT ? "LEFT 180°" : "RIGHT 180°");
    }
}

// Belok setelah mundur
void CleaningAlgorithm::_handleTurning() {
    unsigned long elapsed = millis() - _stateStartTime;
    
    // PENTING: Jangan cek obstacle saat belok!
    // Robot harus menyelesaikan putarannya dulu agar bisa bebas dari halangan.
    // Jika kita interrupt di tengah belok, robot akan terjebak loop mundur-belok.
    
    if (elapsed < _turnDuration) {
        if (_turnDir == TURN_LEFT) {
            wheels.turnLeft();
        } else {
            wheels.turnRight();
        }
    } else {
        // Belok selesai → cek apakah masih ada obstacle di depan
        if (sensors.isFrontBlocked()) {
            // Masih ada halangan di depan setelah belok → mundur dan belok lagi
            Serial.println("[CLEAN] Turn complete but front still blocked → backup again");
            _startBackupObstacle();
        } else {
            // Depan clear → jalan lurus lagi
            _setState(CLEAN_FORWARD);
            wheels.moveForward();
            Serial.println("[CLEAN] Turn complete → Moving forward");
        }
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
    if (millis() - _lastObstacleTime < (unsigned long)timing.stuckTimeWindow) {
        _obstacleCount++;
    } else {
        _obstacleCount = 1;
    }
    _lastObstacleTime = millis();
    
    if (_obstacleCount >= timing.stuckObstacleCount) {
        // Robot terjebak! Putar besar untuk escape
        Serial.print("[CLEAN] !!! STUCK DETECTED (");
        Serial.print(timing.stuckObstacleCount);
        Serial.print(" obstacles in ");
        Serial.print(timing.stuckTimeWindow / 1000);
        Serial.println("s) → Escape turn");
        _obstacleCount = 0;
        TurnDirection escapeDir = (random(2) == 0) ? TURN_LEFT : TURN_RIGHT;
        _startTurn(escapeDir, timing.escapeTurnDuration);
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

// Random turn duration antara turnDurationMin dan turnDurationMax
unsigned long CleaningAlgorithm::_randomTurnDuration() {
    return random(timing.turnDurationMin, timing.turnDurationMax + 1);
}

// Tentukan arah belok berdasarkan sensor mana yang triggered
TurnDirection CleaningAlgorithm::_decideTurnDirection() {
    bool front = sensors.isFrontBlocked();
    bool left  = sensors.isLeftBlocked();
    bool right = sensors.isRightBlocked();
    
    // PRIORITAS 1: Semua sisi terblokir (pojok/sudut mati) → random
    if (front && left && right) {
        return (random(2) == 0) ? TURN_LEFT : TURN_RIGHT;
    }
    
    // PRIORITAS 2: Depan + satu sisi → belok ke sisi yang kosong
    if (front && left) {
        return TURN_RIGHT;  // Depan + kiri terblokir → belok kanan
    }
    if (front && right) {
        return TURN_LEFT;   // Depan + kanan terblokir → belok kiri
    }
    
    // PRIORITAS 3: Kedua sisi terblokir, tapi depan kosong → lorong sempit, random
    if (left && right) {
        return (random(2) == 0) ? TURN_LEFT : TURN_RIGHT;
    }
    
    // PRIORITAS 4: Hanya satu sisi → belok menjauhi obstacle
    if (left) {
        return TURN_RIGHT;
    }
    if (right) {
        return TURN_LEFT;
    }
    
    // PRIORITAS 5: Hanya depan terblokir → random
    return (random(2) == 0) ? TURN_LEFT : TURN_RIGHT;
}
