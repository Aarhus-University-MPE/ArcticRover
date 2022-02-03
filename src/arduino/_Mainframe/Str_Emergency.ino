/*  Strategy - Emergency mode

    Disable all outputs and activate light.
    Detatch interrupts from input buttons, to not exit mode unless emergency reset.
    No actions until mode end.
*/

// Start sequence of strategy
void StartStrategyEmergency() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN("Strategy (Emergency): Starting");

  // Disable all outputs
  SystemDisable();
  SetStatus(MODULE_ESTOP, false);

  // Disable input buttons
  detachInterrupt(PI_BUTTON_MODE);
  detachInterrupt(PI_BUTTON_SELECT);
  DEBUG_PRINTLN("Strategy (Emergency): Systems Disabled");

  DEBUG_PRINTLN("Strategy (Emergency): Initialized");
  DEBUG_PRINTLINE();
}

// Main sequence of strategy
void RunStrategyEmergency() {
  StrategyRunLed(MODE_EMERGENCY);

  // Exit strategy, if emergency not set -> Idle strategy
  if (EmergencyStopStatus()) {
    LedBlink(BINARY_CODE_LED_GRN, ESTOP_DEBOUNCE_TIME / 2, ESTOP_DEBOUNCE_TIME / 2);
    if (EmergencyStopStatus()) {
      LedBlinkDoubleShort(BINARY_CODE_LED_GRN);
      SetMode(MODE_IDLE);
    }
  }
}

// End sequence of strategy
void FinishStrategyEmergency() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN("Strategy (Emergency): Ending");

  lastMillisMode = millis();
  lastMillisEstop = millis();

  // Enable input buttons
  attachInterrupt(PI_BUTTON_MODE, ModeButtonInterruptHandler, FALLING);

  // Assign Emergency Stop button interrupt
  attachInterrupt(PI_INT_BUTTON_ESTOP, EstopButtonInterruptHandler, FALLING);
  SetStatus(MODULE_ESTOP, true);

  DEBUG_PRINTLN("Strategy (Emergency): Finished");

  // Disable light
}

void SelectFunctionEmergency() {
}

// Returns TRUE if Emergency Button is NOT pressed
// Returns FALSE if Emergency Button IS pressed
bool EmergencyStopStatus() {
  return digitalRead(PI_BUTTON_ESTOP);
}