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
  bool status = accel.begin();  // scale +/- 2g and frequency of 1.56 Hz
  accel.setScale(SCALE_2G);
  accel.setDataRate(ODR_12);
  SetStatus(MODULE_ACCEL, status);
  return status;
}

void TerminateAccel(){
  //
}


// accel.read() will update two sets of variables.
// * int's x, y, and z will store the signed 12-bit values
//   read out of the accelerometer.
// * floats cx, cy, and cz will store the calculated
//   acceleration from those 12-bit values. These variables
//   are in units of g's.
void ReadAccel() {
  if (accel.available())
  {
    accel.read();
  }
}

bool AccelStatus(){
  return GetStatus(MODULE_ACCEL);
}

bool AccelTest(bool printRes){
  if(printRes){
    DEBUG_PRINT("Accelerometer: \t "); 
    if(AccelStatus()){
      ReadAccel();
      DEBUG_PRINT("x: ");
      DEBUG_PRINT((float)accel.cx);
      DEBUG_PRINT("\t y: ");
      DEBUG_PRINT((float)accel.cy);
      DEBUG_PRINT("\t z: ");
      DEBUG_PRINTLN((float)accel.cz);
    }
    else{
      DEBUG_PRINTLN("ERROR");
    }
  }
  
  return AccelStatus();
}