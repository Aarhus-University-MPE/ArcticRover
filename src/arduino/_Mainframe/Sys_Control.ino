/*
  GeoRover System Control

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021
*/

void SystemEnable(int module) {
  if (GetStatus(module)) return;
  DEBUG_PRINT("SYS Enable: ")
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
      InitBlackBox();
      status = BlackBoxStatus();
      break;
    case MODULE_HEATING:
      status = HeatingStart();
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
  SystemEnable(MODULE_SD);
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
  DEBUG_PRINT("SYS Disable: ")
  DEBUG_PRINTLN(ModuleToString(module));

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
  SystemTest();

  switch (mode) {
    case MODE_REMOTECONTROL:
      status = (((ToLong(SystemStatus) ^ SYSREQ_REMOTE_CONTROL) &
                 SYSREQ_REMOTE_CONTROL) |
                (1L << MODULE_RESERVED)) == (1L << MODULE_RESERVED);
      if (!status) {
        DEBUG_PRINT("ERROR Code: ")
        DEBUG_PRINTLN(String(((ToLong(SystemStatus) ^ SYSREQ_REMOTE_CONTROL) &
                              SYSREQ_REMOTE_CONTROL) |
                             (1L << MODULE_RESERVED)));
      }
      break;
    case MODE_AUTONOMOUS:
      status =
          (((ToLong(SystemStatus) ^ SYSREQ_AUTONOMOUS) & SYSREQ_AUTONOMOUS) |
           (1L << MODULE_RESERVED)) == (1L << MODULE_RESERVED);
      if (!status) {
        DEBUG_PRINT("ERROR Code: ")
        DEBUG_PRINTLN(String(((ToLong(SystemStatus) ^ SYSREQ_REMOTE_CONTROL) &
                              SYSREQ_REMOTE_CONTROL) |
                             (1L << MODULE_RESERVED)));
      }
      break;
    default:
      break;
  }

  return status;
}
