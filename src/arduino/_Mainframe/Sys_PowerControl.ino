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

void SystemEnable(int module)
{
  if (GetStatus(module))
    return;

  bool status = true;

  switch (module)
  {
  case MODULE_PWR_MOTOR:
    digitalWrite(PO_POWER_MOTOR, HIGH);
    DEBUG_PRINTLN("Power to Motors: Enabled.");
    break;
  case MODULE_PWR_5V:
    digitalWrite(PO_POWER_5V, HIGH);
    DEBUG_PRINTLN("Power to Secondary Systems (5V): Enabled");
    break;
  case MODULE_PWR_12V:
    digitalWrite(PO_POWER_12V, HIGH);
    DEBUG_PRINTLN("Power to Secondary Systems(12V): Enabled");
    break;
  case MODULE_RF:
    SystemEnable(MODULE_PWR_5V);
    digitalWrite(PO_POWER_RF, HIGH);
    if (InitializeSBUS())
    {
      DEBUG_PRINTLN("Short Range Communication (RF): Enabled");
    }
    else
      status = false;
    break;
  case MODULE_IRIDIUM:
    SystemEnable(MODULE_PWR_5V);
    digitalWrite(PO_POWER_IRIDIUM, HIGH);
    if (InitializeIridium())
    {
      DEBUG_PRINTLN("Long Range Communication (Iridium): Enabled");
    }
    else
      status = false;
    break;
  case MODULE_SD:
    SystemEnable(MODULE_PWR_5V);
    if (InitializeSDReader())
    {
      DEBUG_PRINTLN("Local Storage: Enabled");
    }
    else
      status = false;
    break;
  case MODULE_GNSS:
    SystemEnable(MODULE_PWR_5V);
    if (InitializeGnss())
    {
      DEBUG_PRINTLN("Global Positioning System: Enabled")
    }
    else
      status = false;
  default:
    break;
  }

  SetStatus(module, status);
}

void SystemDisable(int module)
{
  if (!GetStatus(module))
    return;

  switch (module)
  {
  case MODULE_PWR_MOTOR:
    DEBUG_PRINTLN("Power to Motors: Disabled.");
    digitalWrite(PO_POWER_MOTOR, LOW);
    break;
  case MODULE_PWR_5V:
    DEBUG_PRINTLN("Power to Secondary Systems (5V): Disabled");
    digitalWrite(PO_POWER_5V, LOW);
    break;
  case MODULE_PWR_12V:
    DEBUG_PRINTLN("Power to Secondary Systems(12V): Disabled");
    digitalWrite(PO_POWER_12V, LOW);
    break;
  case MODULE_RF:
    digitalWrite(PO_POWER_RF, LOW);
    InitializeSBUS();
    DEBUG_PRINTLN("Short Range Communication (RF): Disabled");
    break;
  case MODULE_IRIDIUM:
    digitalWrite(PO_POWER_IRIDIUM, LOW);
    DEBUG_PRINTLN("Long Range Communication (Iridium): Disabled");
    break;
  case MODULE_GNSS:
    TerminateGnss();
    DEBUG_PRINTLN("Global Positioning System: Disabled")
    break;
  default:
    break;
  }

  SetStatus(module, false);
}


void SystemDisable(){
  SystemDisable(MODULE_PWR_MOTOR);
  SystemDisable(MODULE_PWR_5V);
  SystemDisable(MODULE_PWR_12V);
}