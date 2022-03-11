/*
  GeoRover Motor Class - Manages GEM Motor Functionalities.

  Mads Rosenhøj Jepepsen
  Aarhus University
  2022
*/

#include "GemMotor.h"

#include "Arduino.h"

// Constructor
GemMotor::GemMotor(int _TX_id, int _RX_id) {
  GemMotor::TX_id    = _TX_id;
  GemMotor::RX_id    = _RX_id;
  GemMotor::swEnable = SW_DISABLE;

  GemMotor::canMsg.can_id  = GemMotor::TX_id;
  GemMotor::canMsg.can_dlc = 3;
}

int GemMotor::GetRpm() { return GemMotor::rpm; }

bool GemMotor::GetCanTxStatus() { return GemMotor::canTxStatus; }

bool GemMotor::GetCanRxStatus() { return GemMotor::canRxStatus; }

void GemMotor::SetCanTxStatus() {
  GemMotor::canTxStatus = true;
  GemMotor::canRxStatus = false;
}

void GemMotor::SetCanRxStatus() {
  GemMotor::canTxStatus       = false;
  GemMotor::canRxStatus       = true;
  GemMotor::canRxTimeoutStart = millis();
}

GemMotor::CAN_ERROR GemMotor::CheckCanRxTimeout() {
  if (!GemMotor::canRxStatus) {
    return GemMotor::CAN_ERROR_IDLE;
  }
  GemMotor::canRxTimeout = (millis() - GemMotor::canRxTimeoutStart > CAN_TIMEOUT_DURATION);

  if (GemMotor::canRxTimeout) {
    GemMotor::canRxStatus = false;
    return GemMotor::CAN_ERROR_TIMEOUT;
  }
  return GemMotor::CAN_ERROR_WAITING;
}

void GemMotor::ResetCanStatus() {
  GemMotor::canTxStatus = false;
  GemMotor::canRxStatus = false;
}

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

  // Clamp velocity to (max speed bwd < velocity < max speed fwd)
  velocity = max(MOTOR_MAX_SPEED_BWD, min(MOTOR_MAX_SPEED_FWD, velocity));

  GemMotor::rpmTarget = abs(velocity) * RPM_VEL_FACTOR * RPM_CONTROL_SCALE;

  GemMotor::controlValue = min(GemMotor::rpmTarget, MAX_CONTROL_VALUE) * dir;
}

GemMotor::ERROR GemMotor::PrintStatus() {
  if (!validStatus) {
    Serial.print(F("Motor Status "));
    Serial.print(GemMotor::TX_id);
    Serial.println(F(": Invalid"));
    return ERROR_INVALID_STATUS;
  }

  GemMotor::PrintControl(true);
  // GemMotor::PrintInverterState(true);
  GemMotor::PrintError(false);
  GemMotor::PrintWarning(true);

  return ERROR_OK;
}

struct can_frame* GemMotor::GetCanMsg() {
  GemMotor::BuildCanMsg();

  GemMotor::SetCanRxStatus();

  return GemMotor::canMsgPtr;
}

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

bool GemMotor::Status() {
  MotorStateUpdate();
  return GemMotor::motorStatus && GemMotor::validStatus;
}

bool GemMotor::ParseCanMsg(struct can_frame _canMsg) {
  GemMotor::canRxStatus = true;
  validStatus           = true;

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
      GemMotor::canRxStatus = false;
      validStatus           = false;
      break;
  }

  return GemMotor::canRxStatus;
}

void GemMotor::MotorStateUpdate(){
  // Check latest motor state for error
  if(GemMotor::motorState != MOTOR_ERROR){
    GemMotor::motorStatus = true;
    return;
  }

  // Check if error previously not reported - Set timestamp for first error
  if(!GemMotor::motorError){
    GemMotor::motorError = true;
    GemMotor::firstErrorMillis = millis();
    return;
  } 

  // Check time passed since first error, if above timeout set motor state as error.
  if(millis() - GemMotor::firstErrorMillis > MOTOR_ERROR_TIMEOUT){
    GemMotor::motorStatus = false;
  }
}

void GemMotor::ParseCanControl(struct can_frame _canMsg) {
  GemMotor::controlValueRx = (int)((_canMsg.data[1] << 8) | _canMsg.data[0]);
  GemMotor::motorState     = (byte)(_canMsg.data[3] >> 6);
  GemMotor::rpm            = (int)((_canMsg.data[6] << 8) | _canMsg.data[5]) / 10.0f;
  GemMotor::temperature    = (int)_canMsg.data[7];
  
  // Update new motor state
  MotorStateUpdate();
}

void GemMotor::PraseCanInverterState(struct can_frame _canMsg) {
  GemMotor::inverterPeakCurr = (int)((_canMsg.data[1] << 8) | _canMsg.data[0]);
  GemMotor::power            = (int)((_canMsg.data[3] << 8) | _canMsg.data[2]);
}

void GemMotor::ParseCanWarning(struct can_frame _canMsg) {
  for (size_t i = 0; i < 8; i++) {
    for (size_t j = 0; j < 8; j++) {
      GemMotor::warning[i * 8 + j] = _canMsg.data[i] >> j & 1;
    }
  }
}

void GemMotor::ParseCanError(struct can_frame _canMsg) {
  for (size_t i = 0; i < 8; i++) {
    for (size_t j = 0; j < 8; j++) {
      GemMotor::error[i * 8 + j] = _canMsg.data[i] >> j & 1;
    }
  }
}

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

void GemMotor::PrintInverterState(bool endline) {
  Serial.print(F("Max inverter peak current: "));
  Serial.print(GemMotor::inverterPeakCurr);
  Serial.print(F("\t Motor power: "));
  Serial.print(GemMotor::power);
  Serial.print(F(" W"));
  Serial.print(F("\t"));
  if (endline) Serial.println();
}

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

void GemMotor::PrintError(bool endline) {
  Serial.print(F("Error: "));
  for (size_t i = 0; i < 64; i++) {
    if (GemMotor::error[i] == 1) {
      Serial.print(i);
      Serial.print(F("\t"));
    }
  }
  Serial.print(F("\t"));
  if (endline) Serial.println();
}