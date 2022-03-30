/*
  GeoRover Autonomy Stack

  Mads Rosenhøj Jeppesen
  Aarhus University
  2021
*/

bool autonomyStart, autonomyRestart, autonomyActive, standbyMode;
unsigned long millisLastAutonomyCycle;

// Primary Autonomous Algorithm
void AutonomousProcess() {
  if (!AutonomyPreRun()) return;

  if (!AutonomyPowerCycle()) return;

  if (!StandbyRestart()) return;

  if (!CanBusProcess()) return;  // TODO: Handle CAN Error

  if (!Navigate()) return;  // TODO: Handle run-check Error
}

// Runs necessary autonomy pre-runs (Power, Communication, Navigation)
bool AutonomyPreRun() {
  if (autonomyActive) return true;

  if (!AutonomyPreCheck()) return false;

  // System ready to start, waiting idle
  if (!autonomyActive) {
    StatusRunLed(SIGNAL_LOADING);
    return false;
  }

  // Select button pressed, start navigation
  if (autonomyStart) {
    autonomyStart       = false;
    millisAutonomyStart = millis();
    DEBUG_PRINTLINE();
    DEBUG_PRINTLN(F("Starting Autonomous Driving"));
    DEBUG_PRINTLINE();
  }

  return true;
}

// Run power cycle, check if levels below thresholds or restart autonomy.
bool AutonomyPowerCycle() {
  if (millis() - millisLastAutonomyCycle < AUTONOMY_PWR_CYCLE_DT) return standbyMode;

  millisLastAutonomyCycle = millis();

  // Check power levels
  if (!PowerCycle()) {
    if (standbyMode) return false;
    AutonomyStandby();
    return false;
  }

  if (!standbyMode) {
    return true;
  }

  // Power levels sufficient, restart systems if previously powered down
  if (!StandbyRestart()) return false;

  standbyMode = false;
  return true;
}

// Runs necessary system pre-checks (enables systems, run Navigation pre-check)
bool AutonomyPreCheck() {
  // Enables primary and secondary systems
  if (!SystemEnableMode()) {
    autonomyActive = false;
    StatusHaltLed(SIGNAL_ERROR);
    return false;
  }

  // Runs navigation route checks
  if (!NavigationPreCheck()) {
    return false;
  }
  return true;
}


// Inverts current autonomy state
void AutonomyToggle() {
  autonomyActive = !autonomyActive;
  autonomyStart  = autonomyActive;
  NavigationPreCheckReset();
}

// Full autonomy reset
void AutonomyReset() {
  NavigationPreCheckReset();
  ResetNavigation();
  ResetPowerCycle();
  autonomyActive = false;
  autonomyStart  = false;
}

// Power down system
void AutonomyStandby() {
  standbyMode     = true;
  autonomyRestart = true;
  MotorUpdate(0, 0);
  MotorCycle();
  SystemDisable();
}

// Power levels acceptable, if coming out of standby, reboot systems and await PreCheck flag
bool StandbyRestart() {
  if (autonomyRestart) {
    autonomyRestart = false;
    NavigationPreCheckReset();
  }

  if (!AutonomyPreCheck()) {
    return false;
  }
  return true;
}

