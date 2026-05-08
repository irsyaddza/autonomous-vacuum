#include "TimingSettings.h"
#include <Preferences.h>

Preferences timingPrefs;

void TimingSettings::load() {
    timingPrefs.begin("vacuum-timing", true); // read-only
    backupDuration = timingPrefs.getInt("backupDuration", BACKUP_DURATION);
    turnDurationMin = timingPrefs.getInt("turnMin", TURN_DURATION_MIN);
    turnDurationMax = timingPrefs.getInt("turnMax", TURN_DURATION_MAX);
    turnDurationSmall = timingPrefs.getInt("turnSmall", TURN_DURATION_SMALL);
    cliffBackupDuration = timingPrefs.getInt("cliffBackup", CLIFF_BACKUP_DURATION);
    cliffTurnDuration = timingPrefs.getInt("cliffTurn", CLIFF_TURN_DURATION);
    spiralInitialDuration = timingPrefs.getInt("spiralInit", SPIRAL_INITIAL_DURATION);
    spiralIncrement = timingPrefs.getInt("spiralInc", SPIRAL_INCREMENT);
    spiralMaxDuration = timingPrefs.getInt("spiralMax", SPIRAL_MAX_DURATION);
    spiralTurnDuration = timingPrefs.getInt("spiralTurn", SPIRAL_TURN_DURATION);
    stuckObstacleCount = timingPrefs.getInt("stuckCount", STUCK_OBSTACLE_COUNT);
    stuckTimeWindow = timingPrefs.getInt("stuckTime", STUCK_TIME_WINDOW);
    escapeTurnDuration = timingPrefs.getInt("escapeTurn", ESCAPE_TURN_DURATION);
    timingPrefs.end();
}

void TimingSettings::save() {
    timingPrefs.begin("vacuum-timing", false); // read-write
    timingPrefs.putInt("backupDuration", backupDuration);
    timingPrefs.putInt("turnMin", turnDurationMin);
    timingPrefs.putInt("turnMax", turnDurationMax);
    timingPrefs.putInt("turnSmall", turnDurationSmall);
    timingPrefs.putInt("cliffBackup", cliffBackupDuration);
    timingPrefs.putInt("cliffTurn", cliffTurnDuration);
    timingPrefs.putInt("spiralInit", spiralInitialDuration);
    timingPrefs.putInt("spiralInc", spiralIncrement);
    timingPrefs.putInt("spiralMax", spiralMaxDuration);
    timingPrefs.putInt("spiralTurn", spiralTurnDuration);
    timingPrefs.putInt("stuckCount", stuckObstacleCount);
    timingPrefs.putInt("stuckTime", stuckTimeWindow);
    timingPrefs.putInt("escapeTurn", escapeTurnDuration);
    timingPrefs.end();
}
