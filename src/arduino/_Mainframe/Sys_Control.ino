/*
  GeoRover System Control

  Mads Rosenhøj Jeppesen
  Aarhus University
  2021
*/

bool SystemEnable(int module) {
  if (GetStatus(module)) return true;
  DEBUG_PRINT(F("SYS Enable: "));
  DEBUG_PRINT(ModuleToString(module));

  bool status = true;

  switch (module) {
    case MODULE_PWR:
      status = BatteryStatus();
      break;
    case MODULE_PWR_5V:
      digitalWrite(PO_POWER_5V, HIGH);
      break;
    case MODULE_PWR_12V:
      digitalWrite(PO_POWER_12V, HIGH);
      break;
    case MODULE_PWR_24V:
      digitalWrite(PO_POWER_24V, HIGH);
      break;
    case MODULE_PWR_MOTOR:
      digitalWrite(PO_POWER_MOTOR, HIGH);
      break;
    case MODULE_MOTORS:
      status = InitializeMotors();
      break;
    case MODULE_MOTOR_L:
      status = MotorStatusLeft();
      break;
    case MODULE_MOTOR_R:
      status = MotorStatusRight();
      break;
    case MODULE_MOTOR_ACT:
      status = MotorState();
      break;
    case MODULE_CANBUS:
      status = InitializeCanBus();
      break;
    case MODULE_RF:
      status = InitializeSBUS();
      break;
    case MODULE_IRIDIUM:
      digitalWrite(PO_POWER_IRIDIUM, HIGH);
      status = InitializeIridium();
      break;
    case MODULE_ACCEL:
      status = InitializeAccel();
      break;
    case MODULE_GNSS:
      status = InitializeGnss();
      break;
    case MODULE_SD:
      status = InitializeSDReader();
      break;
    case MODULE_BLACKBOX:
      status = InitBlackBox();
      break;
    case MODULE_ROUTE:
      status = RouteCheck();
      break;
    case MODULE_HEATING:
      status = HeatingStart();
      break;
    case MODULE_TEMP:
      status = TemperatureStatus();
      break;
    case MODULE_BACKUPCPU:
      status = GetStatus(MODULE_BACKUPCPU);
      break;
    case MODULE_LED:
      status = true;
      break;
    case MODULE_DEBUG:
      status = DebugInitialize();
      break;
    default:
      DEBUG_PRINT(F("- UNKNOWN Case"));
      break;
  }

  if (status) {
    DEBUG_PRINTLN(F(": Enabled"));
  } else {
    DEBUG_PRINTLN(F(": ERROR"));
  }
  SetStatus(module, status);
  delay(5);
  return status;
}

// Enables Primary Systems
void SystemEnablePrimary() {
  SystemEnable(MODULE_PWR);
  SystemEnable(MODULE_SD);
  SystemEnable(MODULE_BLACKBOX);
  SystemEnable(MODULE_GNSS);
  SystemEnable(MODULE_IRIDIUM);
  SystemEnable(MODULE_TEMP);
}

// Enables module then flash LED indicating success or error
bool SystemSignalEnable(byte module) {
  bool status = SystemEnable(module);
  if (status) {
    StatusHaltLed(LED_SIGNAL_OK_SHORT_HALT);
  } else {
    StatusHaltLed(LED_SIGNAL_ERROR_SHORT_HALT);
  }
  return status;
}

// Enable systems for current mode
bool SystemEnableMode() {
  int modules = 0;
  bool status = true;
  bool enableStatus[10];
  switch (mode) {
    case MODE_REMOTECONTROL:
      modules = 4;

      enableStatus[0] = SystemEnable(MODULE_PWR);
      SystemEnable(MODULE_PWR_MOTOR);
      enableStatus[1] = SystemEnable(MODULE_MOTORS);
      enableStatus[2] = SystemEnable(MODULE_CANBUS);
      enableStatus[3] = SystemEnable(MODULE_RF);

      break;

    case MODE_AUTONOMOUS:
      modules = 5;

      enableStatus[0] = SystemEnable(MODULE_PWR);
      SystemEnable(MODULE_PWR_MOTOR);
      enableStatus[1] = SystemEnable(MODULE_MOTORS);
      enableStatus[2] = SystemEnable(MODULE_CANBUS);
      enableStatus[3] = SystemEnable(MODULE_ACCEL);
      enableStatus[4] = SystemEnable(MODULE_RF);  // TODO: Temporary Remote Control Override for testing safety

      break;

    default:
      status = false;
      break;
  }

  for (int i = 0; i < modules; i++) {
    // if (!SystemEnableStatus(enableStatus[i])) {
    if (!enableStatus[i]) {
      status = false;
    }
  }
  return status;
}

