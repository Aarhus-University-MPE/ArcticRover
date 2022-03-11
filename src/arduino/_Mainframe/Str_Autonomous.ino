/*  Autonomous mode

    Primary Strategy for Autonomous Navigation 
*/

// Start sequence of strategy
void StartStrategyAutonomous() {
  DEBUG_PRINTLN(F("Strategy (Autonomous): Starting."));

  SystemEnableMode();

  AttachSelectButton();

  DEBUG_PRINTLN(F("Strategy (Autonomous): Initialized."));
}

// Main sequence of strategy
void RunStrategyAutonomous() {
  // Blink light
  StrategyRunLed();
}

// End sequence of strategy
void FinishStrategyAutonomous() {
  DEBUG_PRINTLN(F("Strategy (Autonomous): Ending."));
  delay(20);

  DetachSelectButton();

  // Disable all outputs
  SystemDisable();
  delay(20);

  DEBUG_PRINTLN(F("Strategy (Autonomous): Finished."));
}

/* -------------------------------------------------
    Functions
------------------------------------------------- */

// Enable/Disable motor power
void SelectFunctionAutonomous() {
  // Start Driving
}