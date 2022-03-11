/*
  Arctic Rover Class - Manages primary functions
  - Motors
  - Canbus
  - sbus (Remote Control)

  Uses https://github.com/autowp/arduino-mcp2515

  Mads Rosenhøj Jepepsen
  Aarhus University
  2022
*/
#include "ArcticRover.h"

#include "Arduino.h"

ArcticRover::ERROR ArcticRover::begin() {
  ERROR err;

  err = ArcticRover::SbusBegin();
  if (err != ERROR_OK) return err;

  err = ArcticRover::CanbusBegin();
  if (err != ERROR_OK) return err;

  return ERROR_OK;
}

ArcticRover::ERROR ArcticRover::SbusBegin() {
  SPI.begin();

  digitalWrite(ArcticRover::PO_RF_POWER, HIGH);

  ArcticRover::sbus.begin(false);
  if (!ArcticRover::sBusSerial) return ERROR_SBUS;

  return ERROR_OK;
}

ArcticRover::ERROR ArcticRover::CanbusBegin() {
  MCP2515::ERROR err;

  ArcticRover::canModem.reset();
  err = ArcticRover::canModem.setBitrate(CAN_125KBPS, MCP_8MHZ);
  if (err != MCP2515::ERROR_OK) return ERROR_CAN;

  err = ArcticRover::canModem.setNormalMode();
  if (err != MCP2515::ERROR_OK) return ERROR_CAN;

  return ERROR_OK;
}

ArcticRover::ERROR ArcticRover::idle() {
  // stop motors
  // stop canbus
  // disable sbus

  return ERROR_OK;
}

ArcticRover::ERROR ArcticRover::stopMotors(){
  // Cut Power
  // SystemDisable(MODULE_PWR_MOTOR);

  // Cut HW Enable
}

ArcticRover::ERROR ArcticRover::beginMode(MODES mode) {
  ERROR err;
  switch (mode) {
    case MODE_IDLE:
      err = idle();
      break;
    case MODE_REMOTE_CONTROL:
      err = beginRemoteControl();
      break;
    case MODE_AUTONOMOUS:
      err = beginAutonomous();
      break;
    default:
      break;
  }

  if (err != ERROR_OK) idle();

  return err;
}

ArcticRover::ERROR ArcticRover::beginRemoteControl() {
  if (SbusBegin() != ERROR_OK) return ERROR_SBUS;
  if (CanbusBegin() != ERROR_OK) return ERROR_CAN;

  return ERROR_OK;
}

ArcticRover::ERROR ArcticRover::beginAutonomous() {
  if (CanbusBegin() != ERROR_OK) return ERROR_CAN;

  return ERROR_OK;
}

// Scale SBUS channel value from range [0, 256] to [-1, 1]
float ArcticRover::GetChannelFloat(int channel) {
  int value = GetChannel(channel);

  float valueFloat = value / 128.0 - 1;

  return valueFloat;
}

// Scale SBUS channel value from range [0, 256] to [-1, 1]
float ArcticRover::GetChannelFloatFull(int channel) {
  int value = GetChannel(channel);

  float valueFloat = value / 256.0;

  return valueFloat;
}

// Scale the S.BUS channel values into the range [0, 256]
int ArcticRover::GetChannel(int channel) {
  int value = sbus.getNormalizedChannel(channel);
  if (value < minChannel) {
    return 256 / 2;
  }

  float result = value;

  result -= minChannel;
  result /= (maxChannel - minChannel);
  result *= 256;

  return (int)result;
}

