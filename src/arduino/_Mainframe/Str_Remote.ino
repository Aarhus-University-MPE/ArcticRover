/*  Strategy - Remote control

    Allows manual control of motors
*/

// Start sequence of strategy
void StartStrategyRemote()
{
  DEBUG_PRINTLN("Strategy (Remote): Starting.");

  StrategyStartLed(MODE_REMOTECONTROL);

  // Enable Primary System Power
  SystemEnable(MODULE_PWR_5V);
  delay(20);

  SystemEnable(MODULE_RF);
  delay(20);

  // Enable motor control
  attachInterrupt(PI_BUTTON_SELECT, SelectButtonRemoteInterruptHandler, FALLING);
  DEBUG_PRINTLN("Motor Remote Control ENABLED... Enable/Disable motors with Select Button.");

  LedBlinkDoubleShort(BINARY_CODE_LED_GRN);
}

// Main sequence of strategy
void RunStrategyRemote()
{

  // Read RF signal
  if (GetStatus(MODULE_MOTOR))
    ProcessIncomingCommands();

  HeartbeatRemote();
}

// End sequence of strategy
void FinishStrategyRemote()
{
  DEBUG_PRINT("Strategy: Remote Control ending... ");

  detachInterrupt(PI_BUTTON_SELECT);

  if (GetStatus(MODULE_MOTOR_EN))
  {
    TerminateMotors(true);
    digitalWrite(PO_POWER_MOTOR, LOW);
  }

  digitalWrite(PO_POWER_5V, LOW);
  digitalWrite(PO_POWER_RF, LOW);

  LedBlinkDoubleShort(BINARY_CODE_LED_RED);
  DEBUG_PRINTLN("Systems powered down... remote control DISABLED.");
}

// Enable/Disable motor power
void SelectButtonRemoteInterruptHandler()
{
  if (millis() - lastMillisSelect > BTN_DEBOUNCE_TIME)
  {
    lastMillisSelect = millis();

    if (!GetStatus(MODULE_MOTOR_EN))
    {
      digitalWrite(PO_POWER_MOTOR, HIGH);
      InitializeMotors(true);

      LedBlinkDoubleShort(BINARY_CODE_LED_GRN);
      SetStatus(MODULE_MOTOR_EN, true);
    }
    else
    {
      digitalWrite(PO_POWER_MOTOR, LOW);
      TerminateMotors(true);

      LedBlink(BINARY_CODE_LED_RED, LED_BLINK_LONG, 0);
      SetStatus(MODULE_MOTOR_EN, false);
    }
  }
}

// Read RF signal and move motors accordingly
void ProcessIncomingCommands()
{

  sbus.process(); // Read RF data stream

  float steer = getChannelFloat(1);
  float throttle = getChannelFloat(0);

  // move motors
  MotorMove(steer, throttle);
}

// Runs system checks
void HeartbeatRemote()
{
  DEBUG_PRINT("Remote heartbeat starting...");

  bool systemStable = true;    // Required
  bool subSystemStable = true; // Optional
  // Check Battery
  // Check Sensors
  // Check Motors

  if (systemStable && subSystemStable)
    LedBlinkSingleShort(BINARY_CODE_LED_GRN);
  else if (systemStable)
    LedBlinkSingleShort(BINARY_CODE_LED_YEL);
  else
    LedBlinkSingleShort(BINARY_CODE_LED_RED);

  DEBUG_PRINTLN("System stable.");
  SetStatus(MODULE_MOTOR, systemStable);
}
