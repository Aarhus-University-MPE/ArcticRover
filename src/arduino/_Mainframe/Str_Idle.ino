/*  Idle mode

    Description...
*/

// Start sequence of strategy
void StartStrategyIdle() {
  DEBUG_PRINTLN("Strategy (Idle): Starting");
  StrategyStartLed(MODE_IDLE);

  // Disable all outputs
  SystemDisable();

  AttachSelectButton();

  DEBUG_PRINTLN("Strategy (Idle): Initialized");
  LedBlinkDoubleShort(BINARY_CODE_LED_GRN);
}

// Main sequence of strategy
void RunStrategyIdle() {
  StrategyRunLed(MODE_IDLE);
}

// End sequence of strategy
void FinishStrategyIdle() {
  DEBUG_PRINTLN("Strategy (Idle): Ending");
  SystemDisable();

  DEBUG_PRINTLN("Strategy (Idle): Finished");
}

// Selecet button function
void SelectFunctionIdle(){
  StrategyStartLed(MODE_IDLE);
}