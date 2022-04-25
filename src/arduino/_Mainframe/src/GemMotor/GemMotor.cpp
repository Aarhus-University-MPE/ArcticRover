/*
  GeoRover Motor Class - Manages GEM Motor Functionalities.

  Mads Rosenhøj Jeppesen
  Aarhus University
  2022
*/

#include "GemMotor.h"

#include "Arduino.h"

#if DIAGNOSTICS
#define TIMEOUTMSG()          \
  Serial.print("Timeout - "); \
  Serial.println(TX_id, HEX)
#define SENTMSG()          \
  Serial.print("Sent - "); \
  Serial.println(TX_id, HEX)
#define RECEIVEDMSG()          \
  Serial.print("Received - "); \
  Serial.println(TX_id, HEX)
#else
#define TIMEOUTMSG()
#define SENTMSG()
#define RECEIVEDMSG()
#endif

// Constructor
GemMotor::GemMotor(int _TX_id, int _RX_id) {
  GemMotor::TX_id    = _TX_id;
  GemMotor::RX_id    = _RX_id;
  GemMotor::swEnable = SW_DISABLE;

  GemMotor::canMsg.can_id  = GemMotor::TX_id;
  GemMotor::canMsg.can_dlc = 3;
}

// returns latest motor RPM value
int GemMotor::GetRpm() { return GemMotor::rpm; }

// returns true if motor is currently running
bool GemMotor::GetState() { return (GemMotor::motorState == MOTOR_RUN); }

void GemMotor::SetTorqueMode(bool torqueMode) {
  if (torqueMode) {
    GemMotor::controlMode = GemMotor::CONTROL_TORQUE;
  } else {
    GemMotor::controlMode = GemMotor::CONTROL_SPEED;
  }
}

// returns true if TX flag is set, indicating ready to send msg to motor
bool GemMotor::GetCanTxStatus() { return GemMotor::canTxStatus; }

// returns true if RX flag is set, indicating awaiting motor response msg
bool GemMotor::GetCanRxStatus() { return GemMotor::canRxStatus; }

// Set TX flag, unset RX flag, indicate ready to send message to motor
void GemMotor::SetCanTxStatus() {
  GemMotor::canTxStatus = true;
  GemMotor::canRxStatus = false;
}

// Set RX flag, unset TX flag, indicate ready awaiting motor return message, timestamp timeout
void GemMotor::SetCanRxStatus() {
  GemMotor::canTxStatus       = false;
  GemMotor::canRxStatus       = true;
  GemMotor::canRxTimeoutStart = millis();
}

// Check time since RX flag set and compare to timeout duration, if timeout unset RX flag
GemMotor::CAN_ERROR GemMotor::CheckCanRxTimeout() {
  if (!GemMotor::canRxStatus) {
    return GemMotor::CAN_ERROR_IDLE;
  }
  GemMotor::canRxTimeout = (millis() - GemMotor::canRxTimeoutStart > CAN_TIMEOUT_DURATION);

  if (GemMotor::canRxTimeout) {
    GemMotor::canRxStatus = false;
    validStatus           = false;
    TIMEOUTMSG();
    return GemMotor::CAN_ERROR_TIMEOUT;
  }
  return GemMotor::CAN_ERROR_WAITING;
}

// Unset RX and TX flag
void GemMotor::ResetCanStatus() {
  GemMotor::canTxStatus = false;
  GemMotor::canRxStatus = false;
}

// Update controlvalue based on target velocity, if below min disables motor
void GemMotor::Update(float velocity) {
  if (abs(velocity) < MIN_VELOCITY) {
    GemMotor::swEnable     = SW_DISABLE;
    GemMotor::controlValue = 0;
    return;
  }

  int dir;
  if (velocity < 0) {
    dir = -1;
  } else {
    dir = 1;
  }

  GemMotor::swEnable = SW_ENABLE;
  GemMotor::SetTorqueMode(false);

  // Clamp velocity to (max speed bwd < velocity < max speed fwd)
  velocity = max(-MOTOR_MAX_SPEED_BWD, min(MOTOR_MAX_SPEED_FWD, velocity));

  GemMotor::rpmTarget    = abs(velocity) * VEL_RPM_SCALE * RPM_CONTROL_SCALE;
  GemMotor::controlValue = min(GemMotor::rpmTarget, MAX_CONTROL_VALUE) * dir;
}

