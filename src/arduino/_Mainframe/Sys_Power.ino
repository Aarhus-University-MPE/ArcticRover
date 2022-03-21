/*
  GeoRover Power control

  Mads Rosenhøj Jeppesen
  Aarhus University
  2021
*/

// BatteryReading = 19.2 kOhm / (19.2 kOhm + 110 kOhm) * Battery Voltage
// 3.567 V -> ~5V ... 730 -> 1022 (int)
// 24.0 V -> 33.6 V
const int batteryLevelVoltage[] = {1022, 1004, 986, 956, 931, 920, 913, 898, 883, 839, 730};
const int batteryLevelPct[]     = {100, 98, 95, 91, 81, 62, 43, 25, 15, 6, 1};
//                            33.6, 33.0, 32.4, 31.44, 30.6, 30.24, 30.0, 29.52, 29.04, 27.6, 24.0

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

  DEBUG_PRINT(F("Battery Level: "));
  DEBUG_PRINTLN(BatteryLevel());

  DEBUG_PRINT(F("Battery Voltage: "));
  DEBUG_PRINTLN(BatteryVoltage());
  return VoltageCheck();
}

int BatteryLevel() {
  int voltage = analogRead(PA_SENSOR_BATT);

  int i;

  while (voltage < batteryLevelVoltage[i]) {
    i++;
  }

  return batteryLevelPct[i];
}

float BatteryVoltage(){
  int voltageInt = analogRead(PA_SENSOR_BATT);

  float voltageRead = voltageInt * 5.0f/1024.0f; 
  
  float voltageBattery = voltageRead *  129.2f/19.2f;
  
  return voltageBattery;
}
