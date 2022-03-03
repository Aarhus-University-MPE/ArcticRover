/*
  GeoRover Motor Class - Manages GEM Motor Functionalities.

  Mads Rosenhøj Jepepsen
  Aarhus University
  2022
*/

#include "_motor.h"

#include "Arduino.h"
#include "_constants.h"

// Constructor
GemMotor::GemMotor(int _TX_id, int _RX_id) {
  GemMotor::TX_id = _TX_id;
  GemMotor::RX_id = _RX_id;
  GemMotor::mode = MOTOR_MODE_NORMAL;
  GemMotor::controlMode = CONTROL_MODE_DEFAULT;
  GemMotor::SWEnable = false;

  GemMotor::canMsg.can_id = GemMotor::TX_id;
  GemMotor::canMsg.can_dlc = 3;

  GemMotor::canMsgPtr = &(GemMotor::canMsg);
}

int GemMotor::GetRpm() { return GemMotor::rpm; }

bool GemMotor::GetCANTXStatus() { return GemMotor::CANTXStatus; }

bool GemMotor::GetCANRXStatus() { return GemMotor::CANRXStatus; }

void GemMotor::SetCANTXStatus() {
  GemMotor::CANTXStatus = false;
  GemMotor::CANRXStatus = true;
  GemMotor::CANRXTimeoutStart = millis();
}

GemMotor::ERROR GemMotor::CheckCANRXTimeout() {
  if (!GemMotor::CANRXStatus) {
    return ERROR_IDLE;
  }
  GemMotor::CANRXTimeout = (millis() - GemMotor::CANRXTimeoutStart > CAN_TIMEOUT_DURATION);

  if (GemMotor::CANRXTimeout) {
    GemMotor::CANRXStatus = false;
    return ERROR_TIMEOUT;
  }
  return ERROR_WAITING;
}

void GemMotor::ResetCANStatus() {
  GemMotor::CANTXStatus = false;
  GemMotor::CANRXStatus = false;
}

void GemMotor::Update(int velocity) {
  if (abs(velocity) < MIN_VELOCITY) {
    GemMotor::mode = MOTOR_MODE_NORMAL;
    GemMotor::SWEnable = false;
    GemMotor::controlValue = 0;
    return;
  }

  if (velocity < 0) {
    GemMotor::mode = MOTOR_MODE_REVERSE;
  } else {
    GemMotor::mode = MOTOR_MODE_NORMAL;
  }

  int _rpm = velocity * RPM_VEL_FACTOR * RPM_CONTROL_SCALE;

  GemMotor::controlValue = min(_rpm, MAX_CONTROL_VALUE);
}

struct can_frame* GemMotor::GetCanMsg() {
  // Control values
  GemMotor::canMsg.data[0] = GemMotor::controlValue >> 8 & 0xFF;
  GemMotor::canMsg.data[1] = GemMotor::controlValue & 0xFF;
  ;

  // 0 .. 1 Control Mode
  // 2 .. 4 Motor Mode
  // 5      SW enable
  // 6      reserved
  // 7      debug_mode
  GemMotor::canMsg.data[2] =
      GemMotor::controlMode & B11 | GemMotor::mode >> 2 & B111 | GemMotor::SWEnable >> 5 & B1;
  //

  return GemMotor::canMsgPtr;
}

bool GemMotor::Status() {
  return GemMotor::motorState != 3;
}

bool GemMotor::ParseCanMsg(struct can_frame _canMsg, bool print) {
  switch (_canMsg.can_id - RX_id) {
    case 0:
      ParseCanControl(_canMsg, print);
      GemMotor::CANRXStatus = true;
      break;

    case 1:
      PraseCanInverterState(canMsg, print);
      GemMotor::CANRXStatus = true;
      break;

    case 2:
      ParseCanWarning(canMsg, print);
      GemMotor::CANRXStatus = true;
      break;

    case 3:
      ParseCanError(canMsg, print);
      GemMotor::CANRXStatus = true;
      break;

    default:
      break;
  }

  return GemMotor::CANRXStatus;
}

void GemMotor::ParseCanControl(struct can_frame _canMsg, bool print) {
  GemMotor::controlValueRx = (int)((_canMsg.data[1] << 8) | _canMsg.data[0]);
  GemMotor::motorState = (int)(_canMsg.data[3] >> 6);
  GemMotor::rpm = (int)((_canMsg.data[6] << 8) | _canMsg.data[5]) / 10.0f;
  GemMotor::temperature = (int)_canMsg.data[7];

  if (print) {
    DEBUG_PRINT("Control value: ");
    DEBUG_PRINT(GemMotor::controlValueRx);
    DEBUG_PRINT("\t Motor State: ");
    DEBUG_PRINT(GemMotor::motorState);
    DEBUG_PRINT("\t rpm: ");
    DEBUG_PRINT(GemMotor::rpm);
    DEBUG_PRINT("\t Temperature: ");
    DEBUG_PRINTLN(GemMotor::temperature);
  }
}

void GemMotor::PraseCanInverterState(struct can_frame _canMsg, bool print) {
  GemMotor::inverterPeakCurr = (int)((_canMsg.data[1] << 8) | _canMsg.data[0]);
  GemMotor::power = (int)((_canMsg.data[3] << 8) | _canMsg.data[2]);

  if (print) {
    DEBUG_PRINT("Max inverter peak current: ");
    DEBUG_PRINT(GemMotor::inverterPeakCurr);
    DEBUG_PRINT("\t Motor power: ");
    DEBUG_PRINT(GemMotor::power);
    DEBUG_PRINTLN(" W");
  }
}

void GemMotor::ParseCanWarning(struct can_frame _canMsg, bool print) {
  bool warning[64];
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      warning[i * 8 + j] = _canMsg.data[i] >> j & 1;
    }
  }
  if (print) {
    DEBUG_PRINT("Warning: ");
    for (int i = 0; i < 64; i++) {
      if (warning[i] == 1) {
        DEBUG_PRINT(i);
        DEBUG_PRINT("\t");
      }
    }
    DEBUG_PRINTLN();
  }
}

void GemMotor::ParseCanError(struct can_frame _canMsg, bool print) {
  bool error[64];
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      error[i * 8 + j] = _canMsg.data[i] >> j & 1;
    }
  }

  if (print) {
    DEBUG_PRINT("Error: ");
    for (int i = 0; i < 64; i++) {
      if (error[i] == 1) {
        DEBUG_PRINT(i);
        DEBUG_PRINT("\t");
      }
    }
    DEBUG_PRINTLN();
  }
}
