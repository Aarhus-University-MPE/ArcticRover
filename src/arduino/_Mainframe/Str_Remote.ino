/*  Strategy - Remote control

    Allows manual control of motors
*/

// Start sequence of strategy
void StartStrategyRemote()
{
  DEBUG_PRINTLN("Strategy (Remote): Starting");
  StrategyStartLed(MODE_REMOTECONTROL);

  SystemDisable();
  
  // Enable Primary System Power
  SystemEnable(MODULE_PWR_5V);
  delay(20);

  SystemEnable(MODULE_RF);
  delay(20);

  // Enable motor control
  AttachSelectButton();
   
  DEBUG_PRINTLN("Strategy (Remote): Initialized");
  LedBlinkDoubleShort(BINARY_CODE_LED_GRN);
}

// Main sequence of strategy
void RunStrategyRemote()
{

  // Read RF signal
  if (GetStatus(MODULE_MOTOR)){
    ProcessIncomingCommands();
  }
}

// End sequence of strategy
void FinishStrategyRemote()
{
  DEBUG_PRINTLN("Strategy (Remote): Ending");
  
  DetachSelectButton();
  
  if (GetStatus(MODULE_MOTOR_EN))
  {
    TerminateMotors(true);
  }
  
  SystemDisable();


  LedBlinkDoubleShort(BINARY_CODE_LED_RED);
  DEBUG_PRINTLN("Strategy (Remote): Finished");
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


// Select button function
void SelectFunctionRemote(){
  if (millis() - lastMillisSelect > BTN_DEBOUNCE_TIME)
  {
    lastMillisSelect = millis();

    if (!GetStatus(MODULE_MOTOR_EN))
    {
      SystemEnable(MODULE_MOTOR);

      LedBlinkDoubleShort(BINARY_CODE_LED_GRN);
    }
    else
    {
      SystemDisable(MODULE_MOTOR);

      LedBlink(BINARY_CODE_LED_RED, LED_BLINK_LONG, 0);
    }
  }
}