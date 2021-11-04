/*  Strategy - Remote control

    Allows manual control of motors
*/

// Start sequence of strategy
void StartStrategyRemote()
{
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
  LedBlinkDoubleShort(BINARY_CODE_LED_GRN);
}

unsigned long lastSystemCheck = 9999999;
// Main sequence of strategy
void RunStrategyRemote()
{
  static bool systemActive = true;
  
  if (systemActive)
  {
    // System Check
    if(millis() - lastSystemCheck > SYSTEM_CHECK_DT){
      lastSystemCheck = millis();
      systemActive = SystemCheck(MODE_REMOTECONTROL);
    }

    //Read RF signal
    sbus.process();

    ProcessIncomingCommands();
  }
  else
  {
    // Attempt rebooting systems
    if (millis() - lastSystemReboot > SYSTEM_REBOOT_DT)
    {
      DEBUG_PRINTLN("Rebooting Subsystems");
      lastSystemReboot = millis();
      SystemEnableMode(MODE_REMOTECONTROL);

      systemActive = SystemCheck(MODE_REMOTECONTROL);
      if(systemActive){
        lastSystemReboot = 0;
        DEBUG_PRINTLN("Systems Restored");
      }
      DEBUG_PRINTLINE();
      
    }
  }
}

// End sequence of strategy
void FinishStrategyRemote()
{
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN("Strategy (Remote): Ending");

  DetachSelectButton();

  if (GetStatus(MODULE_MOTOR_ACT))
  {
    TerminateMotors();
  }

  SystemDisable();

  LedBlinkDoubleShort(BINARY_CODE_LED_RED);
  DEBUG_PRINTLN("Strategy (Remote): Finished");
}

// Read RF signal and move motors accordingly
unsigned long lastProcessCommand = 0;
void ProcessIncomingCommands()
{
  if (millis() - lastProcessCommand > REMOTE_PROCESS_DT)
  {
    lastProcessCommand = millis();
    float throttle = getChannelFloat(REMOTE_CHANNEL_THROTTLE);
    float steer = getChannelFloat(REMOTE_CHANNEL_STEER);
    float enable = getChannelFloat(REMOTE_CHANNEL_ENABLE);

    // move motors
    MotorMove(steer, throttle, enable);
  }
}

// Select button function
void SelectFunctionRemote()
{
  if (millis() - lastMillisSelect > BTN_DEBOUNCE_TIME)
  {
    lastMillisSelect = millis();

    if (!GetStatus(MODULE_MOTORS))
    {
      SystemEnable(MODULE_MOTORS);

      LedBlinkDoubleShort(BINARY_CODE_LED_GRN);
    }
    else
    {
      SystemDisable(MODULE_MOTORS);

      LedBlink(BINARY_CODE_LED_RED, LED_BLINK_LONG, 0);
    }
  }
}