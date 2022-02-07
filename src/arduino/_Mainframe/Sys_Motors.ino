/*
  GeoRover Motor control

  Mads Rosenhøj Jepepsen
  Aarhus University
  2022
*/

// Initialize motor power
bool InitializeMotors() {
  if (GetStatus(MODULE_PWR_MOTOR)) {
    delay(10);

    digitalWrite(PO_MOTOR_EN, true);
    return true;
  } else {
    DEBUG_PRINTLN("MOTOR POWER ERROR!");
    return false;
  }
}

// Disable Motor HW Enable
void TerminateMotors() {
  digitalWrite(PO_MOTOR_EN, false);
  SystemDisable(MODULE_MOTORS);
}

// Moves motors based on direction and speed input within the range of [-1 and 1]
// -1 full left, 1 full right (dir)
// -1 full reverse, 1 full forward (speed)
void MotorUpdate(float dir, float speed, float enable) {
  if (!enable) {
    // Stop motors
    SystemDisable(MODULE_MOTORS);
    return;
  } else {
    // Start motors
    SystemEnable(MODULE_MOTORS);
  }

  float velocityLeft;
  float velocityRight;

  SpeedCalculation(dir, speed, velocityLeft, velocityRight);

  // Send command
  motorLeft.Update(velocityLeft);
  motorRight.Update(velocityRight);

  DEBUG_PRINT("Motor Move: ");
  DEBUG_PRINT("Left: ");
  DEBUG_PRINT(velocityLeft);
  DEBUG_PRINT("\t Right: ");
  DEBUG_PRINTLN(velocityRight);
}


void SpeedCalculation(float dir, float speed, float &velocityLeft, float &velocityRight) {
  // Move, handle direction
  float steerFactorLeft = 1;
  float steerFactorRight = 1;

  // turn right, right wheel moves slower
  if (dir > 0) {
    steerFactorRight = steerFactor(dir);
  }
  // turn left, left wheel moves slower
  else if (dir < 0) {
    steerFactorLeft = steerFactor(dir);
  }

  float speedLeft = speed * steerFactorLeft;
  float speedRight = speed * steerFactorRight;

  if (speedLeft < 0)
    velocityLeft = MOTOR_MAX_SPEED_BWD * speedLeft;
  else
    velocityLeft = MOTOR_MAX_SPEED_FWD * speedLeft;

  if (speedRight < 0)
    velocityRight = MOTOR_MAX_SPEED_BWD * speedRight;
  else
    velocityRight = MOTOR_MAX_SPEED_FWD * speedRight;

  if (abs(velocityLeft) < MIN_VELOCITY) velocityLeft = 0;
  if (abs(velocityRight) < MIN_VELOCITY) velocityRight = 0;
}

bool MotorState(){
  return GetStatus(MODULE_MOTORS);
}


// Motors operational?
bool MotorStatus() {
  return (MotorStatusLeft() && MotorStatusRight());
}

bool MotorStatusLeft() {
  return motorLeft.Status();
}

bool MotorStatusRight() {
  return motorRight.Status();
}

// Calculates steering factor from 2nd order function (-2x^2 + 1), used in skid steering
float steerFactor(float dir) {
  float scale;

  if (dir >= 1)
    scale = -1;

  else if (dir <= -1)
    scale = -1;

  else
    scale = -2.0 * (dir * dir) + 1;

  return scale;
}

// Runs motor test, ramps each motor up and down
void MotorTest1() {
  if (GetStatus(MODULE_MOTORS)) {
    float speed = -0.01;
    DEBUG_PRINTLN("Ramping up");
    for (size_t i = 0; i < 101; i++) {
      speed += 0.01;
      MotorUpdate(0, speed, true);
      delay(MOTOR_RAMP_TIME);
    }
    DEBUG_PRINTLN("Ramping down");
    for (size_t i = 0; i < 100; i++) {
      speed -= 0.01;
      MotorUpdate(0, speed, true);
      delay(MOTOR_RAMP_TIME);
    }
  }
}

// Runs motor test, ramps each right, left and back to center
void MotorTest2() {
  if (GetStatus(MODULE_MOTORS)) {
    float speed = MOTOR_MAX_SPEED_FWD * 0.25;
    float dir = -0.02;
    DEBUG_PRINTLN("Turning Right");
    for (size_t i = 0; i < 51; i++) {
      dir += 0.02;
      MotorUpdate(dir, speed, true);
      delay(MOTOR_RAMP_TIME);
    }
    DEBUG_PRINTLN("Turning Left");
    for (size_t i = 0; i < 101; i++) {
      dir -= 0.02;
      MotorUpdate(dir, speed, true);
      delay(MOTOR_RAMP_TIME);
    }
    DEBUG_PRINTLN("Centering");
    for (size_t i = 0; i < 51; i++) {
      dir += 0.02;
      MotorUpdate(dir, speed, true);
      delay(MOTOR_RAMP_TIME);
    }
  }
}
