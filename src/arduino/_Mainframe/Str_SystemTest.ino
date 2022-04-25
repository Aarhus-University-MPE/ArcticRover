/*  Strategy - System Test mode

    Runs full system test ...
*/
bool runTest = false;

// Start sequence of strategy
void StartStrategySystemTest() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (System Test): Starting"));
  SystemDisable();

  AttachSelectButton();

  DEBUG_PRINTLN(F("Strategy (System Test): Initialized"));
  DEBUG_PRINTLINE();
  StrategyStartLed();
}

// Main sequence of strategy
void RunStrategySystemTest() {
  // Blink light
  StrategyRunLed();

  if (runTest) {
    runTest = !SystemTest();
  }
}

// End sequence of strategy
void FinishStrategySystemTest() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (System Test): Ending"));

  DetachSelectButton();
  SystemDisable();

  DEBUG_PRINTLN(F("Strategy (System Test): Finished"));
}

// Selecet button function
void SelectFunctionSystemTest() {
  if (!SelectButtonDebounce()) {
    return;
  }

  systemTestState = 0;
  runTest         = true;
  DetachSelectButton();
}
