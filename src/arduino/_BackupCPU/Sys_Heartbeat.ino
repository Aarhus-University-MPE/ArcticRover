/*
  GeoRover Heartbeat functions.
  Send and receive heartbeat to primary CPU.

  Mads Rosenhøj Jeppesen
  Aarhus University
  2021
*/

unsigned long lastMillisResetPrimary = 0;
bool receivedHeartBeat = false;

// Runs heartbeat in and out
void HeartBeat() {
  HeartBeatOut();
  HeartBeatIn();
  HeartBeatTimeout();
}

unsigned long lastMillisHeartbeatOut = 0;
// Sends heartbeat with with frequency of HRTBEAT_FRQ
void HeartBeatOut() {
  if (millis() - lastMillisHeartbeatOut > HRTBEAT_DT_OUT) {
    DEBUG_PRINTLN("Sending Heartbeat!");
    lastMillisHeartbeatOut = millis();
    digitalWrite(PO_PRIMARY_HRTBEAT, false);
    delay(20);
    digitalWrite(PO_PRIMARY_HRTBEAT, true);

    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
  }
}

unsigned long lastMillisHeartbeatIn = 0;
// Checks if time since last heartbeat received > maximum treshold
// Will attempt to reset PRIMARY CPU with a frequency of  PRIMARY_RST_FRQ
void HeartBeatTimeout() {
  if (millis() - lastMillisHeartbeatIn > HRTBEAT_TRESHOLD) {
    // Reset PRIMARY CPU
    if (GetStatus(MODULE_PRIMARYCPU)) {
      lastMillisResetPrimary = millis();
      SetStatus(MODULE_PRIMARYCPU, false);
      DEBUG_PRINTLN("Error: Primary System offline, attempting to Reset.");
      ResetPrimaryCPU();
    } else {
      if (millis() - lastMillisResetPrimary > BACKUP_RST_DT) {
        lastMillisResetPrimary = millis();
        DEBUG_PRINTLN("Error: Primary System offline, attempting to Reset.");
        ResetPrimaryCPU();
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

  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);

  lastMillisHeartbeatIn = millis();
  DEBUG_PRINTLN("Heartbeat Received!");
  if (!GetStatus(MODULE_PRIMARYCPU)) {
    SetStatus(MODULE_PRIMARYCPU, true);
    DEBUG_PRINTLN("Primary System online");
  }
}

// Set Heartbeat flag
void HeartBeatInInterrupt() {
  receivedHeartBeat = true;
}

// Attempt to reset primary CPU
void ResetPrimaryCPU() {
  digitalWrite(PO_PRIMARY_RST, false);
  delay(20);
  digitalWrite(PO_PRIMARY_RST, true);
}
