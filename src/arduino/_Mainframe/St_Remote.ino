/*  Strategy - Remote control
    
    Allows manual control of motors
*/

// Start sequence of strategy
void StartStrategyRemote() {
  DEBUG_PRINT("Strategy: Remote Control Starting...");
  
  LedBlink(BINARY_CODE_LED_RED, LED_BLINK_LONG, 0);
  
  // Enable motor control
  attachInterrupt(PI_BUTTON_SELECT,SelectButtonRemoteInterruptHandler, FALLING);
  digitalWrite(PO_POWER_5V,    HIGH);
  digitalWrite(PO_POWER_RF,    HIGH);

  LedBlinkDoubleShort(BINARY_CODE_LED_YEL, BINARY_CODE_LED_GRN);
  DEBUG_PRINTLN("motor remote control ENABLED... Enable/Disable motors with Select Button.");

}

// Main sequence of strategy
void RunStrategyRemote(){

  // Read RF signal
  if(isSystemStable)  ProcessIncomingCommands();

  isSystemStable = HeartbeatRemote();  
}

// End sequence of strategy
void FinishStrategyRemote() {
  DEBUG_PRINT("Strategy: Remote Control ending... ");
  
  detachInterrupt(PI_BUTTON_SELECT);

  if (isMotorEnabled) {
    TerminateMotors(true);
    digitalWrite(PO_POWER_MOTOR, LOW);
  }

  digitalWrite(PO_POWER_5V,    LOW);
  digitalWrite(PO_POWER_RF,    LOW);

  LedBlinkDoubleShort(BINARY_CODE_LED_RED);
  DEBUG_PRINTLN("Systems powered down... remote control DISABLED.");
}

// Enable/Disable motor power
void SelectButtonRemoteInterruptHandler(){
  if (millis() - lastMillisSelect > BUTTON_TIMEOUT_DURATION) {
    lastMillisSelect=millis();

    if (!isMotorEnabled){
      digitalWrite(PO_POWER_MOTOR, HIGH);
      InitializeMotors(true);

      LedBlinkDoubleShort(BINARY_CODE_LED_GRN);
      isMotorEnabled = true;
    }
    else {
      digitalWrite(PO_POWER_MOTOR, LOW);
      TerminateMotors(true);

      LedBlink(BINARY_CODE_LED_RED, LED_BLINK_LONG, 0);
      isMotorEnabled = false;
    }
  }
}

// Read RF signal and move motors accordingly
void ProcessIncomingCommands(){
  
  sbus.process();       // Read RF data stream

  float steer = getChannelFloat(1);
  float throttle = getChannelFloat(0);

  // move motors
  MotorMove(steer, throttle);
}

// Runs system checks
bool HeartbeatRemote(){
  DEBUG_PRINT("Remote heartbeat starting...");

  bool systemStable = true;       // Required
  bool subSystemStable = true;    // Optional
  // Check Battery
  // Check Sensors
  // Check Motors

  if(systemStable && subSystemStable) LedBlinkSingleShort(BINARY_CODE_LED_GRN);
  else if(systemStable) LedBlinkSingleShort(BINARY_CODE_LED_YEL);
  else LedBlinkSingleShort(BINARY_CODE_LED_RED);

  DEBUG_PRINTLN("System stable.");
  return systemStable;
}