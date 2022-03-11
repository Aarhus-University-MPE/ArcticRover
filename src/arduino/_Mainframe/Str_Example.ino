/*  Strategy - Example

    Description...
*/

// Start sequence of strategy
void StartStrategyExample() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (Example): Starting"));
  SystemDisable();

  StrategyStartLed();

  SystemEnableMode();

  DEBUG_PRINTLN(F("Strategy (Example): Initialized"));
  DEBUG_PRINTLINE();
}

// Main sequence of strategy
void RunStrategyExample() {
  StrategyRunLed(); // Non-blocking LED blink
  
  // Add code which runs every loop for current strategy

}

// End sequence of strategy
void FinishStrategyExample() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (Example): Ending"));

  DetachSelectButton();

  SystemDisable();

  DEBUG_PRINTLN(F("Strategy (Example): Finished"));
}

// Select button function for current strategy
void SelectFunctionExample() {
  if (millis() - lastMillisSelect > BTN_DEBOUNCE_TIME) {
    lastMillisSelect = millis();
    // Add functionality here
  }
}

