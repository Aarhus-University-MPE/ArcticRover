/*
  GeoRover Power control

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021
*/

const int batteryLevelVoltage[] = {255, 250, 245, 238, 231, 229, 227, 223, 220, 209, 181};
const int batteryLevelPct[] = {100, 98, 95, 91, 81, 62, 43, 25, 15, 6, 1};

// Checks voltage levels above critical values?
bool VoltageCheck() {
  return BatteryLevel() > BATTERY_MIN_LEVEL;
}

bool BatteryStatus() {
  return VoltageCheck();
}

bool BatteryStatus(bool print) {
  if (!print) {
    return VoltageCheck();
  }

  DEBUG_PRINT("Battery Level: ");
  DEBUG_PRINTLN(BatteryLevel());

  return VoltageCheck();
}

int BatteryLevel() {
  int voltage = analogRead(PA_SENSOR_BATT);

  DEBUG_PRINTLN(voltage);
  
  int i;

  while (voltage < batteryLevelVoltage[i]) {
    i--;
  }

  return batteryLevelPct[i];
}