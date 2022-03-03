/*  Strategy - Remote control

    Allows manual control of motors
*/
bool remoteActive;
bool systemActive;

// Start sequence of strategy
void StartStrategyRemote() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN("Strategy (Remote): Starting");
  StrategyStartLed(MODE_REMOTECONTROL);

  SystemDisable();
  delay(20);

  SystemEnableMode(MODE_REMOTECONTROL);

  // Enable motor control
  AttachSelectButton();

  DEBUG_PRINTLN("Strategy (Remote): Initialized");
  DEBUG_PRINTLINE();
}

unsigned long lastSystemCheck = 9999999;
// Main sequence of strategy
void RunStrategyRemote() {
  if (!remoteActive) {
    SystemDisable(MODULE_MOTORS);
    return;
  }

  SystemEnableMode(MODE_REMOTECONTROL);

  SystemEnable(MODULE_MOTORS);

  if (!RemoteSystemCheck()) {
    remoteActive = false;
    return;
  }

  // Read RF signal
  sbus.process();
  ProcessIncomingCommands();

  // Transmit via CAN
  CanBusProcess();
}

// End sequence of strategy
void FinishStrategyRemote() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN("Strategy (Remote): Ending");

  DetachSelectButton();

  if (GetStatus(MODULE_MOTOR_ACT)) {
    TerminateMotors();
  }

  SystemDisable();

  LedBlinkDoubleShort(BINARY_CODE_LED_RED);
  DEBUG_PRINTLN("Strategy (Remote): Finished");
}

// Select button function
void SelectFunctionRemote() {
  if (millis() - lastMillisSelect > BTN_DEBOUNCE_TIME) {
    lastMillisSelect = millis();
    remoteActive = !remoteActive;
  }
}

// Read RF signal and move motors accordingly
unsigned long lastProcessCommand = 0;
void ProcessIncomingCommands() {
  if (millis() - lastProcessCommand > REMOTE_PROCESS_DT) {
    lastProcessCommand = millis();
    float throttle = getChannelFloat(REMOTE_CHANNEL_THROTTLE);
    float steer = getChannelFloat(REMOTE_CHANNEL_STEER);
    float enable = getChannelFloat(REMOTE_CHANNEL_ENABLE);

    // move motors
    MotorUpdate(steer, throttle, enable);
  }
}

// System Check
bool RemoteSystemCheck() {
  if (millis() - lastSystemCheck > SYSTEM_CHECK_DT) {
    lastSystemCheck = millis();
    systemActive = SystemCheck(MODE_REMOTECONTROL);
  }
  return systemActive;
}