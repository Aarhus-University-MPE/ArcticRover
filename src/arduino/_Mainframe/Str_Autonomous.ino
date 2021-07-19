/*  Autonomous mode

    Primary Strategy for Autonomous Navigation 
*/

// Start sequence of strategy
void StartStrategyAutonomous() {
  DEBUG_PRINTLN("Strategy (Autonomous): Starting.");
  delay(20);

  // Enable Primary System Power
  digitalWrite(PO_POWER_5V,    HIGH);

  DEBUG_PRINTLN("Power to Primary Systems Enabled.");
  delay(20);

  // Enable Secondary System Power
  digitalWrite(PO_POWER_MOTOR,    HIGH);
  digitalWrite(PO_POWER_12V,      HIGH);

  DEBUG_PRINTLN("Power to Secondary Systems Enabled.");
  delay(20);
  
  // Initialize Long Range Communication
  digitalWrite(PO_POWER_IRIDIUM,  HIGH);
  InitializeIridium();

  DEBUG_PRINTLN("Long Range Communication Enabled.");
  delay(20);
  
  // Disable RF Communication
  digitalWrite(PO_POWER_RF,       LOW);

  DEBUG_PRINTLN("Short Range Communication Disabled.");
  delay(20);

  // Attach Select Button
  attachInterrupt(PI_BUTTON_SELECT,SelectButtonAutonomousInterruptHandler,FALLING);

  DEBUG_PRINTLN("System Input Button Enabled.");
  delay(20);

  DEBUG_PRINTLN("Strategy (Autonomous): Initialized.");
}

// Main sequence of strategy
void RunStrategyAutonomous() {
  // Blink light

}

// End sequence of strategy
void FinishStrategyAutonomous() {
  DEBUG_PRINTLN("Strategy (Autonomous): Ending.");
  delay(20);

  // Disable all outputs
  digitalWrite(PO_POWER_MOTOR,    LOW);
  digitalWrite(PO_POWER_12V,      LOW);
  digitalWrite(PO_POWER_RF,       LOW);
  digitalWrite(PO_POWER_IRIDIUM,  LOW);

  DEBUG_PRINTLN("Power to Secondary Systems Disabled.");
  delay(20);

  DEBUG_PRINTLN("Strategy (Autonomous): Finished.")
}

/* -------------------------------------------------
    Functions
------------------------------------------------- */


// Enable/Disable motor power
void SelectButtonAutonomousInterruptHandler() {
  
}