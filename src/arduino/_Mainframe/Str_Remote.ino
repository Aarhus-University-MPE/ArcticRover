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
    DEBUG_PRINTLINE();
    DEBUG_PRINTLN(F("Starting Remote Control"));
    DEBUG_PRINTLINE();
    if(!SystemEnableMode()){
      DEBUG_PRINTLINE();
      DEBUG_PRINTLN(F("Module Start Error"));
      DEBUG_PRINTLINE();
      RemoteHalt();
      return;
    }
  }

  if (!SystemCheckMode(MODE_REMOTECONTROL)) {
    RemoteHalt();
    return;
  }

  StrategyRunLed(MODE_AUTONOMOUS);

  // Read RF signal
  SBusProcess();
  if (!SBusProcess()) {
    RemoteHalt();
    return;
  }
  SBusPrint();

  // Transmit via CAN
  CanBusProcess();
  if (!CanBusProcess()) {
    RemoteHalt();
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

void RemoteHalt() {
  remoteActive = false;
  SystemDisable();
  StatusHaltLed(LED_SIGNAL_ERROR);
}
