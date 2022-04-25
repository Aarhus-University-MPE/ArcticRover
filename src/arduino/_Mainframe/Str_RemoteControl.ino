/*  Strategy - Remote control

    Allows manual control of motors
*/

// Start sequence of strategy
void StartStrategyRemote() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (Remote): Starting"));
  SystemDisable();

  RemoteControlInitialize();

  AttachSelectButton();

  DEBUG_PRINTLN(F("Strategy (Remote): Initialized"));
  DEBUG_PRINTLINE();

  StrategyStartLed();
}

// Main sequence of strategy
void RunStrategyRemote() {
  RemoteControlProcess();

  RemoteControlStatusLed();
}

// End sequence of strategy
void FinishStrategyRemote() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (Remote): Ending"));

  DetachSelectButton();

  SystemDisable();

  DEBUG_PRINTLN(F("Strategy (Remote): Finished"));
}

// Select button function
void SelectFunctionRemote() {
  if (!SelectButtonDebounce()) {
    return;
  }

  lastSystemCheck = millis() - SYSTEM_CHECK_DT;
  remoteActive    = !remoteActive;
  remoteStart     = true;
}
