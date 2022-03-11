/*
  GeoRover System Control

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021
*/

bool SystemStatus[MODULE_COUNT];

bool GetStatus(int module) {
  return SystemStatus[module];
}
void SetStatus(int module, bool status) {
  SystemStatus[module] = status;
}
void SetStatus(bool status) {
  for (int i = 0; i < MODULE_COUNT; i++) {
    SystemStatus[i] = status;
  }
  SystemStatus[MODULE_ESTOP] = true;
  SystemStatus[MODULE_RESERVED] = true;
}

void SystemEnable(int module) {
  if (GetStatus(module)) return;
  DEBUG_PRINT("SYS Enable: ");
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
      digitalWrite(PO_POWER_RF, HIGH);
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
    default:
      DEBUG_PRINT("- UNKNOWN Case");
      break;
  }

  if (status) {
    DEBUG_PRINTLN(": Enabled");
  } else {
    DEBUG_PRINTLN(": ERROR");
  }
  SetStatus(module, status);
  delay(10);
}

// Enables Primary Systems
void SystemEnablePrimary() {
  SystemEnable(MODULE_PWR);
  SystemEnable(MODULE_SD);
  SystemEnable(MODULE_BLACKBOX);
  SystemEnable(MODULE_GNSS);
  SystemEnable(MODULE_IRIDIUM);
}

void SystemEnableMode(int mode) {
  switch (mode) {
    case MODE_REMOTECONTROL:
      SystemEnable(MODULE_PWR);
      // SystemEnable(MODULE_PWR_MOTOR); // <-- Enabled by user input
      // (along with motor initialization) SystemEnable(MODULE_MOTORS); //
      // <-- Enabled by user input
      SystemEnable(MODULE_RF);
      SystemEnable(MODULE_CANBUS);
      break;
    case MODE_AUTONOMOUS:
      // SystemEnable();
      break;
    case MODE_SYSTEMTEST:
      //
      break;
    default:
      break;
  }
}

void SystemDisable(int module) {
  if (!GetStatus(module)) return;
  bool status = false;
  DEBUG_PRINT("SYS Disable: ");
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
      SystemDisable(MODULE_PWR_MOTOR);
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
    case MODULE_RESERVED:
      status = true;
      break;
    default:
      DEBUG_PRINT(" - Unknown Case");
      DEBUG_PRINTLN(ModuleToString(module));
      break;
  }

  if (status) {
    DEBUG_PRINTLN("Unable to Disable");
  } else {
    DEBUG_PRINTLN("- Disabled");
  }

  SetStatus(module, status);
}

// Disable all secondary systems
void SystemDisable() {
  SystemDisable(MODULE_RF);
  // SystemDisable(MODULE_IRIDIUM);
  // SystemDisable(MODULE_GNSS);
  SystemDisable(MODULE_ACCEL);
  SystemDisable(MODULE_MOTORS);
  SystemDisable(MODULE_CANBUS);
  SystemDisable(MODULE_PWR_MOTOR);
  SystemDisable(MODULE_PWR_5V);
  SystemDisable(MODULE_PWR_12V);
  SystemDisable(MODULE_PWR_24V);
}

