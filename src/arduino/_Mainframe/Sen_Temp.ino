/*
  GeoRover Temperature Sensor functionalities, utilizing a 10kOhm NTC thermistor
  (https://dk.rs-online.com/web/p/termistor-ic/7062759)

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021
*/

int tempTestState = 0;
long millisTempTestStart = 0;
long millisLastTempPrint = 0;

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

bool HeatingStart() {
  if (millis() - lastMillistHeatingOff < HEATING_TIMEOUT) {
    return false;
  }

  digitalWrite(PO_POWER_HEATING, HIGH);
  lastMillistHeatingOn = millis();

  return true;
}

bool HeatingStop() {
  digitalWrite(PO_POWER_HEATING, LOW);
  lastMillistHeatingOff = millis();
}

bool TemperatureStatus() {
  bool status = MeanThermTemp() > TEMP_SYSTEM_MIN;

  SetStatus(MODULE_TEMP, status);
  return status;
}

// Runs temperature test for a duration, returns true once test is complete
bool TemperatureTest() {
  bool testDone = false;
  int meanTemp;
  switch (tempTestState) {
    case 0:
      DEBUG_PRINT(F("Thermistor feed starting for: "));
      DEBUG_PRINT(SYS_TEST_DURATION);
      DEBUG_PRINTLN(F(" ms"));
      millisTempTestStart = millis();
      tempTestState++;
      break;
    case 1:
      if (millis() - millisLastTempPrint > SYS_PRINT_PERIOD_LONG) {
        millisLastTempPrint = millis();
        meanTemp = MeanThermTemp();
        DEBUG_PRINT(F("Temperatures: "));
        DEBUG_PRINT(F("\tT1: "));
        DEBUG_PRINT(ThermTemp(0));
        DEBUG_PRINT(F("\tT2: "));
        DEBUG_PRINT(ThermTemp(1));
        DEBUG_PRINT(F("\tT3: "));
        DEBUG_PRINT(ThermTemp(2));
        DEBUG_PRINT(F("\tMean: "));
        DEBUG_PRINTLN(meanTemp);

        SetStatus(MODULE_TEMP, meanTemp > TEMP_SYSTEM_MIN);
      }

      if (millis() - millisTempTestStart > SYS_TEST_DURATION) tempTestState++;
      break;
    case 2:
      testDone = true;
      tempTestState = 0;
      break;
    default:
      break;
  }

  
  return testDone;
}

// If temp < minimum temp runs heating period
void HeatingProcess() {
  if (TemperatureStatus()) {
    SystemDisable(MODULE_HEATING);
    return;
  }

  HeatingCheck();
}

// If currently heating checks for maximum heating duration
// If not currently heating checks for timeout to reactivate heating
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