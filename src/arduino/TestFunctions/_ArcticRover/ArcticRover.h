/*
  Arctic Rover Class - Manages Motor pairs

  Uses https://github.com/autowp/arduino-mcp2515 as CAN library

  Mads Rosenhøj Jeppesen
  Aarhus University
  2022
*/

#include <SBUS.h>
#include <SPI.h>
#include <mcp2515.h>

#include "GemMotor.h"
#include "Sys_Control.h"
// #include "_constants.h"

class ArcticRover {
 public:
  enum ERROR {
    ERROR_OK              = 0,
    ERROR_CAN             = 1,
    ERROR_SBUS            = 2,
    ERROR_MOTOR_L_TIMEOUT = 3,
    ERROR_MOTOR_R_TIMEOUT = 4,
    ERROR_MOTOR_R_POWER   = 5,
    ERROR_MOTOR_L_POWER   = 6
  };

  enum MODES {
    MODE_IDLE           = 0,
    MODE_REMOTE_CONTROL = 1,
    MODE_AUTONOMOUS     = 2
  };

  enum MODULE {
    MODULE_PWR           = 0,
    MODULE_PWR_5V        = 1,
    MODULE_PWR_12V       = 2,
    MODULE_PWR_24V       = 3,
    MODULE_PWR_MOTOR     = 4,
    MODULE_MOTORS        = 5,
    MODULE_MOTOR_L       = 6,
    MODULE_MOTOR_R       = 7,
    MODULE_MOTOR_ACTIVE  = 8,
    MODULE_CANBUS        = 9,
    MODULE_RF            = 10,
    MODULE_IRIDIUM       = 11,
    MODULE_GNSS          = 12,
    MODULE_ACCELEROMETER = 13,
    MODULE_SD            = 14,
    MODULE_BLACKBOX      = 15,
    MODULE_DBGCOMM       = 16,
    MODULE_LED           = 17,
    MODULE_HEATING       = 18,
    MODULE_TEMP          = 19,
    MODULE_BACKUPCPU     = 20,
    MODULE_ESTOP         = 21,
    MODULE_RESERVED      = 22
  };

  ArcticRover(const int _TX_ID_Left, const int _RX_ID_Left,
              const int _TX_ID_Right, const int _RX_ID_Right,
              const int _canCsPin, HardwareSerial& _sBusSerial)
              : motorL(GemMotor(_TX_ID_Left, _RX_ID_Left)),
                motorR(GemMotor(_TX_ID_Right, _RX_ID_Right)),
                canModem(MCP2515(_canCsPin)),
                sbus(_sBusSerial),
                sBusSerial(_sBusSerial) {
  }

  ERROR begin();
  ERROR beginMode(MODES);

  void process();
  // Updates controlvalues based on input velocity
  void Update(float velocity, float direction, bool enable);
  void SystemEnable(int module);
  // Prints latest Motor Status
  ERROR PrintStatus();
  // Returns status of motors (Error)
  ERROR Status();

 private:
  GemMotor motorL;
  GemMotor motorR;
  MCP2515 canModem;
  SBUS sbus;
  HardwareSerial& sBusSerial;

  ERROR idle();
  ERROR SbusBegin();
  ERROR CanbusBegin();
  ERROR beginRemoteControl();
  ERROR beginAutonomous();
  ERROR stopMotors();

  struct can_frame canMsg;

  float GetChannelFloat(int channel);
  float GetChannelFloatFull(int channel);
  int GetChannel(int channel);
  int PO_RF_POWER;

  const int minChannel = -83;
  const int maxChannel = 83;
};
