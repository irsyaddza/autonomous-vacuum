#ifndef TIMING_SETTINGS_H
#define TIMING_SETTINGS_H

#include "config.h"

// ===== RUNTIME TIMING SETTINGS =====
// Variabel ini bisa diubah lewat halaman Diagnostic di website
// tanpa perlu flash ulang firmware.
// Nilai default diambil dari #define di config.h

struct TimingSettings {
    // Obstacle avoidance
    int backupDuration      = BACKUP_DURATION;
    int turnDurationMin     = TURN_DURATION_MIN;
    int turnDurationMax     = TURN_DURATION_MAX;
    int turnDurationSmall   = TURN_DURATION_SMALL;
    
    // Cliff avoidance
    int cliffBackupDuration = CLIFF_BACKUP_DURATION;
    int cliffTurnDuration   = CLIFF_TURN_DURATION;
    
    // Spiral pattern
    int spiralInitialDuration = SPIRAL_INITIAL_DURATION;
    int spiralIncrement       = SPIRAL_INCREMENT;
    int spiralMaxDuration     = SPIRAL_MAX_DURATION;
    int spiralTurnDuration    = SPIRAL_TURN_DURATION;
    
    // Stuck detection
    int stuckObstacleCount  = STUCK_OBSTACLE_COUNT;
    int stuckTimeWindow     = STUCK_TIME_WINDOW;
    int escapeTurnDuration  = ESCAPE_TURN_DURATION;
    
    // Reset semua ke default
    void resetDefaults() {
        backupDuration      = BACKUP_DURATION;
        turnDurationMin     = TURN_DURATION_MIN;
        turnDurationMax     = TURN_DURATION_MAX;
        turnDurationSmall   = TURN_DURATION_SMALL;
        cliffBackupDuration = CLIFF_BACKUP_DURATION;
        cliffTurnDuration   = CLIFF_TURN_DURATION;
        spiralInitialDuration = SPIRAL_INITIAL_DURATION;
        spiralIncrement       = SPIRAL_INCREMENT;
        spiralMaxDuration     = SPIRAL_MAX_DURATION;
        spiralTurnDuration    = SPIRAL_TURN_DURATION;
        stuckObstacleCount  = STUCK_OBSTACLE_COUNT;
        stuckTimeWindow     = STUCK_TIME_WINDOW;
        escapeTurnDuration  = ESCAPE_TURN_DURATION;
    }
};

// Global instance — accessible from all files
extern TimingSettings timing;

#endif
