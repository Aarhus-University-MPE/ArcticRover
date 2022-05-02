/*
  GeoRover Accelerometer functionalities
  - Sensor: SparkFun Triple Axis Accelerometer Breakout - MMA8452Q https://www.sparkfun.com/products/12756)
  - Library Dependency: https://github.com/sparkfun/SparkFun_MMA8452Q_Arduino_Library

  Mads Rosenhøj Jeppesen
  Aarhus University
  2021
*/

#include <SparkFun_MMA8452Q.h>

MMA8452Q accel;

int accTestState        = 0;
long millisAccTestStart = 0;
long millisLastAccPrint = 0;

bool InitializeAccel() {
  bool status = accel.begin();  // scale +/- 2g and frequency of 1.56 Hz
  accel.setScale(SCALE_2G);
  accel.setDataRate(ODR_12);
  SetStatus(MODULE_ACCEL, status);
  return status;
}

void TerminateAccel() {
  //
}

// accel.read() will update two sets of variables.
// * int's x, y, and z will store the signed 12-bit values
//   read out of the accelerometer.
// * floats cx, cy, and cz will store the calculated
//   acceleration from those 12-bit values. These variables
//   are in units of g's.
void ReadAccel() {
  if (accel.available()) {
    accel.read();
  }
}

bool AccelStatus() {
  if (!GetStatus(MODULE_ACCEL)) return false;

  return accel.cz > MIN_ACCEL_TILT;
}

bool AccelTest() {
  bool testDone = false;
  switch (accTestState) {
    case 0:
      DEBUG_PRINT(F("Accelerometer feed starting for: "));
      DEBUG_PRINT(SYS_TEST_DURATION);
      DEBUG_PRINTLN(F(" ms"));
      millisAccTestStart = millis();
      accTestState++;
      break;
    case 1:
      if (millis() - millisLastAccPrint > SYS_PRINT_PERIOD) {
        millisLastAccPrint = millis();
        DEBUG_PRINT(F("Accelerometer: "));
        if (AccelStatus()) {
          ReadAccel();
          DEBUG_PRINT(F("x: "));
          DEBUG_PRINT((float)accel.cx);
          DEBUG_PRINT(F("\t y: "));
          DEBUG_PRINT((float)accel.cy);
          DEBUG_PRINT(F("\t z: "));
          DEBUG_PRINTLN((float)accel.cz);
        } else {
          DEBUG_PRINTLN(F("ERROR"));
        }
      }

      if (millis() - millisAccTestStart > SYS_TEST_DURATION) accTestState++;
      break;
    case 2:
      testDone     = true;
      accTestState = 0;
      DEBUG_PRINTLINE();
    default:
      break;
  }

  return testDone;
}