/*  Strategy - Autonomous

    Primary Strategy for Autonomous Navigation
*/
bool autonomyActive;

// Start sequence of strategy
void StartStrategyAutonomous() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (Autonomous): Starting."));

  SystemEnableMode();

  AttachSelectButton();

  DEBUG_PRINTLN(F("Strategy (Autonomous): Initialized."));
  DEBUG_PRINTLINE();

  StrategyStartLed();
}

// Main sequence of strategy
void RunStrategyAutonomous() {
  if (!autonomyActive) {
    SystemDisable(MODULE_MOTORS);
    StrategyRunLed(MODE_IDLE);
    return;
  }

  if (!Navigate()){
    autonomyActive = false;
    SystemDisable(MODULE_MOTORS);
    LedBlinkHalt(BINARY_CODE_LED_RED, LED_BLINK_LONG);
    return;
  }

  SystemEnable(MODULE_PWR_MOTOR);
  SystemEnable(MODULE_MOTORS);

  // Blink light
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
    autonomyActive     = !autonomyActive;
  }
}