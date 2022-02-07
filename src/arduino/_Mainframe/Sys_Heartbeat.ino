/*
  GeoRover Heartbeat functions.
  Send and receive heartbeat to backup MCU.

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021
*/

unsigned long lastMillisResetBackup = 0;

// Runs heartbeat in and out
void HeartBeat() {
  HeartBeatOut();
  HeartBeatIn();

  HeartbeatBlackBox();
}

unsigned long lastMillisHeartbeatOut = 0;
// Sends heartbeat with with frequency of HRTBEAT_FRQ
void HeartBeatOut() {
  if (millis() - lastMillisHeartbeatOut > HRTBEAT_DT_OUT) {
    lastMillisHeartbeatOut = millis();
    digitalWrite(PO_BACKUP_HRTBEAT, true);
    delay(20);
    digitalWrite(PO_BACKUP_HRTBEAT, false);
  }
}

bool HeartBeatStatus() {
  HeartBeatIn();

  return GetStatus(MODULE_BACKUPCPU);
}

unsigned long lastMillisHeartbeatIn = 0;
// Checks if time since last heartbeat received > maximum treshold
// Will attempt to reset backup CPU with a frequency of  BACKUP_RST_FRQ
void HeartBeatIn() {
  if (millis() - lastMillisHeartbeatIn > HRTBEAT_TRESHOLD) {
    // Reset backup CPU
    if (GetStatus(MODULE_BACKUPCPU)) {
      lastMillisResetBackup = millis();
      SetStatus(MODULE_BACKUPCPU, false);
      DEBUG_PRINTLINE();
      DEBUG_PRINTLN("Error: Backup CPU offline, attempting to Reset.");
      ResetBackupCPU();
      DEBUG_PRINTLINE();
    } else {
      if (millis() - lastMillisResetBackup > BACKUP_RST_DT) {
        lastMillisResetBackup = millis();
        DEBUG_PRINTLINE();
        DEBUG_PRINTLN("Error: Backup CPU offline, attempting to Reset.");
        DEBUG_PRINTLINE();
        ResetBackupCPU();
      }
    }
  }
}

unsigned long lastMillisHeartbeatInt = 0;
// Resets timer since last heartbeat received
void HeartBeatInInterrupt() {
  if (millis() - lastMillisHeartbeatInt > BTN_DEBOUNCE_TIME) {
    lastMillisHeartbeatIn = millis();
    if (!GetStatus(MODULE_BACKUPCPU)) {
      SetStatus(MODULE_BACKUPCPU, true);
      DEBUG_PRINTLINE();
      DEBUG_PRINTLN("Backup CPU online");
      DEBUG_PRINTLINE();
    }
  }
}

// Attempt to reset backup CPU
void ResetBackupCPU() {
  digitalWrite(PO_BACKUP_RST, true);
  delay(20);
  digitalWrite(PO_BACKUP_RST, false);
}

unsigned long lastMillisHeartbeatBlackbox = 0;
void HeartbeatBlackBox() {
  if (millis() - lastMillisHeartbeatBlackbox > HRTBEAT_DT_LOG) {
    lastMillisHeartbeatBlackbox = millis();
    DEBUG_PRINTLINE();
    DEBUG_PRINT("Heartbeat System Status: ");
    SystemCheck();
    DEBUG_PRINTLN(String(ToLong(SystemStatus)));
    DEBUG_PRINTLINE();
  }
}
