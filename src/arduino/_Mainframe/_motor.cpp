#include "_motor.h"

#include "Arduino.h"
#include "_constants.h"

// Constructor
_motor::_motor(int _TX_id, int _RX_id) {
  _motor::TX_id = _TX_id;
  _motor::RX_id = _RX_id;
  _motor::mode = MOTOR_MODE_NORMAL;
  _motor::controlMode = CONTROL_MODE_DEFAULT;
  _motor::SWEnable = false;

  _motor::canMsg.can_id = _motor::TX_id;
  _motor::canMsg.can_dlc = 3;

  _motor::canMsgPtr = &(_motor::canMsg);
}

// Primary motor parameters
void _motor::ParseCanControl(struct can_frame _canMsg, bool print) {
  _motor::controlValueRx = (int)((_canMsg.data[1] << 8) | _canMsg.data[0]);
  _motor::motorState = (int)(_canMsg.data[3] >> 6);
  _motor::rpm = (int)((_canMsg.data[6] << 8) | _canMsg.data[5]) / 10.0f;
  _motor::temperature = (int)_canMsg.data[7];

  if (print) {
    DEBUG_PRINT("Control value: ");
    DEBUG_PRINT(_motor::controlValueRx);
    DEBUG_PRINT("\t Motor State: ");
    DEBUG_PRINT(_motor::motorState);
    DEBUG_PRINT("\t rpm: ");
    DEBUG_PRINT(_motor::rpm);
    DEBUG_PRINT("\t Temperature: ");
    DEBUG_PRINTLN(_motor::temperature);
  }
}

// Motor power states, inverter peak current and motor power
void _motor::PraseCanInverterState(struct can_frame _canMsg, bool print) {
  _motor::inverterPeakCurr = (int)((_canMsg.data[1] << 8) | _canMsg.data[0]);
  _motor::power = (int)((_canMsg.data[3] << 8) | _canMsg.data[2]);

  if (print) {
    DEBUG_PRINT("Max inverter peak current: ");
    DEBUG_PRINT(_motor::inverterPeakCurr);
    DEBUG_PRINT("\t Motor power: ");
    DEBUG_PRINT(_motor::power);
    DEBUG_PRINTLN(" W");
  }
}

// Current motor warnings
void _motor::ParseCanWarning(struct can_frame _canMsg, bool print) {
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

// Current motor errors
void _motor::ParseCanError(struct can_frame _canMsg, bool print) {
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

int _motor::GetRpm() { return _motor::rpm; }
bool _motor::GetCANTXStatus() { return _motor::CANTXStatus; }
bool _motor::GetCANRXStatus() { return _motor::CANRXStatus; }

// Indicates CAN msg sent.
// Unset TX flag, Set RX flag indicate awaiting response
void _motor::setCANTXStatus() {
  _motor::CANTXStatus = false;
  _motor::CANRXStatus = true;
  _motor::CANRXTimeoutStart = millis();
}

void _motor::CheckCANRXTimeout() {
  if (!_motor::CANRXStatus) {
    return;
  }
  _motor::CANRXTimeout = (millis() - _motor::CANRXTimeoutStart > CAN_TIMEOUT);

  if (_motor::CANRXTimeout) {
    _motor::CANRXStatus = false;
  }
}

void _motor::ResetCANStatus() {
  _motor::CANTXStatus = false;
  _motor::CANRXStatus = false;
}

void _motor::Update(int velocity) {
  if (abs(velocity) < MIN_VELOCITY) {
    _motor::mode = MOTOR_MODE_NORMAL;
    _motor::SWEnable = false;
    _motor::controlValue = 0;
    return;
  }

  if (velocity < 0) {
    _motor::mode = MOTOR_MODE_REVERSE;
  } else {
    _motor::mode = MOTOR_MODE_NORMAL;
  }

  int _rpm = velocity * RPM_VEL_FACTOR * RPM_CONTROL_SCALE;

  _motor::controlValue = min(_rpm, MAX_CONTROL_VALUE);
}

struct can_frame* _motor::GetCanMsg() {
  // Control values
  _motor::canMsg.data[0] = _motor::controlValue >> 8 & 0xFF;
  _motor::canMsg.data[1] = _motor::controlValue & 0xFF;
  ;

  // 0 .. 1 Control Mode
  // 2 .. 4 Motor Mode
  // 5      SW enable
  // 6      reserved
  // 7      debug_mode
  _motor::canMsg.data[2] =
      _motor::controlMode & B11 | _motor::mode >> 2 & B111 | _motor::SWEnable >> 5 & B1;
  //

  return _motor::canMsgPtr;
}

bool _motor::ParseCanMsg(struct can_frame _canMsg, bool print) {
  switch (_canMsg.can_id - RX_id) {
    case 0:
      ParseCanControl(_canMsg, print);
      _motor::CANRXStatus = true;
      break;

    case 1:
      PraseCanInverterState(canMsg, print);
      _motor::CANRXStatus = true;
      break;

    case 2:
      ParseCanWarning(canMsg, print);
      _motor::CANRXStatus = true;
      break;

    case 3:
      ParseCanError(canMsg, print);
      _motor::CANRXStatus = true;
      break;

    default:
      break;
  }

  return _motor::CANRXStatus;
}