// Update controlvalue based on target velocity, if below min disables motor
void GemMotor::UpdateTorque(float torque) {
  if (abs(torque) < MIN_TORQUE) {
    GemMotor::swEnable     = SW_DISABLE;
    GemMotor::controlValue = 0;
    return;
  }

  int dir;
  if (torque < 0) {
    dir = -1;
  } else {
    dir = 1;
  }

  GemMotor::swEnable = SW_ENABLE;
  GemMotor::SetTorqueMode(true);

  // Clamp torque to (max torque bwd < torque < max torque fwd)
  torque = max(-MOTOR_MAX_TORQUE_BWD, min(MOTOR_MAX_TORQUE_FWD, torque));

  GemMotor::torqueTarget = abs(torque) * TORQUE_CONTROL_SCALE;

  GemMotor::controlValue = min(GemMotor::torqueTarget, MAX_CONTROL_VALUE_TORQUE) * dir;
}

// Prints current motor parameters returns error if no motor messages received
GemMotor::ERROR GemMotor::PrintStatus() {
  // Serial.print(F("Motor Status "));
  // Serial.print(GemMotor::TX_id, HEX);
  if (!GemMotor::validStatus) {
    Serial.println(F(": Invalid"));
    return ERROR_INVALID_STATUS;
  }

  // GemMotor::PrintControl(true);
  // GemMotor::PrintInverterState(true);
  if (!GemMotor::MotorError()) {
    GemMotor::PrintError(true);
  } else {
    // Serial.println(F(": OK"));
  }
  // GemMotor::PrintWarning(true);

  return ERROR_OK;
}

// Returns canMsg pointer constructed from motor state, set using Update(velocity), sets RX status
struct can_frame* GemMotor::GetCanMsg() {
  GemMotor::BuildCanMsg();

  GemMotor::SetCanRxStatus();

  SENTMSG();

  return GemMotor::canMsgPtr;
}

// Update canMsg values
void GemMotor::BuildCanMsg() {
  // Control values
  GemMotor::canMsg.data[1] = GemMotor::controlValue >> 8 & 0xFF;
  GemMotor::canMsg.data[0] = GemMotor::controlValue & 0xFF;

  // 0 .. 1 Control Mode
  // 2 .. 4 Motor Mode
  // 5      SW enable
  // 6      reserved
  // 7      debug_mode
  GemMotor::canMsg.data[2] =
      GemMotor::controlMode | (GemMotor::mode) << 2 | (GemMotor::swEnable) << 5;
}

// Updates motor status and returns current motor status (Error)
bool GemMotor::Status() {
  MotorStatusUpdate();
  return GemMotor::motorStatus;
}

// Returns can status of motor (valid Can data)
bool GemMotor::CanStatus() {
  return GemMotor::validStatus;
}

// Updates current motor status, if error reported for more than timeout duration (5000 ms) unsets motor status
// Timeout implemented due to motor capacitor taking time to charge up
void GemMotor::MotorStatusUpdate() {
  // Check latest motor state for error
  if (GemMotor::motorState != MOTOR_ERROR && GemMotor::validStatus && GemMotor::MotorError()) {
    GemMotor::motorStatus = true;
    // GemMotor::ResetMotorStatus();
    // return;
  } else {
    // GemMotor::ErrorTimeout();
    GemMotor::motorStatus = false;
  }
}
// returns false if motors report error
bool GemMotor::MotorError() {
  for (size_t i = 0; i < 64; i++) {
    if (GemMotor::error[i] == 1) {
      return false;
    }
  }
  return true;
}

// Timeout error model - Currently unused
void GemMotor::ErrorTimeout() {
  // Check if error previously not reported - Set timestamp for first error
  if (!GemMotor::motorError) {
    GemMotor::motorError       = true;
    GemMotor::firstErrorMillis = millis();
    return;
  }

  // Check time passed since first error, if above timeout set motor state as error.
  if (millis() - GemMotor::firstErrorMillis > MOTOR_ERROR_TIMEOUT) {
    GemMotor::motorStatus = false;
  }
}

