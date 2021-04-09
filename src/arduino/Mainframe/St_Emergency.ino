/*  Strategy - Emergency mode
    
    Disable all outputs and activate light.
    Detatch interrupts from input buttons, to not exit mode unless emergency reset.
    No actions until mode end.
*/

// Start sequence of strategy
void StartStrategyEmergency() {
  DEBUG_PRINT("Strategy: Emergency starting... ");
  DBG_ONLY(delay(500));  
  delay(20);
  
  // Disable all outputs
  digitalWrite(PO_POWER_MOTOR, LOW);
  digitalWrite(PO_POWER_12V,   LOW);
  digitalWrite(PO_POWER_5V,    LOW);
  DEBUG_PRINT("Power supply disabled... ");
  delay(20);

  // Disable input buttons
  detachInterrupt(PI_BUTTON_MODE);
  detachInterrupt(inputBtn2);
  DEBUG_PRINTLN("Inputs disabled.");
  
}

// Main sequence of strategy
void RunStrategyEmergency() {
  // Blink light
  LedBlink(BINARY_CODE_LED_RED, LED_BLINK_LONG, LED_BLINK_VERY_LONG){
  
}

// End sequence of strategy
void FinishStrategyEmergency() {
  DEBUG_PRINT("Strategy: Emergency ending... ");
  
  // Enable Power
  digitalWrite(pwrMotor,   HIGH);
  digitalWrite(pwr12VMain, HIGH);
  digitalWrite(pwr5VMain,  HIGH)
  
  DEBUG_PRINT("Power enabled... ")

  // Enable input buttons
  attachInterrupt(PI_BUTTON_MODE,ModeButtonInterruptHandler, FALLING);
  attachInterrupt(inputBtn2,ModeButtonInterruptHandler, FALLING);
  
  DEBUG_PRINTLN("All systems online.");

  // Disable light
}





