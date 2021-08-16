/*
  GeoRover Power control

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021
*/

// Checks voltage levels above critical values?
bool VoltageCheck()
{
  bool valid = true;

  // Measure voltage, above critical?

  return valid;
}

bool BatteryStatus()
{
  bool valid = true;

  return valid;
}

void SystemEnable(int module)
{
  if (GetStatus(module))
    return;

  bool status = true;

  switch (module)
  {
  case MODULE_PWR_MOTOR:
    digitalWrite(PO_POWER_MOTOR_ON, HIGH);
    delay(50);
    digitalWrite(PO_POWER_MOTOR_ON, LOW);
    DEBUG_PRINTLN("Power to Motors: Enabled.");
    break;
  case MODULE_PWR_5V:
    digitalWrite(PO_POWER_5V, HIGH);
    DEBUG_PRINTLN("Power to Secondary Systems (5V): Enabled");
    break;
  case MODULE_PWR_12V:
    digitalWrite(PO_POWER_12V, HIGH);
    DEBUG_PRINTLN("Power to Secondary Systems (12V): Enabled");
    break;
  case MODULE_RF:
    SystemEnable(MODULE_PWR_5V);
    digitalWrite(PO_POWER_RF, HIGH);
    DEBUG_PRINT("Short Range Communication (RF): ");
    if (InitializeSBUS())
    {
      DEBUG_PRINTLN("Enabled");
    }
    else
    {
      DEBUG_PRINTLN("Error");
      status = false;
    }
    break;
  case MODULE_IRIDIUM:
    SystemEnable(MODULE_PWR_5V);
    digitalWrite(PO_POWER_IRIDIUM, HIGH);
    DEBUG_PRINT("Long Range Communication (Iridium): ");
    if (InitializeIridium())
    {
      DEBUG_PRINTLN("Enabled");
    }
    else
    {
      DEBUG_PRINTLN("Error");
      status = false;
    }
    break;
  case MODULE_SD:
    SystemEnable(MODULE_PWR_5V);
    DEBUG_PRINT("Local Storage: ");
    if (InitializeSDReader())
    {
      DEBUG_PRINTLN("Enabled");
    }
    else
    {
      DEBUG_PRINTLN("Error");
      status = false;
    }
    break;
  case MODULE_ACCEL:
    SystemEnable(MODULE_PWR_5V);
    DEBUG_PRINT("Accelerometer: ");
    if (InitializeAccel())
    {
      DEBUG_PRINTLN("Enabled");
    }
    else
      DEBUG_PRINTLN("Error");
      status = false;
    break;
  case MODULE_GNSS:
    SystemEnable(MODULE_PWR_5V);
    DEBUG_PRINT("Global Navigation Satellite Systems: ");
    if (InitializeGnss())
    {
      DEBUG_PRINTLN("Enabled");
    }
    else
    {
      DEBUG_PRINTLN("Error");
      status = false;
    }
    break;
  default:
    break;
  }

  SetStatus(module, status);
  delay(10);
}

void SystemDisable(int module)
{
  if (!GetStatus(module))
    return;

  switch (module)
  {
  case MODULE_PWR_MOTOR:
    DEBUG_PRINTLN("Power to Motors: Disabled.");
    digitalWrite(PO_POWER_MOTOR_OFF, HIGH);
    delay(50);
    digitalWrite(PO_POWER_MOTOR_OFF, LOW);
    break;
  case MODULE_PWR_5V:
    DEBUG_PRINTLN("Power to Secondary Systems (5V): Disabled");
    digitalWrite(PO_POWER_5V, LOW);
    break;
  case MODULE_PWR_12V:
    DEBUG_PRINTLN("Power to Secondary Systems (12V): Disabled");
    digitalWrite(PO_POWER_12V, LOW);
    break;
  case MODULE_RF:
    digitalWrite(PO_POWER_RF, LOW);
    TerminateSBUS();
    DEBUG_PRINTLN("Short Range Communication (RF): Disabled");
    break;
  case MODULE_IRIDIUM:
    digitalWrite(PO_POWER_IRIDIUM, LOW);
    DEBUG_PRINTLN("Long Range Communication (Iridium): Disabled");
    break;
  case MODULE_ACCEL:
    TerminateAccel();
    DEBUG_PRINTLN("Accelerometer: Disabled");
    break;
  case MODULE_GNSS:
    TerminateGnss();
    DEBUG_PRINTLN("Global Navigation Satellite Systems: Disabled")
    break;
  default:
    break;
  }

  SetStatus(module, false);
}

void SystemEnable()
{
  SystemEnable(MODULE_PWR_MOTOR);
  SystemEnable(MODULE_PWR_5V);
  SystemEnable(MODULE_PWR_12V);
  SystemEnable(MODULE_RF);
  SystemEnable(MODULE_IRIDIUM);
  SystemEnable(MODULE_GNSS);
  SystemEnable(MODULE_SD);
  SystemEnable(MODULE_ACCEL);
}

void SystemDisable()
{
  SystemDisable(MODULE_RF);
  SystemDisable(MODULE_IRIDIUM);
  SystemDisable(MODULE_GNSS);
  SystemDisable(MODULE_ACCEL);
  SystemDisable(MODULE_PWR_MOTOR);
  SystemDisable(MODULE_PWR_5V);
  SystemDisable(MODULE_PWR_12V);
}