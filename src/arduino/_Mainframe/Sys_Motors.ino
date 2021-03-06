/*
  GeoRover Motor control

  Mads Rosenhøj Jeppesen
  Aarhus University
  2022
*/
unsigned long millisLastMotorStep;

// Initialize motor power
bool InitializeMotors() {
  // if (!GetStatus(MODULE_PWR_MOTOR)) {
  //   return false;
  // }

  digitalWrite(PO_MOTOR_EN, true);

  delay(50);

  // return MotorStatus();
  return true;
}

// Disable Motor HW Enable
void TerminateMotors() {
  digitalWrite(PO_MOTOR_EN, false);
}

// Moves motors based on direction and speed input within the range of [-1 and 1]
// -1 full left, 1 full right (dir)
// -1 full reverse, 1 full forward (speed)
void MotorUpdate(float dir, float speed) {
  float velocityLeft;
  float velocityRight;

  // Clamp inputs
  dir   = max(-1.0f, min(1.0f, dir));
  speed = max(-1.0f, min(1.0f, speed));

  SpeedCalculation(dir, speed, velocityLeft, velocityRight);

  // Update CAN messages
  motorLeft.Update(velocityLeft);
  motorRight.Update(velocityRight);
}

// Moves motors based on direction and speed input within the range of [-1 and 1]
// -1 full left, 1 full right (dir)
// -1 full reverse, 1 full forward (speed)
void MotorUpdateTorque(float dir, float speed) {
  float torqueLeft;
  float torqueRight;

  // Clamp inputs
  dir   = max(-1.0f, min(1.0f, dir));
  speed = max(-1.0f, min(1.0f, speed));

  TorqueCalculation(dir, speed, torqueLeft, torqueRight);

  // Update CAN messages
  motorLeft.UpdateTorque(torqueLeft);
  motorRight.UpdateTorque(torqueRight);
}

// Calculates skid steering based on direction and speed
void TorqueCalculation(float dir, float speed, float &torqueLeft, float &torqueRight) {
  float steerFactorLeft  = 1.0f;
  float steerFactorRight = 1.0f;

  // turn right, right wheel moves slower
  if (dir > 0) {
    steerFactorRight = SteerFactor(dir);
  }
  // turn left, left wheel moves slower
  else if (dir < 0) {
    steerFactorLeft = SteerFactor(dir);
  }

  torqueLeft  = speed * steerFactorLeft;
  torqueRight = speed * steerFactorRight;

  if (abs(torqueLeft) < MIN_TORQUE) torqueLeft = 0;
  if (abs(torqueRight) < MIN_TORQUE) torqueRight = 0;
}

// Calculates skid steering based on direction and speed
void SpeedCalculation(float dir, float speed, float &velocityLeft, float &velocityRight) {
  float steerFactorLeft  = 1.0f;
  float steerFactorRight = 1.0f;

  // turn right, right wheel moves slower
  if (dir > 0) {
    steerFactorRight = SteerFactor(dir);
  }
  // turn left, left wheel moves slower
  else if (dir < 0) {
    steerFactorLeft = SteerFactor(dir);
  }

  float speedLeft  = speed * steerFactorLeft;
  float speedRight = speed * steerFactorRight;

  if (speedLeft < 0) {
    velocityLeft = MOTOR_MAX_SPEED_BWD * speedLeft;
  } else {
    velocityLeft = MOTOR_MAX_SPEED_FWD * speedLeft;
  }

  if (speedRight < 0) {
    velocityRight = MOTOR_MAX_SPEED_BWD * speedRight;
  } else {
    velocityRight = MOTOR_MAX_SPEED_FWD * speedRight;
  }

  if (abs(velocityLeft) < MIN_VELOCITY) velocityLeft = 0;
  if (abs(velocityRight) < MIN_VELOCITY) velocityRight = 0;
}

// Run CAN cycle until both motors report NO ERROR or times out
bool MotorCycle() {
  // if (!GetStatus(MODULE_CANBUS)) {
  //   return false;
  // }
  if (!GetStatus(MODULE_MOTORS)) {
    return false;
  }
  if (!GetStatus(MODULE_PWR_MOTOR)) {
    return false;
  }

  MotorUpdate(0, 0);
  unsigned long millisMotorStart = millis();

  // (!motorLeft.CanStatus() || !motorRight.CanStatus()) && 1

  // Run canbus for a while to get motor states and test data connection
  while ((!motorLeft.CanStatus() || !motorRight.CanStatus()) && (millis() - millisMotorStart) < MOTOR_STARTUP_TIMEOUT) {
    if (!CanBusProcess()) {
      return false;
    }
  }

  return MotorCanStatus();
}

// Returns true if both motors are operational
bool MotorState() {
  return motorLeft.GetState() || motorRight.GetState();
}

// Motors operational?
bool MotorStatus() {
  // bool status = (MotorStatusLeft() && MotorStatusRight());
  bool status = GetStatus(MODULE_MOTORS);

  return status;
}

// Returns true if both motors have a valid can signal
bool MotorCanStatus() {
  return motorLeft.CanStatus() && motorRight.CanStatus();
}
// Left motor status
bool MotorStatusLeft() {
  return motorLeft.Status();
}

// Right motor status
bool MotorStatusRight() {
  return motorRight.Status();
}

