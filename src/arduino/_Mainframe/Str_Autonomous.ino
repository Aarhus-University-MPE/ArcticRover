/*  Strategy - Autonomous

    Primary Strategy for Autonomous Navigation
*/

// Start sequence of strategy
void StartStrategyAutonomous() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (Autonomous): Starting."));

  SystemDisable();

  // TODO: Test reboot sequence (autonomyActive bool saved to EEPROM)
  // Skip reset in case of reboot.
  // if (!AutonomyReboot()) AutonomyReset(); // Uncomment once autonomyActive EEPROM has been tested

  AutonomyReset();  // TODO: Remove once AutonomyReset() above is uncommented

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

  AutonomyReset();

  SystemDisable();

  DEBUG_PRINTLN(F("Strategy (Autonomous): Finished."));
  DEBUG_PRINTLINE();
}

// Select button function, starts autonomous driving
void SelectFunctionAutonomous() {
  if (!SelectButtonDebounce()) {
    return;
  }

  AutonomyToggle();
}
