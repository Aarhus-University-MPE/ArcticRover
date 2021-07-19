/*  Idle mode

    Description...
*/

// Start sequence of strategy
void StartStrategyIdle() {
  DEBUG_PRINTLN("Strategy (Idle): Starting");
  delay(20);

  // Disable all outputs
  SystemDisable();

  DEBUG_PRINTLN("Strategy (Idle): Initialized");
  StrategyStartLed(MODE_IDLE);
}

// Main sequence of strategy
void RunStrategyIdle() {
  LedBlink(BINARY_CODE_LED_YEL, LED_BLINK_LONG, LED_BLINK_VERY_LONG);

}

// End sequence of strategy
void FinishStrategyIdle() {
  DEBUG_PRINTLN("Strategy (Idle): Ending");
  //SystemDisable(MODULE_PWR_MOTOR);

  DEBUG_PRINTLN("Strategy (Idle): Finished");
}
