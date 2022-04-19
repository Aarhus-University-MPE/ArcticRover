/*  Strategy - Autonomous

    Primary Strategy for Autonomous Navigation
*/

// Start sequence of strategy
void StartStrategyAutonomous() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (Autonomous): Starting."));

  SystemDisable();

  // TODO: Will reset autonomy upon reboot
  // - Add bool in EEPROM autonomous active, if true skip reset. Unset bool upon in FinishStrategyAutonomous
  AutonomyReset();

  AttachSelectButton();

  DEBUG_PRINTLN(F("Strategy (Autonomous): Initialized."));
  DEBUG_PRINTLINE();

  StrategyStartLed();
}

// Main sequence of strategy
void RunStrategyAutonomous() {
  AutonomousProcess();

  StrategyRunLed();
}

// End sequence of strategy
void FinishStrategyAutonomous() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (Autonomous): Ending."));

  DetachSelectButton();

  // TODO: Add unset autonomy bool in EEPROM once implemented

  SystemDisable();

  DEBUG_PRINTLN(F("Strategy (Autonomous): Finished."));
  DEBUG_PRINTLINE();
}

// Select button function, starts autonomous driving
void SelectFunctionAutonomous() {
  if (millis() - lastMillisSelect > BTN_DEBOUNCE_TIME) {
    lastMillisSelect = millis();
    AutonomyToggle();
  }
}
