/*
  GeoRover Heartbeat functions.
  Send and receive heartbeat to backup MCU.

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021
*/

unsigned long lastMillisResetBackup = 0;
bool receivedHeartBeat = false;

// Runs heartbeat in and out
void HeartBeat() {
  HeartBeatOut();
  HeartBeatIn();
  HeartBeatTimeout();
  HeartbeatBlackBox();
}

unsigned long lastMillisHeartbeatOut = 0;
// Sends heartbeat with with frequency of HRTBEAT_FRQ
void HeartBeatOut() {
  if (millis() - lastMillisHeartbeatOut > HRTBEAT_DT_OUT) {
    // DEBUG_PRINTLN("Sending Heartbeat!");
    lastMillisHeartbeatOut = millis();
    digitalWrite(PO_BACKUP_HRTBEAT, true);
    delay(20);
    digitalWrite(PO_BACKUP_HRTBEAT, false);
  }
}

bool HeartBeatStatus() {
  HeartBeatTimeout();

  return GetStatus(MODULE_BACKUPCPU);
}


unsigned long lastMillisHeartbeatIn = 0;
// Checks if time since last heartbeat received > maximum treshold
// Will attempt to reset backup CPU with a frequency of  BACKUP_RST_FRQ
void HeartBeatTimeout() {
  if (millis() - lastMillisHeartbeatIn > HRTBEAT_TRESHOLD) {
    // Reset backup CPU
    if (GetStatus(MODULE_BACKUPCPU)) {
      lastMillisResetBackup = millis();
      SetStatus(MODULE_BACKUPCPU, false);
      DEBUG_PRINTLINE();
      DEBUG_PRINTLN("Error: Backup CPU Heartbeat Timeout, attempting to Reset Backup System.");
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


// Resets Heartbeatflag if set and resets heartbeat in timer
void HeartBeatIn() {
  if (!receivedHeartBeat) {
    return;
  }
  receivedHeartBeat = false;
  
  if (millis() - lastMillisHeartbeatIn < BTN_DEBOUNCE_TIME) {
    return;
  }

  lastMillisHeartbeatIn = millis();
  // DEBUG_PRINTLN("Heartbeat Received!");
  if (!GetStatus(MODULE_BACKUPCPU)) {
    SetStatus(MODULE_BACKUPCPU, true);
    DEBUG_PRINTLINE();
    DEBUG_PRINTLN("Backup CPU online");
    DEBUG_PRINTLINE();
  }
}

// Set Heartbeat flag
void HeartBeatInInterrupt() {
  if(receivedHeartBeat){
    return;
  }
  receivedHeartBeat = true;
}

// Attempt to reset backup CPU
void ResetBackupCPU() {
  digitalWrite(PO_BACKUP_RST, false);
  delay(20);
  digitalWrite(PO_BACKUP_RST, true);
}

unsigned long lastMillisHeartbeatBlackbox = HRTBEAT_DT_LOG;
void HeartbeatBlackBox() {
  if (millis() - lastMillisHeartbeatBlackbox > HRTBEAT_DT_LOG) {
    lastMillisHeartbeatBlackbox = millis();
    DEBUG_PRINTLINE();
    DEBUG_PRINTLN("Heartbeat System Check: ");
    SystemCheck();
    DEBUG_PRINT("System Status: ");
    DEBUG_PRINTLN(String(ToLong(SystemStatus)));
    DEBUG_PRINTLINE();
  }
}