// Sets navigationPreCheck flag to false and signals
bool SystemEnableStatus(bool status) {
  if (status) {
    StatusHaltLed(LED_SIGNAL_OK_SHORT_HALT);
  } else {
    StatusHaltLed(LED_SIGNAL_ERROR_SHORT_HALT);
  }
  return status;
}

// Disables specified module
void SystemDisable(int module) {
  if (!GetStatus(module)) return;
  bool status = false;
  DEBUG_PRINT(F("SYS Disable: "));
  DEBUG_PRINT(ModuleToString(module));

  switch (module) {
    case MODULE_PWR:
      status = BatteryStatus();
      break;
    case MODULE_PWR_5V:
      digitalWrite(PO_POWER_5V, LOW);
      break;
    case MODULE_PWR_12V:
      digitalWrite(PO_POWER_12V, LOW);
      break;
    case MODULE_PWR_24V:
      digitalWrite(PO_POWER_24V, LOW);
      break;
    case MODULE_PWR_MOTOR:
      digitalWrite(PO_POWER_MOTOR, LOW);
      break;
    case MODULE_MOTORS:
      TerminateMotors();
      break;
    case MODULE_MOTOR_L:
      status = MotorStatusLeft();
      break;
    case MODULE_MOTOR_R:
      status = MotorStatusRight();
      break;
    case MODULE_MOTOR_ACT:
      status = MotorState();
      break;
    case MODULE_CANBUS:
      TerminateCanBus();
      break;
    case MODULE_RF:
      TerminateSBUS();
      digitalWrite(PO_POWER_RF, LOW);
      break;
    case MODULE_IRIDIUM:
      TerminateIridium();
      digitalWrite(PO_POWER_IRIDIUM, LOW);
      break;
    case MODULE_GNSS:
      TerminateGnss();
      break;
    case MODULE_ACCEL:
      TerminateAccel();
      break;
    case MODULE_SD:
      status = SDReaderStatus();
      break;
    case MODULE_BLACKBOX:
      status = BlackBoxStatus();
      break;
    case MODULE_DBGCOMM:
      status = DebugCommStatus();
      break;
    case MODULE_LED:
      status = LedStatus();
      break;
    case MODULE_HEATING:
      HeatingStop();
      break;
    case MODULE_TEMP:
      status = TemperatureStatus();
      break;
    case MODULE_BACKUPCPU:
      status = HeartBeatStatus();
      break;
    case MODULE_ESTOP:
      status = EmergencyStopStatus();
      break;
    case MODULE_DEBUG:
      DebugTerminate();
      break;
    case MODULE_RESERVED:
      status = true;
      break;
    default:
      DEBUG_PRINT(F(" - Unknown Case"));
      DEBUG_PRINTLN(ModuleToString(module));
      break;
  }

  if (status) {
    DEBUG_PRINTLN(F("Unable to Disable"));
  } else {
    DEBUG_PRINTLN(F("- Disabled"));
  }

  SetStatus(module, status);
}

// Disable all secondary systems
void SystemDisable() {
  SystemDisable(MODULE_RF);
  // SystemDisable(MODULE_IRIDIUM);
  // SystemDisable(MODULE_GNSS);
  SystemDisable(MODULE_ACCEL);
  SystemDisable(MODULE_PWR_MOTOR);
  SystemDisable(MODULE_MOTORS);
  SystemDisable(MODULE_CANBUS);
  SystemDisable(MODULE_PWR_MOTOR);
  SystemDisable(MODULE_PWR_5V);
  SystemDisable(MODULE_PWR_12V);
  SystemDisable(MODULE_PWR_24V);
}

// Runs system check if time since last check > timeout, returns comparison between required modules to current
bool SystemCheckMode(int mode) {
  static bool status = true;

  if (millis() - lastSystemCheck < SYSTEM_CHECK_DT) {
    return status;
  }

  lastSystemCheck = millis();
  SystemCheck();

  switch (mode) {
    case MODE_REMOTECONTROL:
      status = ((((ToLong(SystemStatus) ^ SYSREQ_REMOTE_CONTROL) &
                  SYSREQ_REMOTE_CONTROL) |
                 (1L << MODULE_RESERVED)) == (1L << MODULE_RESERVED));
      if (!status) {
        DEBUG_PRINT(F("ERROR Code: "));
        DEBUG_PRINTLN(String(((ToLong(SystemStatus) ^ SYSREQ_REMOTE_CONTROL) &
                              SYSREQ_REMOTE_CONTROL) |
                             (1L << MODULE_RESERVED)));
      }
      break;
    case MODE_AUTONOMOUS:
      status =
          ((((ToLong(SystemStatus) ^ SYSREQ_AUTONOMOUS) & SYSREQ_AUTONOMOUS) |
            (1L << MODULE_RESERVED)) == (1L << MODULE_RESERVED));
      if (!status) {
        DEBUG_PRINT(F("ERROR Code: "));
        DEBUG_PRINTLN(((ToLong(SystemStatus) ^ SYSREQ_REMOTE_CONTROL) &
                       SYSREQ_REMOTE_CONTROL) |
                      (1L << MODULE_RESERVED));
      }
      break;
    default:
      break;
  }

  return status;
}

