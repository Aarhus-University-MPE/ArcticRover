/*
  GeoRover Autonomy Stack

  Mads Rosenhøj Jeppesen
  Aarhus University
  2021
*/

bool autonomyStart, autonomyRestart, autonomyActive, standbyMode;
unsigned long millisLastAutonomyCycle, millisLastRunCheck;

// Primary Autonomous Algorithm
void AutonomousProcess() {
  // Autonomy not active -> Check Current Power Levels, Communication and Navigation
  if (!AutonomyPreRun()) return;

  // Autonomy active -> Check Power Levels, standby if below min, resume if above std recharge level
  if (!AutonomyPowerCycle()) return;

  // Reboot systems if waking up from standby, await PreChecks before continuing
  if (!StandbyRestart()) return;

  // Runtime system check GNSS status, IMU tilt
  // if (!AutonomyRunCheck()) return;

  // Autonomous Navigation towards waypoints based on current GNSS position and heading
  Navigate();

  NavigationPrint();

  SBusAutonomyProcess();  // TODO: Remove Manual Control

  // Transmit motor controls based on Navigation alogrithm
  CanBusProcess();  // TODO: Handle CAN Error

  CanBusPrint();
}

// Runs necessary autonomy pre-runs (Power, Communication, Navigation)
bool AutonomyPreRun() {
  if (autonomyActive) return true;

  if (!AutonomyPreCheck()) return false;

  // System ready to start, waiting idle
  if (!autonomyActive) {
    StatusRunLed(LED_SIGNAL_LOADING);
    // TODO: Print ready
    return false;
  }

  // Select button pressed, start navigation
  if (autonomyStart) {
    autonomyStart       = false;
    millisAutonomyStart = millis();
    DEBUG_PRINTLINE();
    DEBUG_PRINTLN(F("Starting Autonomous Driving"));
    DEBUG_PRINTLINE();
    AutonomySpeedUpdate(0);
  }

  return true;
}

/*
  Power Cycle Algorithm:
    if current power level (CPL) < min Level -> Set standby (start charging)
    if currently on charging and CPL > BATTERY_STD_RECHARGE, awake from standby
*/
bool AutonomyPowerCycle() {
  if (millis() - millisLastAutonomyCycle < AUTONOMY_PWR_CYCLE_DT) return !standbyMode;

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
    StatusHaltLed(LED_SIGNAL_ERROR);
    return false;
  }

  // Runs navigation route checks
  if (!NavigationPreCheck()) {
    return false;
  }
  return true;
}

// Check system status while navigation is running.
// GNSS status, Accelerometer output (tilt?), Battery Status
bool AutonomyRunCheck() {
  if (millis() - millisLastRunCheck < NAVIGATION_RUNCHECK_DT) {
    return true;
  }

  millisLastRunCheck = millis();

  return SystemCheckMode(MODE_AUTONOMOUS);
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
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Powering Down System"));
  DEBUG_PRINTLINE();
  MotorUpdate(0, 0);
  MotorCycle();
  SystemDisable();
}

// Power levels acceptable, if coming out of standby, reboot systems and await PreCheck flag
bool StandbyRestart() {
  if (autonomyRestart) {
    autonomyRestart = false;
    DEBUG_PRINTLINE();
    DEBUG_PRINTLN(F("Restarting System from Power Down"));
    DEBUG_PRINTLINE();
    NavigationPreCheckReset();
    AutonomySpeedUpdate(0);
  }

  if (!AutonomyPreCheck()) {
    return false;
  }
  return true;
}
