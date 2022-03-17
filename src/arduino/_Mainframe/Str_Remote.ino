/*  Strategy - Remote control

    Allows manual control of motors
*/
bool remoteActive;

// Start sequence of strategy
void StartStrategyRemote() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (Remote): Starting"));
  SystemDisable();

  SystemEnableMode();

  AttachSelectButton();

  DEBUG_PRINTLN(F("Strategy (Remote): Initialized"));
  DEBUG_PRINTLINE();

  StrategyStartLed();
}

// Main sequence of strategy
void RunStrategyRemote() {
  if (!remoteActive) {
    SystemDisable(MODULE_MOTORS);
    StrategyRunLed(MODE_IDLE);
    return;
  }

  SystemEnable(MODULE_PWR_MOTOR);
  SystemEnable(MODULE_MOTORS);
  SystemEnable(MODULE_RF);
  SystemEnable(MODULE_CANBUS);

  // if (!SystemCheck(MODE_REMOTECONTROL)) {
  //   remoteActive = false;
  //   SystemDisable(MODULE_MOTORS);
  //   LedBlinkHalt(BINARY_CODE_LED_RED, LED_BLINK_LONG);
  //   return;
  // }

  // StrategyRunLed(MODE_AUTONOMOUS);

  // Read RF signal
  sbus.process();
  SBusProcess();
  // SBusPrint();

  // Transmit via CAN
  CanBusProcess();
  //CanBusPrint();
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
  }
}