// Reset status of motor
void GemMotor::ResetMotorStatus() {
  GemMotor::motorStatus      = true;
  GemMotor::motorError       = false;
  GemMotor::firstErrorMillis = millis();
}

// Parses incoming motor messages and convert to motor states
bool GemMotor::ParseCanMsg(struct can_frame _canMsg) {
  GemMotor::canRxStatus = false;
  GemMotor::validStatus = true;

  RECEIVEDMSG();

  switch (_canMsg.can_id - RX_id) {
    case 0:
      ParseCanControl(_canMsg);
      break;
    case 1:
      PraseCanInverterState(_canMsg);
      break;
    case 2:
      ParseCanWarning(_canMsg);
      break;
    case 3:
      ParseCanError(_canMsg);
      break;
    default:
      break;
  }

  return GemMotor::validStatus;
}

// Parse incoming canMsg to Primary control values (Motor RX+0)
void GemMotor::ParseCanControl(struct can_frame _canMsg) {
  GemMotor::controlValueRx = (int)((_canMsg.data[1] << 8) | _canMsg.data[0]);
  GemMotor::motorState     = (int)(_canMsg.data[3] >> 6);
  GemMotor::rpm            = (int)((_canMsg.data[6] << 8) | _canMsg.data[5]) / 10.0f;
  GemMotor::temperature    = (int)_canMsg.data[7];

  // Update new motor state
  MotorStatusUpdate();
}

// Parse incoming canMsg to Motor inverter states (Motor RX+1)
void GemMotor::PraseCanInverterState(struct can_frame _canMsg) {
  GemMotor::inverterPeakCurr = (int)((_canMsg.data[1] << 8) | _canMsg.data[0]);
  GemMotor::power            = (int)((_canMsg.data[3] << 8) | _canMsg.data[2]);
}

// Parse incoming canMsg to Motor Warnings (Motor RX+2)
void GemMotor::ParseCanWarning(struct can_frame _canMsg) {
  for (size_t i = 0; i < 8; i++) {
    for (size_t j = 0; j < 8; j++) {
      GemMotor::warning[i * 8 + j] = _canMsg.data[i] >> j & 1;
    }
  }
}

// Parse incoming canMsg to Motor Errors (Motor RX+3)
void GemMotor::ParseCanError(struct can_frame _canMsg) {
  for (size_t i = 0; i < 8; i++) {
    for (size_t j = 0; j < 8; j++) {
      GemMotor::error[i * 8 + j] = _canMsg.data[i] >> j & 1;
    }
  }
}

// Print latest Motor Control Values
void GemMotor::PrintControl(bool endline) {
  Serial.print(F("Control value: "));
  Serial.print(GemMotor::controlValueRx);
  Serial.print(F("\t Motor State: "));
  Serial.print(GemMotor::motorState);
  Serial.print(F("\t rpm: "));
  Serial.print(GemMotor::rpm);
  Serial.print(F("\t Temperature: "));
  Serial.print(GemMotor::temperature);
  Serial.print(F("\t"));
  if (endline) Serial.println();
}

// Print latest Motor Inverter states
void GemMotor::PrintInverterState(bool endline) {
  Serial.print(F("Max inverter peak current: "));
  Serial.print(GemMotor::inverterPeakCurr);
  Serial.print(F("\t Motor power: "));
  Serial.print(GemMotor::power);
  Serial.print(F(" W"));
  Serial.print(F("\t"));
  if (endline) Serial.println();
}

// Print latest Motor Warnings
void GemMotor::PrintWarning(bool endline) {
  Serial.print(F("Warning: "));
  for (size_t i = 0; i < 64; i++) {
    if (GemMotor::warning[i] == 1) {
      Serial.print(i);
      Serial.print(F("\t"));
    }
  }
  Serial.print(F("\t"));
  if (endline) Serial.println();
}

// Print latest Motor Errors
void GemMotor::PrintError(bool endline) {
  // Serial.print(F("Motor Error "));
  // Serial.print(GemMotor::TX_id, HEX);
  Serial.print(F(" - Error: "));
  for (size_t i = 0; i < 64; i++) {
    if (GemMotor::error[i] == 1) {
      Serial.print(i);
      Serial.print(F("\t"));
    }
  }
  Serial.print(F("\t"));
  if (endline) Serial.println();
}
