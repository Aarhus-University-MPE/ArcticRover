/*  Idle mode

    Description...
*/

// Start sequence of strategy
void StartStrategyIdle() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (Idle): Starting"));

  SystemDisable();

  AttachSelectButton();

  DEBUG_PRINTLN(F("Strategy (Idle): Initialized"));
  DEBUG_PRINTLINE();

  StrategyStartLed();
}

// Main sequence of strategy
void RunStrategyIdle() {
  StrategyRunLed();
  // HeatingProcess();
}

// End sequence of strategy
void FinishStrategyIdle() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (Idle): Ending"));
  SystemDisable();

  DEBUG_PRINTLN(F("Strategy (Idle): Finished"));
}

// Selecet button function
void SelectFunctionIdle() {
}