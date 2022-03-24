/*  Strategy - Autonomous

    Primary Strategy for Autonomous Navigation
*/
bool autonomyActive, autonomyStart;

// Start sequence of strategy
void StartStrategyAutonomous() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (Autonomous): Starting."));

  SystemDisable();

  autonomyActive = false;
  autonomyStart  = false;

  AttachSelectButton();

  DEBUG_PRINTLN(F("Strategy (Autonomous): Initialized."));
  DEBUG_PRINTLINE();

  StrategyStartLed();
}

// Main sequence of strategy
void RunStrategyAutonomous() {
  // Run autonomous navigation precheck
  if (!SystemEnableMode()){
    StatusHaltLed(SIGNAL_ERROR);
    delay(500);
    return;
  }

  if (!NavigationPreCheck()) {
    return;
  }

  // System ready to start, waiting idle
  if (!autonomyActive) {
    StatusRunLed(SIGNAL_IDLE);
    return;
  }

  // Select button pressed, start navigation
  if (autonomyStart) {
    autonomyStart = false;
    if (!NavigationStart()) {
      SystemDisable(MODULE_MOTORS);
      StatusHaltLed(SIGNAL_ERROR);
      autonomyActive = false;
      return;
    }
  }

  // Autonomous navigation
  if (!Navigate()) {
    StatusHaltLed(SIGNAL_ERROR);
    autonomyActive = false;
    return;
  }

  StrategyRunLed();
}

// End sequence of strategy
void FinishStrategyAutonomous() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (Autonomous): Ending."));

  DetachSelectButton();

  SystemDisable();

  DEBUG_PRINTLN(F("Strategy (Autonomous): Finished."));
  DEBUG_PRINTLINE();
}

// Select button function, starts autonomous driving
void SelectFunctionAutonomous() {
  if (millis() - lastMillisSelect > BTN_DEBOUNCE_TIME) {
    lastMillisSelect = millis();
    autonomyActive   = !autonomyActive;
    autonomyStart    = true;
    NavigationPreCheckReset();
  }
}