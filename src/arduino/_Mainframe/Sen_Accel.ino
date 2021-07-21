/*
  GeoRover Accelerometer functionalities, utilizing the XXXX
  (SparkFun Triple Axis Accelerometer Breakout - MMA8452Q https://www.sparkfun.com/products/12756)

  Primary library used:
    https://github.com/sparkfun/SparkFun_MMA8452Q_Arduino_Library/tree/V_1.1.0

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021
*/

#include <SparkFun_MMA8452Q.h>

MMA8452Q accel;


bool InitializeAccel() {
  bool status = accel.init(SCALE_2G, ODR_1);  // scale +/- 2g and frequency of 1.56 Hz
  SetStatus(MODULE_ACCEL, status);
  return status;
}

void TerminateAccel(){
  //
}

void ReadAccel() {
  if (accel.available())
  {
    // First, use accel.read() to read the new variables:
    accel.read();

    // accel.read() will update two sets of variables.
    // * int's x, y, and z will store the signed 12-bit values
    //   read out of the accelerometer.
    // * floats cx, cy, and cz will store the calculated
    //   acceleration from those 12-bit values. These variables
    //   are in units of g's.
    // Check the two function declarations below for an example
    // of how to use these variables.
    //printCalculatedAccels();
    //printAccels(); // Uncomment to print digital readings

    // The library also supports the portrait/landscape detection
    //  of the MMA8452Q. Check out this function declaration for
    //  an example of how to use that.
    //printOrientation();

    //Serial.println(); // Print new line every time.
  }
}

bool AccelStatus(){
  return GetStatus(MODULE_ACCEL);
}