#ifndef CLEANING_ALGORITHM_H
#define CLEANING_ALGORITHM_H

#include <Arduino.h>

// ===== STATE MACHINE =====
// Algoritma pembersihan menggunakan Random Bounce + Spiral
// Referensi: iRobot Roomba 500-series reactive cleaning pattern

enum CleanState {
    CLEAN_IDLE,              // Tidak aktif
    CLEAN_SPIRAL,            // Fase 1: Spiral expanding
    CLEAN_FORWARD,           // Fase 2: Jalan lurus (Random Bounce)
    CLEAN_BACKUP_OBSTACLE,   // Mundur karena obstacle
    CLEAN_BACKUP_CLIFF,      // Mundur karena cliff (emergency)
    CLEAN_TURNING            // Belok setelah mundur
};

enum TurnDirection {
    TURN_LEFT,
    TURN_RIGHT
};

class CleaningAlgorithm {
public:
    void begin();
    
    // Main update - dipanggil setiap loop cycle
    // Returns true jika sedang aktif membersihkan
    bool update();
    
    // Start/stop autonomous cleaning
    void start();
    void stop();
    
    // Cek apakah sedang aktif
    bool isActive();
    
    // Get current state name (untuk debugging)
    String getStateName();

private:
    CleanState _state = CLEAN_IDLE;
    CleanState _prevState = CLEAN_IDLE;
    
    bool _active = false;
    
    // Timing
    unsigned long _stateStartTime = 0;
    unsigned long _stateDuration = 0;
    
    // Spiral tracking
    unsigned long _spiralForwardDuration = 0;  // Durasi lurus saat spiral
    bool _spiralTurnRight = true;              // Alternating turn direction
    
    // Turn tracking
    TurnDirection _turnDir = TURN_RIGHT;
    unsigned long _turnDuration = 0;
    
    // Cliff state snapshot (saved when cliff first detected)
    bool _cliffWasFront = false;
    bool _cliffWasLeft  = false;
    bool _cliffWasRight = false;
    
    // Stuck detection
    unsigned long _lastObstacleTime = 0;
    int _obstacleCount = 0;
    
    // State handlers
    void _handleSpiral();
    void _handleForward();
    void _handleBackupObstacle();
    void _handleBackupCliff();
    void _handleTurning();
    
    // Helpers
    void _setState(CleanState newState);
    void _startBackupObstacle();
    void _startBackupCliff();
    void _startTurn(TurnDirection dir, unsigned long duration);
    unsigned long _randomTurnDuration();
    TurnDirection _decideTurnDirection();
    
    // Safety check - runs every cycle regardless of state
    // Returns true if cliff was detected and emergency handled
    bool _checkCliffEmergency();
};

#endif
