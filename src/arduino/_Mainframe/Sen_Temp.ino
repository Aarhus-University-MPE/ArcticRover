/*
  GeoRover Temperature Sensor functionalities, utilizing a 10kOhm NTC thermistor
  (https://dk.rs-online.com/web/p/termistor-ic/7062759)

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021
*/

int MeanThermTemp() {
  int tempCount, total;

  for (int i = 0; i < 3; i++) {
    int therm = ThermTemp(i);
    if (therm != 0) {
      total += therm;
      tempCount++;
    }
  }

  return total / tempCount;
}

int ThermTemp(int thermistor) {
  int temp;
  switch (thermistor) {
    case 0:
      temp = analogRead(PA_SENSOR_TEMP1);
      break;
    case 1:
      temp = analogRead(PA_SENSOR_TEMP2);
      break;
    case 2:
      temp = analogRead(PA_SENSOR_TEMP3);
      break;
    default:
      break;
  }
  return temp;
}

bool TemperatureStatus() {
  bool status = MeanThermTemp() > TEMP_SYSTEM_MIN;

  SetStatus(MODULE_TEMP, status);
  return status;
}

bool TemperatureStatus(bool print) {
  if (!print) {
    return TemperatureStatus();
  }

  int meanTemp = MeanThermTemp();

  DEBUG_PRINT("Temperatures: ");
  DEBUG_PRINT("\tT1: ");
  DEBUG_PRINT(ThermTemp(0));
  DEBUG_PRINT("\tT2: ");
  DEBUG_PRINT(ThermTemp(1));
  DEBUG_PRINT("\tT3: ");
  DEBUG_PRINT(ThermTemp(2));
  DEBUG_PRINT("\tMean: ");
  DEBUG_PRINTLN(meanTemp);

  bool status = meanTemp > TEMP_SYSTEM_MIN;

  SetStatus(MODULE_TEMP, status);
  return status;
}

// If temp < minimum temp runs heating period
void HeatingProcess() {
  if (TemperatureStatus()) {
    SystemDisable(MODULE_HEATING);
    return;
  }

  HeatingCheck();
}

void HeatingCheck() {
  if (GetStatus(MODULE_HEATING)) {
    if (millis() - lastMillistHeatingOn > HEATING_DURATION) {
      SystemDisable(MODULE_HEATING);
    }
  } else {
    if (millis() - lastMillistHeatingOff > HEATING_TIMEOUT) {
      SystemEnable(MODULE_HEATING);
    }
  }
}