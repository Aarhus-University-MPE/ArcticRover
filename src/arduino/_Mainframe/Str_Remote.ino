/*  Strategy - Remote control

    Allows manual control of motors
*/
bool remoteActive;
bool systemActive;
unsigned long lastSystemCheck = 9999999;

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

  if (!RemoteSystemCheck()) {
    remoteActive = false;
    SystemDisable(MODULE_MOTORS);
    LedBlinkHalt(BINARY_CODE_LED_RED, LED_BLINK_LONG);
    return;
  }

  StrategyRunLed(MODE_AUTONOMOUS);

  // Read RF signal
  sbus.process();
  ProcessRf();

  // Transmit via CAN
  CanBusProcess();
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
    remoteActive     = !remoteActive;
  }
}

// Read RF signal and move motors accordingly
void ProcessRf() {
  if (getChannel(6) < REMOTE_CHANNEL_HIGH) {  // Enable (SF)
    MotorUpdate(0, 0, 0);
    return;
  }

  float throttle1 = getChannelFloatFull(1);  // Left stick Vertical
  float dir1      = getChannelFloat(2);      // Left stick Horisontal
  float throttle2 = getChannelFloat(3);      // Right stick Vertical
  float dir2      = getChannelFloat(4);      // Right stick Horisontal
  int gear        = getChannel(5);           // Gear select (SA)

  int forwardDir;
  float speed, dir;
  bool enabled;

  // Primary input (Left Stick)
  if (throttle1 > CONTROLLER_DEADZONE_FLOAT) {
    if (gear < REMOTE_CHANNEL_LOW) {
      speed = throttle1 * MOTOR_MAX_SPEED_BWD;
    } else if (gear > REMOTE_CHANNEL_HIGH) {
      speed = throttle1 * MOTOR_MAX_SPEED_FWD;
    }
    dir = dir1;
    enabled = true;
  }
  // Secondary input (Right Stick)
  else if (abs(throttle2) > CONTROLLER_DEADZONE_FLOAT) {
    if (throttle2 > 0) {
      speed = throttle2 * MOTOR_MAX_SPEED_FWD / 4;
    } else {
      speed = throttle2 * MOTOR_MAX_SPEED_BWD / 4;
    }
    dir = dir2;
    enabled = true;
  }

  MotorUpdate(dir, speed, enabled);
}

// System Check
bool RemoteSystemCheck() {
  if (millis() - lastSystemCheck > SYSTEM_CHECK_DT) {
    lastSystemCheck = millis();
    systemActive    = SystemCheck(MODE_REMOTECONTROL);
  }
  return systemActive;
}