// Calculates steering factor from 2nd order function (-1x^2 + 1), used in skid steering
float SteerFactor(float dir) {
  return -1.0f * (dir * dir) + 1.0f;  // <-- Steer formula
}

// Full motor test
bool MotorTest() {
  if (!GetStatus(MODULE_MOTORS)) {
    return true;
  }
  bool testDone = false;
  static int motorTestState;

  switch (motorTestState) {
    case 0:
      DEBUG_PRINTLN(F("Motor Test 1 - Linear Ramp"));
      DEBUG_PRINTLINE();
      motorTestState++;
      break;
    case 1:
      if (MotorTestRamp()) motorTestState++;
      break;
    case 2:
      DEBUG_PRINTLN(F("Motor Test 2 - Steering"));
      DEBUG_PRINTLINE();
      motorTestState++;
      break;
    case 3:
      if (MotorTestSteer()) motorTestState++;
      break;
    case 4:
      motorTestState = 0;
      testDone       = true;
      MotorUpdate(0, 0);
    default:
      break;
  }

  CanBusProcess();

  return testDone;
}

// Runs Motor Ramp Test, ramps up to maximum speed FWD -> Ramps down to 0 -> Ramps up to maximum speed BWD -> Ramps down to 0
bool MotorTestRamp() {
  if (!GetStatus(MODULE_MOTORS)) {
    return true;
  }

  static int motorTestRampState;
  static float speed;

  bool testDone = false;
  switch (motorTestRampState) {
    case 0:
      DEBUG_PRINTLN(F("Ramping up - FWD"));
      motorTestRampState++;
      break;
    case 1:
      if (millis() - millisLastMotorStep > MOTOR_RAMP_TIME) {
        MotorUpdate(0, speed);
        speed += 0.01;
        millisLastMotorStep = millis();
      }
      if (speed >= 1) motorTestRampState++;
      break;
    case 2:
      DEBUG_PRINTLINE();
      DEBUG_PRINTLN(F("Ramping down"));
      motorTestRampState++;
      break;
    case 3:
      if (millis() - millisLastMotorStep > MOTOR_RAMP_TIME) {
        MotorUpdate(0, speed);
        speed -= 0.01;
        millisLastMotorStep = millis();
      }
      if (speed <= 0) {
        speed = 0;
        MotorUpdate(0, speed);
        motorTestRampState++;
      }
      break;
    case 4:
      DEBUG_PRINTLINE();
      DEBUG_PRINTLN(F("Ramping up - BWD"));
      motorTestRampState++;
      break;
    case 5:
      if (millis() - millisLastMotorStep > MOTOR_RAMP_TIME) {
        MotorUpdate(0, speed);
        speed -= 0.01;
        millisLastMotorStep = millis();
      }
      if (speed <= -1) motorTestRampState++;
      break;
    case 6:
      DEBUG_PRINTLINE();
      DEBUG_PRINTLN(F("Ramping down"));
      motorTestRampState++;
      break;
    case 7:
      if (millis() - millisLastMotorStep > MOTOR_RAMP_TIME) {
        MotorUpdate(0, speed);
        speed += 0.01;
        millisLastMotorStep = millis();
      }
      if (speed >= 0) {
        speed = 0;
        motorTestRampState++;
        MotorUpdate(0, speed);
      }
      break;
    case 8:
      DEBUG_PRINTLINE();
      DEBUG_PRINTLN(F("Motor Test 2 - Steering"));
      motorTestRampState++;
      break;
    case 9:
      motorTestRampState = 0;
      testDone           = true;
      MotorUpdate(0, 0);
    default:
      testDone = true;
      break;
  }

  return testDone;
}

// Runs Motor Steering Test, Runs at lower speed, turns max steer right -> Max Steer Left -> Centered
bool MotorTestSteer() {
  if (!GetStatus(MODULE_MOTORS)) {
    return true;
  }
  static int motorTestSteerState;
  static float dir;
  float speed   = MOTOR_MAX_SPEED_FWD * 0.25;
  bool testDone = false;

  switch (motorTestSteerState) {
    case 0:
      DEBUG_PRINTLN("Turning Right -->");
      motorTestSteerState++;
      break;
    case 1:
      if (millis() - millisLastMotorStep > MOTOR_RAMP_TIME) {
        MotorUpdate(dir, speed);
        dir += 0.02;
        millisLastMotorStep = millis();
      }
      if (dir >= 1) motorTestSteerState++;
      break;
    case 2:
      DEBUG_PRINTLINE();
      DEBUG_PRINTLN("Turning Left <--");
      motorTestSteerState++;
      break;
    case 3:
      if (millis() - millisLastMotorStep > MOTOR_RAMP_TIME) {
        MotorUpdate(dir, speed);
        dir -= 0.02;
        millisLastMotorStep = millis();
      }
      if (dir <= -1) motorTestSteerState++;
      break;
    case 4:
      DEBUG_PRINTLINE();
      DEBUG_PRINTLN("Centering --> || <--");
      motorTestSteerState++;
      break;
    case 5:
      if (millis() - millisLastMotorStep > MOTOR_RAMP_TIME) {
        MotorUpdate(dir, speed);
        dir += 0.02;
        millisLastMotorStep = millis();
      }
      if (dir >= 0) motorTestSteerState++;
      break;
    case 6:
      motorTestSteerState = 0;
      testDone            = true;
      MotorUpdate(0, 0);
    default:
      testDone = true;
      break;
  }

  return testDone;
}