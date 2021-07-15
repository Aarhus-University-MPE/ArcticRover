/*
  GeoRover Motor control

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021
*/

#include <mcp2515.h> //Library for using CAN Communication

bool InitializeMotors(bool RF_Control) {
  DEBUG_PRINT("Initializing Motors... ");
  InitializeCanBus();
  if (RF_Control) InitializeSBUS();

  if (MotorCheck()) {
    DEBUG_PRINTLN("Motors Initialized.");
    return true;
  }
  else {
    DEBUG_PRINTLN("Motors Error.");
    return false;
  }

}



void TerminateMotors(bool RF_Control) {
  DEBUG_PRINT("Terminating Motors... ");
  TerminateCanBus();
  if (RF_Control) TerminateSBUS();
  DEBUG_PRINTLN("Motors Terminated.");
}

// Moves motors based on direction and speed input within the range of [-1 and 1]
// -1 full left, 1 full right (dir)
// -1 full reverse, 1 full forward (speed)
void MotorMove(float dir, float speed) {
  // Move, handle direction
  float steerFactorLeft = 1;
  float steerFactorRight = 1;

  // turn right
  if (dir > 0) {
    steerFactorRight = steerFactor(dir);
  }
  // turn left
  else if (dir < 0) {
    steerFactorLeft = steerFactor(dir);
  }

  float speedLeft = speed * steerFactorLeft;
  float speedRight = speed * steerFactorRight;

  float velocityLeft;
  float velocityRight;

  if (speedLeft < 0) velocityLeft = MOTOR_MAX_SPEED_BWD * speedLeft;
  else              velocityLeft = MOTOR_MAX_SPEED_FWD * speedLeft;

  if (speedRight < 0)  velocityRight = MOTOR_MAX_SPEED_BWD * speedRight;
  else                velocityRight = MOTOR_MAX_SPEED_FWD * speedRight;

  // Send command via CanBUS
}

bool MotorCheck() {
  bool valid = true;

  // Motors operational?

  return valid;
}

// Calculates steering factor from 2nd order function (-2x^2 + 1), used in skid steering
float steerFactor(float dir) {
  float scale;

  if (dir >= 1) scale = -1;

  else if (dir <= -1) scale = -1;

  else scale = -2.0 * (dir * dir) + 1;

  return scale;
}
