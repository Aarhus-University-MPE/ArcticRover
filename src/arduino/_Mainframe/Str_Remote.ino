/*  Strategy - Remote control

    Allows manual control of motors
*/
bool remoteActive, remoteStart;

// Start sequence of strategy
void StartStrategyRemote() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (Remote): Starting"));
  SystemDisable();

  SystemEnableMode();
  
  motorLeft.SetTorqueMode(true);
  motorRight.SetTorqueMode(true);

  remoteActive = false;
  remoteStart = false;

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

  if(remoteStart){
    remoteStart = false;
    SystemEnable(MODULE_PWR_12V); // Temporary due to 12V DCDC replacement
    SystemEnable(MODULE_PWR_MOTOR);
    SystemEnable(MODULE_MOTORS);
    SystemEnable(MODULE_RF);
    SystemEnable(MODULE_CANBUS);
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
  SBusProcess();
  SBusPrint();

  // Transmit via CAN
  CanBusProcess();
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
