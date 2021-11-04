/*
  GeoRover Motor control

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021
*/

// Initialize motor power
bool InitializeMotors() {

  if (MotorStatus()) {    
    analogWrite(PP_MOTOR_THRTL_RIGHT, 0);
    analogWrite(PP_MOTOR_THRTL_LEFT,  0);
    
    delay(10);

    digitalWrite(PO_MOTOR_EN_LEFT,  true);
    digitalWrite(PO_MOTOR_EN_RIGHT, true);
    return true;
  }
  else {
    return false;
  }

}


// Cut motor Power and disable control outputs
void TerminateMotors() {
  digitalWrite(PO_MOTOR_EN_LEFT,  false);
  digitalWrite(PO_MOTOR_EN_RIGHT, false);

  analogWrite(PP_MOTOR_THRTL_RIGHT, 0);
  analogWrite(PP_MOTOR_THRTL_LEFT,  0);
}

// Moves motors based on direction and speed input within the range of [-1 and 1]
// -1 full left, 1 full right (dir)
// -1 full reverse, 1 full forward (speed)
void MotorMove(float dir, float speed, float enable) {
  if(!enable){
    // Stop motors
    DEBUG_PRINTLN("Motor Halt");

    return;
  }
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

  // Send command
  DEBUG_PRINT("Motor Move: ");
  DEBUG_PRINT("Left: ");
  DEBUG_PRINT(velocityLeft);;
  DEBUG_PRINT("\t Right: ");
  DEBUG_PRINTLN(velocityRight);  
}

bool MotorState(){
  bool motorState = false;

  // motor currently running?

  return motorState;
}


bool MotorStatus() {
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

// Runs motor test, ramps each motor up and down
void MotorTest(){
  if (GetStatus(MODULE_MOTORS)){
    float speed = 0;
    for (size_t i = 0; i < 100; i++)
    {
      MotorMove(0,speed,true);
      speed += 0.01;
      delay(MOTOR_RAMP_TIME);
    }
    for (size_t i = 0; i < 100; i++)
    {
      MotorMove(0,speed,true);
      speed -= 0.01;
      delay(MOTOR_RAMP_TIME);
    }
    
  }
}
