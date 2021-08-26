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

    DEBUG_PRINTLN("  Enabling all systems");
    DEBUG_PRINTLINE();

    SystemEnable();
    DEBUG_PRINTLINE();
    
    DEBUG_PRINTLN("Testing subsystems");
    DEBUG_PRINTLINE();

    GetStatus(true);

    DEBUG_PRINTLINE();
    DEBUG_PRINTLN("Disabeling all systems");
    DEBUG_PRINTLINE();
    
    SystemDisable();
    AttachSelectButton();
    
    DEBUG_PRINTLINE();
    DEBUG_PRINTLN("System Test Complete");
    DEBUG_PRINT("  Results: ");
    DEBUG_PRINTLN(String(ToByte(SystemStatus)));
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

// Run full system check
void GetStatus(bool printRes){
  // SetStatus(MODULE_PWR_MOTOR, digitalRead(PO_POWER_MOTOR_ON)); // Bi-stable relay, so not possible to measure
  SetStatus(MODULE_PWR_12V,   digitalRead(PO_POWER_12V));
  SetStatus(MODULE_PWR_5V,    digitalRead(PO_POWER_5V));
  SetStatus(MODULE_RF,       (digitalRead(PO_POWER_RF)      &&  digitalRead(PO_POWER_5V)   && SBusStatus()));
  SetStatus(MODULE_IRIDIUM,   IridiumTest(printRes));
  SetStatus(MODULE_PWR,       BatteryStatus());
  SetStatus(MODULE_MOTOR,     MotorStatus());
  SetStatus(MODULE_MOTOR_EN,  MotorState());
  SetStatus(MODULE_GNSS,      GnssTest(printRes));
  SetStatus(MODULE_SD,        SDReaderStatus());
  SetStatus(MODULE_ACCEL,     AccelTest(printRes));
  SetStatus(MODULE_DBGCOMM,   DebugCommStatus());
  SetStatus(MODULE_BACKUPCPU, HeartBeatStatus());
  SetStatus(MODULE_ESTOP,     emergencyStop);
  SetStatus(MODULE_BLACKBOX,  BlackBoxStatus());
  SetStatus(MODULE_RESERVED,  true);
}