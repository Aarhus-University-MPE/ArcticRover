/*
  GeoRover Temperature Sensor functionalities, utilizing a 10kOhm NTC thermistor
  (https://dk.rs-online.com/web/p/termistor-ic/7062759)

  Mads Rosenhøj Jeppesen
  Aarhus University
  2021
*/

#include "Math.h"

int tempTestState        = 0;
long millisTempTestStart = 0;
long millisLastTempPrint = 0;
long lastMillisTempCheck = 0;

float B25_85 = 3988.0f;   // Kelvin
float R25    = 10000.0f;  // thermistor resistance at 25 °C - 10 kOhm
float R_K    = 20000.0f;  // Voltage divider resistor - 20 kOhm

int MeanThermTemp() {
  int tempCount = 0, total = 0;

  for (int i = 0; i < 3; i++) {
    int therm = ThermTemp(i);
    if (therm != -1000) {
      total += therm;
      tempCount++;
    }
  }

  // No valid readings
  if (tempCount == 0) {
    return TEMP_SYSTEM_MIN + 1;  // Avoids running heating elements
  }
  return total / tempCount;
}

int ThermTemp(int thermistor) {
  int tempValue;
  switch (thermistor) {
    case 0:
      tempValue = analogRead(PA_SENSOR_TEMP1);
      break;
    case 1:
      tempValue = analogRead(PA_SENSOR_TEMP2);
      break;
    case 2:
      tempValue = analogRead(PA_SENSOR_TEMP3);
      break;
    default:
      break;
  }

  int temp = ThermistorConversion(tempValue);

  if (temp < MIN_VALID_THERM_VALUE || temp > MAX_VALID_THERM_VALUE) {
    return -1000;  // Invalid Temp
  }
  return temp;
}

void PrintTemperature() {
  DEBUG_PRINT(F("System Temperatures: "));
  DEBUG_PRINT(F("\tT1: "));
  DEBUG_PRINT(ThermTemp(0));
  DEBUG_PRINT(F("\tT2: "));
  DEBUG_PRINT(ThermTemp(1));
  DEBUG_PRINT(F("\tT3: "));
  DEBUG_PRINT(ThermTemp(2));
  DEBUG_PRINT(F("\tMean: "));
  DEBUG_PRINTLN(MeanThermTemp());

  if (TemperatureBelowMax) {
    DEBUG_PRINTLINE();
    DEBUG_PRINTLN(F("WARNING: System Overheated!"));
    DEBUG_PRINTLINE();
  }
}

/* Thermistor Equation
  Temp(V_out) = B25_85 / (ln ((V_s * R_k / V_out)  - R_K ) / (R_25 * e^(-B25_85/T1))) - 273.15K
*/
const float aTherm = R25 * exp(-B25_85 / (25.0f + 273.15f));

float ThermistorConversion(int tempValue) {
  float voltage = tempValue * 5.0f / 1024.0f;

  return B25_85 / (log(((5.0f * R_K) / voltage - R_K) / aTherm)) - 273.15f;
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

bool HeatingStatus() {
  return GetStatus(MODULE_HEATING);
}

bool TemperatureStatus() {
  return TemperatureAboveMin() && TemperatureBelowMax();
}

// Returns true if temperature is below maximum temperature
bool TemperatureBelowMax() {
  return MeanThermTemp() < TEMP_SYSTEM_MAX;
}

// Returns true if temperature is above minimum temperature (+hysteresis if currently heating)
bool TemperatureAboveMin() {
  if (!GetStatus(MODULE_HEATING)) {
    return MeanThermTemp() > TEMP_SYSTEM_MIN;
  } else {
    return MeanThermTemp() > TEMP_SYSTEM_MIN + TEMP_SYSTEM_HYSTERESIS;
  }
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
        meanTemp            = MeanThermTemp();
        DEBUG_PRINT(F("Temperatures: "));
        DEBUG_PRINT(F("\tT1: "));
        DEBUG_PRINT(ThermTemp(0));
        DEBUG_PRINT(F("\tT2: "));
        DEBUG_PRINT(ThermTemp(1));
        DEBUG_PRINT(F("\tT3: "));
        DEBUG_PRINT(ThermTemp(2));
        DEBUG_PRINT(F("\tMean: "));
        DEBUG_PRINTLN(meanTemp);
      }

      if (millis() - millisTempTestStart > SYS_TEST_DURATION) tempTestState++;
      break;
    case 2:
      testDone      = true;
      tempTestState = 0;
      break;
    default:
      break;
  }

  return testDone;
}

// If temp < minimum temp runs heating period
void HeatingProcess() {
  if (millis() - lastMillisTempCheck < TEMP_CHECK_PERIOD) return;

  lastMillisTempCheck = millis();

  if (TemperatureAboveMin()) {
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