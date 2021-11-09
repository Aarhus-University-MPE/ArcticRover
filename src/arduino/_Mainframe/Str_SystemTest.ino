/*  Strategy - System Test mode

    Runs system test...
*/

// Start sequence of strategy
void StartStrategySystemTest() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN("Strategy (System Test): Starting");
  StrategyStartLed(MODE_SYSTEMTEST);

  SystemDisable();

  AttachSelectButton();

  DEBUG_PRINTLN("Strategy (System Test): Initialized");
  DEBUG_PRINTLINE();
  LedBlinkDoubleShort(BINARY_CODE_LED_GRN);
}

// Main sequence of strategy
bool runTest = false;
void RunStrategySystemTest() {
  // Blink light
  StrategyRunLed(MODE_SYSTEMTEST);
  
  if(runTest){
    DEBUG_PRINTLN("Running Full System Test");

    DEBUG_PRINTLINE();

    SystemTest();

    DEBUG_PRINTLN("Disabeling all systems");
    DEBUG_PRINTLINE();

    unsigned long testResults = ToLong(SystemStatus);
    
    SystemDisable();
    AttachSelectButton();
    
    DEBUG_PRINTLINE();
    DEBUG_PRINTLN("System Test Complete");
    DEBUG_PRINT("  Results: ");
    DEBUG_PRINTLN(String(testResults));
    DEBUG_PRINTLINE();
    runTest = false;
  }
  
}

// End sequence of strategy
void FinishStrategySystemTest() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN("Strategy (System Test): Ending");
  
  DetachSelectButton();
  SystemDisable();

  DEBUG_PRINTLN("Strategy (System Test): Finished");
}

// Selecet button function
void SelectFunctionSystemTest(){
  if (millis() - lastMillisSelect > BTN_DEBOUNCE_TIME)
  {
    lastMillisSelect = millis();
    runTest = true;
    DetachSelectButton();
  }
}

// Enable all systems
void SystemTestEnable(){
  
}

// Run full system check
void SystemTest(){
  // SetStatus(MODULE_PWR_MOTOR, digitalRead(PO_POWER_MOTOR_ON)); // Bi-stable relay, so not possible to measure
  DEBUG_PRINTLN("Running test (1/4) - Power Systems");
  SystemTestModule(MODULE_PWR_12V, false);
  SystemTestModule(MODULE_PWR_5V, false);
  SystemTestModule(MODULE_PWR_MOTOR, false);
  SystemTestModule(MODULE_PWR, false);
  DEBUG_PRINTLN("Test (1/4) - Power Systems (Complete)");
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN("Running test (2/4) - Communication");
  SystemTestModule(MODULE_RF, false);
  SystemTestModule(MODULE_IRIDIUM, false);
  SystemTestModule(MODULE_BACKUPCPU, false);
  DEBUG_PRINTLN("Test (2/4) - Communication (Complete)");
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN("Running test (3/4) - Subsystems");
  SystemTestModule(MODULE_ACCEL, false);
  SystemTestModule(MODULE_SD, false);
  SystemTestModule(MODULE_BLACKBOX, false);
  DEBUG_PRINTLN("Test (3/4) - Subsystems (Complete)");
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN("Running test (4/4) - Motors");
  SystemTestModule(MODULE_MOTORS, false);
  DEBUG_PRINTLN("Test (4/4) - Motors (Complete)");
  DEBUG_PRINTLINE();
  
  SetStatus(MODULE_ESTOP,     EmergencyStopStatus());
  SetStatus(MODULE_RESERVED,  true);
}



bool SystemTestModule(byte module, bool disableAfterTest){
  SystemEnable(module);
  bool status = false;

  if(GetStatus(module)){
    switch (module)
    {
    case MODULE_PWR:
      status = BatteryStatus();
      break;
    case MODULE_PWR_12V:
      status = digitalRead(PO_POWER_12V);
      break;
    case MODULE_PWR_5V:
      status = digitalRead(PO_POWER_5V);
      break;
    case MODULE_PWR_MOTOR:
      status = MotorStatus();
      break;
    case MODULE_GNSS:
      status = GnssTest(true);
      break;
    case MODULE_RF:
      status = SBusTest();
      break;
    case MODULE_IRIDIUM:
      status = IridiumTest();
      break;
    case MODULE_DBGCOMM:
      status = DebugCommStatus();
      break;
    case MODULE_BACKUPCPU:
      status = HeartBeatStatus();
      break;
    case MODULE_ACCEL:
      status = AccelTest(true);
      break;
    case MODULE_SD:
      status = SDReaderStatus();
      break;
    case MODULE_BLACKBOX:
      status = BlackBoxStatus();
      break;
    case MODULE_MOTORS:
      DEBUG_PRINTLN("Motor Test 1 - Linear Ramp");
      MotorTest1();
      DEBUG_PRINTLINE();
      DEBUG_PRINTLN("Motor Test 2 - Steering");
      MotorTest2();
      status = digitalRead(PO_MOTOR_EN_LEFT)  && digitalRead(PO_MOTOR_EN_RIGHT);
      break;
    default:
      break;
    }
  }
  

  SetStatus(module, status);

  if(disableAfterTest){
    SystemDisable(module);
  } 

  return status;
}