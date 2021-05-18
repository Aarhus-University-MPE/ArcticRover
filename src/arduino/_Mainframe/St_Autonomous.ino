/*  Autonomous mode
    
    Description...    
*/

// Start sequence of strategy
void StartStrategyAutonomous() {
  DEBUG_PRINT("Strategy: Autonomous Starting...");
  DBG_ONLY(delay(500));
  
  delay(20);
  
  // Disable all outputs

  DEBUG_PRINTLN("all outputs disabled.");
}

// Main sequence of strategy
void RunStrategyAutonomous() {
  // Blink light
  
}

// End sequence of strategy
void FinishStrategyAutonomous() {
  DEBUG_PRINTLN("Strategy: Autonomous ending.");
  
  // Disable light
}