// Run full system test
bool SystemTest() {
  unsigned long testResults;
  bool testDone = false;
  switch (systemTestState) {
    case 0:
      DEBUG_PRINTLN(F("Running Full System Test"));
      DEBUG_PRINTLINE();
      systemTestState++;
      break;
    case 1:
      DEBUG_PRINTLN(F("Running test (1/4) - Power Systems"));
      SystemTestModule(MODULE_PWR);
      SystemTestModule(MODULE_PWR_5V);
      SystemTestModule(MODULE_PWR_12V);
      SystemTestModule(MODULE_PWR_24V);
      SystemTestModule(MODULE_PWR_MOTOR);
      DEBUG_PRINTLN(F("Test (1/4) - Power Systems (Complete)"));
      DEBUG_PRINTLINE();
      systemTestState++;
      break;
    case 2:
      DEBUG_PRINTLN(F("Running test (2/4) - Communication"));
      systemTestState++;
      break;
    case 3:
      if (SystemTestModule(MODULE_RF)) systemTestState++;
      break;
    case 4:
      if (SystemTestModule(MODULE_IRIDIUM)) systemTestState++;
      break;
    case 5:
      if (SystemTestModule(MODULE_GNSS)) systemTestState++;
      break;
    case 6:
      if (SystemTestModule(MODULE_BACKUPCPU)) systemTestState++;
      break;
    case 7:
      DEBUG_PRINTLN(F("Test (2/4) - Communication (Complete)"));
      DEBUG_PRINTLINE();
      systemTestState++;
      break;
    case 8:
      DEBUG_PRINTLN(F("Running test (3/4) - Subsystems"));
      systemTestState++;
      break;
    case 9:
      if (SystemTestModule(MODULE_ACCEL)) systemTestState++;
      break;
    case 10:
      if (SystemTestModule(MODULE_SD)) systemTestState++;
      break;
    case 11:
      if (SystemTestModule(MODULE_BLACKBOX)) systemTestState++;
      break;
    case 12:
      if (SystemTestModule(MODULE_TEMP)) systemTestState++;
      break;
    case 13:
      if (SystemTestModule(MODULE_LED)) systemTestState++;
      break;
    case 14:
      if (SystemTestModule(MODULE_ROUTE)) systemTestState++;
      break;
    case 15:
      DEBUG_PRINTLN(F("Test (3/4) - Subsystems (Complete)"));
      DEBUG_PRINTLINE();
      systemTestState++;
      break;
    case 16:
      DEBUG_PRINTLN(F("Running test (4/4) - Motors"));
      systemTestState++;
      break;
    case 17:
      if (SystemTestModule(MODULE_MOTORS)) systemTestState++;
      break;
    case 18:
      DEBUG_PRINTLN(F("Test (4/4) - Motors (Complete)"));
      DEBUG_PRINTLINE();
      systemTestState++;
      break;
    case 19:
      DEBUG_PRINTLN(F("Disabeling all systems"));
      DEBUG_PRINTLINE();
      testResults = ToLong(SystemStatus);
      SystemDisable();
      AttachSelectButton();
      DEBUG_PRINTLINE();
      DEBUG_PRINTLN(F("System Test Complete"));
      DEBUG_PRINT(F("  Results: "));
      DEBUG_PRINTLN(String(testResults));
      DEBUG_PRINTLINE();
      testDone        = true;
      systemTestState = 0;
      break;
    default:
      DEBUG_PRINTLN(F("System Test Error: Stopping"));
      systemTestState = 0;
      testDone        = true;
      break;
  }

  return testDone;
}

