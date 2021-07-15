/*  XX mode

    Description...
*/

// Start sequence of strategy
void StartStrategyExample() {
  DEBUG_PRINT("Strategy: Emergency Starting...");
  DBG_ONLY(delay(500));

  delay(20);

  // Disable all outputs

  DEBUG_PRINTLN("all outputs disabled.");
}

// Main sequence of strategy
void RunStrategyExample() {
  // Blink light

}

// End sequence of strategy
void FinishStrategyExample() {
  DEBUG_PRINTLN("Strategy: Emergency ending.");

  // Disable light
}
