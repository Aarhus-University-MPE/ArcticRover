/*  Idle mode

    Description...
*/

// Start sequence of strategy
void StartStrategyIdle() {
  DEBUG_PRINT("Strategy: Idle Starting... ");

  delay(20);

  // Disable all outputs
  digitalWrite(PO_POWER_MOTOR,    LOW);
  digitalWrite(PO_POWER_12V,      LOW);
  digitalWrite(PO_POWER_RF,       LOW);
  digitalWrite(PO_POWER_IRIDIUM,  LOW);

  DEBUG_PRINT("High Power Outputs Disabled... ");

  // Attach mode button
  attachInterrupt(PI_BUTTON_MODE, ModeButtonInterruptHandler, FALLING);
  DEBUG_PRINTLN("Mode Enabled.");
}

// Main sequence of strategy
void RunStrategyIdle() {
  LedBlink(BINARY_CODE_LED_GRN, LED_BLINK_LONG, LED_BLINK_VERY_LONG);

}

// End sequence of strategy
void FinishStrategyIdle() {
  DEBUG_PRINTLN("Strategy: Idle ending.");

}