// Runs module test
bool SystemTestModule(byte module) {
  SystemEnable(module);
  bool status   = false;
  bool testDone = true;

  if (GetStatus(module)) {
    switch (module) {
      case MODULE_PWR:
        status = BatteryStatus(false);
        break;
      case MODULE_PWR_5V:
        status = digitalRead(PO_POWER_5V);
        break;
      case MODULE_PWR_12V:
        status = digitalRead(PO_POWER_12V);
        break;
      case MODULE_PWR_24V:
        status = digitalRead(PO_POWER_24V);
        break;
      case MODULE_PWR_MOTOR:
        status = digitalRead(PO_POWER_MOTOR);
        break;
      case MODULE_MOTORS:
        testDone = MotorTest();
        status   = MotorStatus();
        break;
      case MODULE_MOTOR_L:
        status = MotorStatusLeft();
        break;
      case MODULE_MOTOR_R:
        status = MotorStatusRight();
        break;
      case MODULE_MOTOR_ACT:
        status = MotorState();
        break;
      case MODULE_CANBUS:
        testDone = CanBusTest();
        status   = CanBusStatus();
        break;
      case MODULE_RF:
        testDone = SBusTest();
        status   = SBusStatus();
        break;
      case MODULE_IRIDIUM:
        status = IridiumTest();
        break;
      case MODULE_GNSS:
        testDone = GnssTest();
        status   = GnssStatus();
        break;
      case MODULE_ACCEL:
        testDone = AccelTest();
        status   = AccelStatus();
        break;
      case MODULE_SD:
        status = SDReaderStatus();
        break;
      case MODULE_BLACKBOX:
        status = BlackBoxStatus();
        break;
      case MODULE_ROUTE:
        status = RouteTest();
        break;
      case MODULE_BACKUPCPU:
        status = HeartBeatStatus();
        break;
      case MODULE_DBGCOMM:
        status = DebugCommStatus();
        break;
      case MODULE_LED:
        LedTest();
        status = true;
        break;
      case MODULE_HEATING:
        HeatingStart();
        delay(2000);
        HeatingStop();
        status = true;
        break;
      case MODULE_TEMP:
        status   = TemperatureStatus();
        testDone = TemperatureTest();
        break;
      case MODULE_ESTOP:
        status = EmergencyStopStatus();
        break;
      case MODULE_RESERVED:
        status = true;
        break;
      default:
        DEBUG_PRINT(F("MODULE TEST: Unknown System Module: "));
        DEBUG_PRINTLN(ModuleToString(module));
        break;
    }
  }

  // SetStatus(module, status);

  return testDone;
}

// Checks all Systems
void SystemCheck() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Running full system check"));
  DEBUG_PRINTLINE();
  BatteryStatus(true);
  PrintTemperature();
  for (int i = 0; i < MODULE_COUNT - 3; i++) {
    SystemCheckModule(i);
  }

  SetStatus(MODULE_ESTOP, EmergencyStopStatus());
  SetStatus(MODULE_RESERVED, true);

  DEBUG_PRINT(F("System Status: "));
  DEBUG_PRINTLN(String(ToLong(SystemStatus)));
  DEBUG_PRINTLINE();
}

// Runs module check
bool SystemCheckModule(byte module) {
  bool status = false;

  switch (module) {
    case MODULE_PWR:
      status = BatteryStatus(false);
      break;
    case MODULE_PWR_5V:
      status = digitalRead(PO_POWER_5V);
      break;
    case MODULE_PWR_12V:
      status = digitalRead(PO_POWER_12V);
      break;
    case MODULE_PWR_24V:
      status = digitalRead(PO_POWER_24V);
      break;
    case MODULE_PWR_MOTOR:
      status = digitalRead(PO_POWER_MOTOR);
      break;
    case MODULE_MOTORS:
      status = MotorStatus();  // Left && Right motor
      break;
    case MODULE_MOTOR_L:
      status = MotorStatusLeft();
      break;
    case MODULE_MOTOR_R:
      status = MotorStatusRight();
      break;
    case MODULE_MOTOR_ACT:
      status = MotorState();  // <-- Active or idle?
      break;
    case MODULE_CANBUS:
      status = CanBusStatus();
      break;
    case MODULE_RF:
      status = SBusStatus();
      break;
    case MODULE_IRIDIUM:
      status = IridiumStatus();
      break;
    case MODULE_GNSS:
      status = GnssStatus();
      break;
    case MODULE_ACCEL:
      status = AccelStatus();
      break;
    case MODULE_SD:
      status = SDReaderStatus();
      break;
    case MODULE_BLACKBOX:
      status = BlackBoxStatus();
      break;
    case MODULE_ROUTE:
      status = RouteCheck();
      break;
    case MODULE_DBGCOMM:
      status = DebugCommStatus();
      break;
    case MODULE_LED:
      status = LedStatus();
      break;
    case MODULE_HEATING:
      status = HeatingStatus();
      break;
    case MODULE_TEMP:
      status = TemperatureStatus();
      break;
    case MODULE_BACKUPCPU:
      status = HeartBeatStatus();
      break;
    default:
      DEBUG_PRINT(F("MODULE CHECK: Unknown System Module: "));
      DEBUG_PRINTLN(module);
      break;
  }

  SetStatus(module, status);
  return status;
}
