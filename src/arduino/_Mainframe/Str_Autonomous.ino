/*  Autonomous mode

    Primary Strategy for Autonomous Navigation 
*/

// Start sequence of strategy
void StartStrategyAutonomous() {
  DEBUG_PRINTLN("Strategy (Autonomous): Starting.");
  delay(20);

  // Enable Primary System Power
  digitalWrite(PO_POWER_5V, HIGH);
  delay(20);

  // Initialize Long Range Communication
  SystemEnable(MODULE_IRIDIUM);
  delay(20);

  // Attach Select Button
  AttachSelectButton();
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

  DetachSelectButton();

  // Disable all outputs
  SystemDisable();
  delay(20);

  DEBUG_PRINTLN("Strategy (Autonomous): Finished.")
}

/* -------------------------------------------------
    Functions
------------------------------------------------- */

// Enable/Disable motor power
void SelectFunctionAutonomous() {
}