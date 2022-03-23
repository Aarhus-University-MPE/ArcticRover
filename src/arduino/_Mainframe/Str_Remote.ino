/*  Strategy - Remote control

    Allows manual control of motors
*/
bool remoteActive, remoteStart;

// Start sequence of strategy
void StartStrategyRemote() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (Remote): Starting"));
  SystemDisable();


  remoteActive = false;
  remoteStart  = false;

  AttachSelectButton();

  DEBUG_PRINTLN(F("Strategy (Remote): Initialized"));
  DEBUG_PRINTLINE();

  StrategyStartLed();
}

// Main sequence of strategy
void RunStrategyRemote() {
  if (!remoteActive) {
    SystemDisable();
    StrategyRunLed(MODE_IDLE);
    return;
  }

  if (remoteStart) {
    remoteStart = false;
    SystemEnableMode();
  }

  if (!SystemCheckMode(MODE_REMOTECONTROL)) {
    remoteActive = false;
    SystemDisable(MODULE_MOTORS);
    LedBlinkHalt(BINARY_CODE_LED_RED, LED_BLINK_LONG);
    return;
  }

  // StrategyRunLed(MODE_AUTONOMOUS);

  // Read RF signal
  sbus.process();
  if (!SBusProcess()) {
    remoteActive = false;
    SystemDisable();
    StatusHaltLed(SIGNAL_ERROR);
    return;
  }
  SBusPrint();

  // Transmit via CAN
  if (!CanBusProcess()) {
    remoteActive = false;
    SystemDisable();
    StatusHaltLed(SIGNAL_ERROR);
    return;
  }
  CanBusPrint();
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
  if (millis() - lastMillisSelect > BTN_DEBOUNCE_TIME) {
    lastMillisSelect = millis();
    lastSystemCheck  = millis() - SYSTEM_CHECK_DT;
    remoteActive     = !remoteActive;
    remoteStart      = true;
  }
}
