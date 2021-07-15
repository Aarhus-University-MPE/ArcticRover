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


void InitializeAccel() {
  accel.init(SCALE_2G, ODR_1);  // scale +/- 2g and frequency of 1.56 Hz
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


/******************************************************************************
  MMA8452Q_Basic.ino
  SFE_MMA8452Q Library Basic Example Sketch
  Jim Lindblom @ SparkFun Electronics
  Original Creation Date: June 3, 2014
  https://github.com/sparkfun/MMA8452_Accelerometer

  This sketch uses the SparkFun_MMA8452Q library to initialize the
  accelerometer, and stream values from it.

  Hardware hookup:
  Arduino --------------- MMA8452Q Breakout
    3.3V  ---------------     3.3V
    GND   ---------------     GND
  SDA (A4) --\/330 Ohm\/--    SDA
  SCL (A5) --\/330 Ohm\/--    SCL

  The MMA8452Q is a 3.3V max sensor, so you'll need to do some
  level-shifting between the Arduino and the breakout. Series
  resistors on the SDA and SCL lines should do the trick.


  // The loop function will simply check for new data from the
  //  accelerometer and print it out if it's available.
  void loop()
  {
  // Use the accel.available() function to wait for new data
  //  from the accelerometer.
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
    printCalculatedAccels();
    //printAccels(); // Uncomment to print digital readings

    // The library also supports the portrait/landscape detection
    //  of the MMA8452Q. Check out this function declaration for
    //  an example of how to use that.
    printOrientation();

    Serial.println(); // Print new line every time.
  }
  }

  // The function demonstrates how to use the accel.x, accel.y and
  //  accel.z variables.
  // Before using these variables you must call the accel.read()
  //  function!
  void printAccels()
  {
  Serial.print(accel.x, 3);
  Serial.print("\t");
  Serial.print(accel.y, 3);
  Serial.print("\t");
  Serial.print(accel.z, 3);
  Serial.print("\t");
  }

  // This function demonstrates how to use the accel.cx, accel.cy,
  //  and accel.cz variables.
  // Before using these variables you must call the accel.read()
  //  function!
  void printCalculatedAccels()
  {
  Serial.print(accel.cx, 3);
  Serial.print("\t");
  Serial.print(accel.cy, 3);
  Serial.print("\t");
  Serial.print(accel.cz, 3);
  Serial.print("\t");
  }

  // This function demonstrates how to use the accel.readPL()
  // function, which reads the portrait/landscape status of the
  // sensor.
  void printOrientation()
  {
  // accel.readPL() will return a byte containing information
  // about the orientation of the sensor. It will be either
  // PORTRAIT_U, PORTRAIT_D, LANDSCAPE_R, LANDSCAPE_L, or
  // LOCKOUT.
  byte pl = accel.readPL();
  switch (pl)
  {
  case PORTRAIT_U:
    Serial.print("Portrait Up");
    break;
  case PORTRAIT_D:
    Serial.print("Portrait Down");
    break;
  case LANDSCAPE_R:
    Serial.print("Landscape Right");
    break;
  case LANDSCAPE_L:
    Serial.print("Landscape Left");
    break;
  case LOCKOUT:
    Serial.print("Flat");
    break;
  }
  }
*/
