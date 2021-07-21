/*  Strategy - System Test mode

    Runs system test...
*/

// Start sequence of strategy
void StartStrategySystemTest() {
  DEBUG_PRINTLN("Strategy (System Test): Starting");
  StrategyStartLed(MODE_SYSTEMTEST);

  SystemDisable();

  AttachSelectButton();

  DEBUG_PRINTLN("Strategy (System Test): Initialized");
  LedBlinkDoubleShort(BINARY_CODE_LED_GRN);
}

// Main sequence of strategy
void RunStrategySystemTest() {
  // Blink light
  StrategyRunLed(MODE_SYSTEMTEST);
  
}

// End sequence of strategy
void FinishStrategySystemTest() {
  DEBUG_PRINTLN("Strategy (System Test): Ending");
  
  DetachSelectButton();
  SystemDisable();

  DEBUG_PRINTLN("Strategy (System Test): Finished");
}

// Selecet button function
void SelectFunctionSystemTest(){
  if (millis() - lastMillisSelect > BTN_DEBOUNCE_TIME)
  {
    lastMillisSelect = millis();

    DEBUG_PRINTLN("Running Full System Test");
    SystemEnable();

    GetStatus();
    DEBUG_PRINT("System Status: ");
    DEBUG_PRINTLN(String(ToByte(SystemStatus)));

    DEBUG_PRINTLN("System Test Complete");

    SystemDisable();
  }

}