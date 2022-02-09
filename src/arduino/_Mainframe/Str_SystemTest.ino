/*  Strategy - System Test mode

    Runs system test...
*/

// Start sequence of strategy
void StartStrategySystemTest() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN("Strategy (System Test): Starting");
  StrategyStartLed(MODE_SYSTEMTEST);

  SystemDisable();

  AttachSelectButton();

  DEBUG_PRINTLN("Strategy (System Test): Initialized");
  DEBUG_PRINTLINE();
  StrategyStartLed(MODE_SYSTEMTEST);
}

// Main sequence of strategy
bool runTest = false;
void RunStrategySystemTest() {
  // Blink light
  StrategyRunLed(MODE_SYSTEMTEST);

  if (runTest) {
    if(SystemTest()) runTest = false;
  }
}

// End sequence of strategy
void FinishStrategySystemTest() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN("Strategy (System Test): Ending");

  DetachSelectButton();
  SystemDisable();

  DEBUG_PRINTLN("Strategy (System Test): Finished");
}


// Selecet button function
void SelectFunctionSystemTest() {
  if (millis() - lastMillisSelect > BTN_DEBOUNCE_TIME) {
    systemTestState = 0;
    lastMillisSelect = millis();
    runTest = true;
    DetachSelectButton();
  }
}