bool SystemCheck(int mode) {
  static bool status = false;
  SystemCheck();

  switch (mode) {
    case MODE_REMOTECONTROL:
      status = ((((ToLong(SystemStatus) ^ SYSREQ_REMOTE_CONTROL) &
                 SYSREQ_REMOTE_CONTROL) |
                (1L << MODULE_RESERVED)) == (1L << MODULE_RESERVED));
      if (!status) {
        DEBUG_PRINT("ERROR Code: ");
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
        DEBUG_PRINT("ERROR Code: ");
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
      DEBUG_PRINTLN("Running Full System Test");
      DEBUG_PRINTLINE();
      systemTestState++;
      break;
    case 1:
      DEBUG_PRINTLN("Running test (1/4) - Power Systems");
      SystemTestModule(MODULE_PWR);
      SystemTestModule(MODULE_PWR_5V);
      SystemTestModule(MODULE_PWR_12V);
      SystemTestModule(MODULE_PWR_24V);
      SystemTestModule(MODULE_PWR_MOTOR);
      DEBUG_PRINTLN("Test (1/4) - Power Systems (Complete)");
      DEBUG_PRINTLINE();
      systemTestState++;
      break;
    case 2:
      DEBUG_PRINTLN("Running test (2/4) - Communication");
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
      DEBUG_PRINTLN("Test (2/4) - Communication (Complete)");
      DEBUG_PRINTLINE();
      systemTestState++;
      break;
    case 8:
      DEBUG_PRINTLN("Running test (3/4) - Subsystems");
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
      DEBUG_PRINTLN("Test (3/4) - Subsystems (Complete)");
      DEBUG_PRINTLINE();
      systemTestState++;
      break;
    case 15:
      DEBUG_PRINTLN("Running test (4/4) - Motors");
      systemTestState++;
      break;
    case 16:
      if (SystemTestModule(MODULE_MOTORS)) systemTestState++;
      break;
    case 17:
      DEBUG_PRINTLN("Test (4/4) - Motors (Complete)");
      DEBUG_PRINTLINE();
      systemTestState++;
      break;
    case 18:
      DEBUG_PRINTLN("Disabeling all systems");
      DEBUG_PRINTLINE();
      testResults = ToLong(SystemStatus);
      SystemDisable();
      AttachSelectButton();
      DEBUG_PRINTLINE();
      DEBUG_PRINTLN("System Test Complete");
      DEBUG_PRINT("  Results: ");
      DEBUG_PRINTLN(String(testResults));
      DEBUG_PRINTLINE();
      testDone = true;
      systemTestState = 0;
      break;
    default:
      DEBUG_PRINTLN("System Test Error: Stopping");
      systemTestState = 0;
      testDone = true;
      break;
  }

  return testDone;
}

bool SystemTestModule(byte module) {
  SystemEnable(module);
  bool status = false;
  bool testDone = true;

  if (GetStatus(module)) {
    switch (module) {
      case MODULE_PWR:
        status = BatteryStatus(true);
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
        status = MotorPowerStatus();
        break;
      case MODULE_MOTORS:
        testDone = MotorTest();
        status = MotorStatus();
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
        status = CanBusStatus();
        break;
      case MODULE_RF:
        testDone = SBusTest();
        status = SBusStatus();
        break;
      case MODULE_IRIDIUM:
        status = IridiumTest();
        break;
      case MODULE_GNSS:
        testDone = GnssTest();
        status = GnssStatus();
        break;
      case MODULE_ACCEL:
        testDone = AccelTest();
        status = AccelStatus();
        break;
      case MODULE_SD:
        status = SDReaderStatus();
        break;
      case MODULE_BLACKBOX:
        status = BlackBoxStatus();
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
        status = TemperatureStatus();
        testDone = TemperatureTest();
        break;
      case MODULE_ESTOP:
        status = EmergencyStopStatus();
        break;
      case MODULE_RESERVED:
        status = true;
        break;
      default:
        DEBUG_PRINT("MODULE TEST: Unknown System Module: ");
        DEBUG_PRINTLN(ModuleToString(module));
        break;
    }
  }

  SetStatus(module, status);

  return testDone;
}

// System check
void SystemCheck() {
  for (int i = 0; i < MODULE_COUNT - 2; i++) {
    SystemCheckModule(i);
  }
  
  SetStatus(MODULE_ESTOP, EmergencyStopStatus());
  SetStatus(MODULE_RESERVED, true);
}

bool SystemCheckModule(byte module) {
  bool status = false;

  if (GetStatus(module)) {
    switch (module) {
      case MODULE_PWR:
        status = BatteryStatus(true);
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
        status = MotorStatus();
        break;
      case MODULE_MOTORS:
        status = MotorStatus();  // <-- CAN ERROR MSG
        break;
      case MODULE_MOTOR_L:
        status = MotorStatusLeft();  // <-- CAN ERROR MSG
        break;
      case MODULE_MOTOR_R:
        status = MotorStatusRight();  // <-- CAN ERROR MSG
        break;
      case MODULE_MOTOR_ACT:
        status = MotorState();  // <-- CAN ERROR MSG
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
      case MODULE_DBGCOMM:
        status = DebugCommStatus();
        break;
      case MODULE_LED:
        status = LedStatus();
        break;
      case MODULE_HEATING:
        status = GetStatus(MODULE_HEATING);
        break;
      case MODULE_TEMP:
        status = TemperatureStatus();
        break;
      case MODULE_BACKUPCPU:
        status = HeartBeatStatus();
        break;
      default:
        DEBUG_PRINT("MODULE CHECK: Unknown System Module: ");
        DEBUG_PRINTLN(ModuleToString(module));
        break;
    }
  }

  SetStatus(module, status);
  return status;
}
