/*  Strategy - Emergency mode

    Disable all outputs and activate light.
    Detatch interrupts from input buttons, to not exit mode unless emergency reset.
    No actions until mode end.
*/

// Start sequence of strategy
void StartStrategyEmergency() {
  DEBUG_PRINTLN("Strategy (Emergency): Starting");
  delay(20);

  // Disable all outputs
  SystemDisable();
  delay(20);

  // Disable input buttons
  detachInterrupt(PI_BUTTON_MODE);
  detachInterrupt(PI_BUTTON_SELECT);
  
  DEBUG_PRINTLN("Strategy (Emergency): System Inputs Disabled");
  delay(20);

  DEBUG_PRINTLN("Strategy (Emergency): Initialized");
}

// Main sequence of strategy
void RunStrategyEmergency() {
  StrategyRunLed(MODE_EMERGENCY);

  // Exit strategy, if emergency not set -> Idle strategy
  if (!digitalRead(PI_BUTTON_ESTOP)) {
    LedBlink(BINARY_CODE_LED_GRN, ESTOP_DEBOUNCE_TIME / 2, ESTOP_DEBOUNCE_TIME / 2);
    if (!digitalRead(PI_BUTTON_ESTOP)) {
      LedBlinkDoubleShort(BINARY_CODE_LED_GRN);
      SetMode(MODE_IDLE);
    }
  }
}

// End sequence of strategy
void FinishStrategyEmergency() {
  DEBUG_PRINTLN("Strategy (Emergency): Ending");

  // Enable input buttons
  attachInterrupt(PI_BUTTON_MODE, ModeButtonInterruptHandler, FALLING);

  DEBUG_PRINTLN("Strategy (Emergency): Finished");

  // Disable light
}


void SelectFunctionEmergency(){

}