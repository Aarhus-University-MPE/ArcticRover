/*
  GeoRover Motor Class - Manages GEM Motor Functionalities.

  Uses https://github.com/autowp/arduino-mcp2515

  Mads Rosenhøj Jepepsen
  Aarhus University
  2022
*/

#include "mcp2515.h"

#define MIN_VELOCITY         1      // kmh
#define MOTOR_MAX_SPEED_FWD  10.0f  // kmh
#define MOTOR_MAX_SPEED_BWD  -5.0f  // kmh

#define MAX_RPM              200

#define RPM_VEL              1000.0f / 60.0f
#define WHEEL_CIRCUMFERENCE  1.570f
#define RPM_VEL_FACTOR       RPM_VEL / WHEEL_CIRCUMFERENCE
#define RPM_CONTROL_SCALE    10
#define MAX_CONTROL_VALUE    MAX_RPM* RPM_CONTROL_SCALE
#define CAN_TIMEOUT_DURATION 100
#define CANBUS_TX_PERIOD     10
#define MOTOR_ERROR_TIMEOUT  5000

class GemMotor {
  enum CONTROL_MODE {
    CONTROL_DEFAULT = 0,
    CONTROL_TORQUE  = 1,
    CONTROL_SPEED   = 2
  };

  enum MOTOR_MODE {
    MOTOR_NORMAL       = 0,
    MOTOR_BOOST        = 1,
    MOTOR_REVERSE      = 2,
    MOTOR_REGENERATION = 3
  };

  enum SW_ENABLE {
    SW_DISABLE = 0,
    SW_ENABLE  = 1
  };

 public:
  enum CAN_ERROR {
    CAN_ERROR_IDLE    = 0,
    CAN_ERROR_WAITING = 1,
    CAN_ERROR_TIMEOUT = 2
  };

  enum ERROR {
    ERROR_OK             = 0,
    ERROR_INVALID_STATUS = 1,
    ERROR_MOTOR_ERROR    = 2
  };

  enum MOTOR_ERROR {
    MOTOR_ERROR_SETTINGS_MISSING             = 1,
    MOTOR_ERROR_MOTOR_STALL                  = 2,
    MOTOR_ERROR_CONTROLLER_DATA_TIMEOUT      = 3,
    MOTOR_ERROR_INVALID_HALL_SENSOR_SEQUENCE = 4,
    MOTOR_ERROR_INVALID_HALL_SECTOR          = 5,
    MOTOR_ERROR_ERROR_READING_TEMP           = 6,
    MOTOR_ERROR_POSITION_SENSOR_READING      = 7,
    MOTOR_ERROR_READING_ENCODER              = 8,
    MOTOR_ERROR_ZERO_POSITION_OFFSET_NOTSET  = 9,
    MOTOR_ERROR_HW_ENABLE_NOTSET             = 10,
    MOTOR_ERROR_READING_INVERTER_TEMP_1      = 11,
    MOTOR_ERROR_READING_INVERTER_TEMP_2      = 12,
    MOTOR_ERROR_READING_INVERTER_TEMP_3      = 13,
    MOTOR_ERROR_READING_INVERTER_TEMP_4      = 14,
    MOTOR_ERROR_READING_INVERTER_TEMP_5      = 15,
    MOTOR_ERROR_READING_INVERTER_TEMP_6      = 16,
    MOTOR_ERROR_READING_CPU_TEMP             = 17,
    MOTOR_ERROR_READING_HALL_TEMP            = 18,
    MOTOR_ERROR_READING_DCLINK_TEMP          = 19,
    MOTOR_ERROR_DCLINK_COMMUNICATION         = 20,
    MOTOR_ERROR_INVERTER_1_OVERCURRENT       = 21,
    MOTOR_ERROR_INVERTER_2_OVERCURRENT       = 22,
    MOTOR_ERROR_INVERTER_3_OVERCURRENT       = 23,
    MOTOR_ERROR_INVERTER_4_OVERCURRENT       = 24,
    MOTOR_ERROR_INVERTER_5_OVERCURRENT       = 25,
    MOTOR_ERROR_INVERTER_6_OVERCURRENT       = 26,
    MOTOR_ERROR_DC_OVERVOLTAGE               = 27,
    MOTOR_ERROR_DC_UNDERVOLTAGE              = 28,
    MOTOR_ERROR_DOULBE_CANID_ON_BUS          = 29,
    MOTOR_ERROR_CAN_COMMUNICATION_TIMEOUT    = 30,
    MOTOR_ERROR_INVERTER_1_FAULT             = 31,
    MOTOR_ERROR_INVERTER_2_FAULT             = 32,
    MOTOR_ERROR_INVERTER_3_FAULT             = 33,
    MOTOR_ERROR_INVERTER_4_FAULT             = 34,
    MOTOR_ERROR_INVERTER_5_FAULT             = 35,
    MOTOR_ERROR_INVERTER_6_FAULT             = 36,
    MOTOR_ERROR_CAN_SEND_ERROR               = 37,
    MOTOR_ERROR_LOST_FRAMES_ON_BUS           = 38,
    MOTOR_ERROR_OVERSPEED                    = 39,
    MOTOR_ERROR_CPU_OVERLOAD                 = 40
  };

  enum MOTOR_STATE {
    MOTOR_INIT  = 0,
    MOTOR_IDLE  = 1,
    MOTOR_RUN   = 2,
    MOTOR_ERROR = 3
  };

  enum MOTOR_WARNING {
    MOTOR_WARNING_ABOUT_TO_STALL             = 2,
    MOTOR_WARNING_DELAY_TEMP_SENSOR          = 6,
    MOTOR_WARNING_DELAY_POS_SENSOR           = 7,
    MOTOR_WARNING_DELAY_INVERTER_TEMP_1      = 11,
    MOTOR_WARNING_DELAY_INVERTER_TEMP_2      = 12,
    MOTOR_WARNING_DELAY_INVERTER_TEMP_3      = 13,
    MOTOR_WARNING_DELAY_INVERTER_TEMP_4      = 14,
    MOTOR_WARNING_DELAY_INVERTER_TEMP_5      = 15,
    MOTOR_WARNING_DELAY_INVERTER_TEMP_6      = 16,
    MOTOR_WARNING_DELAY_CPU_TEMP             = 17,
    MOTOR_WARNING_DELAY_HALL_TEMP            = 18,
    MOTOR_WARNING_DELAY_DCLINK_TEMP          = 19,
    MOTOR_WARNING_DELAY_DCLINK_COMMUNICATION = 20,
    MOTOR_WARNING_INVERTER_1_OVERCURRENT     = 21,
    MOTOR_WARNING_INVERTER_2_OVERCURRENT     = 22,
    MOTOR_WARNING_INVERTER_3_OVERCURRENT     = 23,
    MOTOR_WARNING_INVERTER_4_OVERCURRENT     = 24,
    MOTOR_WARNING_INVERTER_5_OVERCURRENT     = 25,
    MOTOR_WARNING_INVERTER_6_OVERCURRENT     = 26,
    MOTOR_WARNING_DC_OVERVOLTAGE             = 27,
    MOTOR_WARNING_DC_UNDERVOLTAGE            = 28,
    MOTOR_WARNING_DOULBE_CANID_ON_BUS        = 29,
    MOTOR_WARNING_CAN_COMMUNICATION_TIMEOUT  = 30,
    MOTOR_WARNING_INVERTER_1_FAULT           = 31,
    MOTOR_WARNING_INVERTER_2_FAULT           = 32,
    MOTOR_WARNING_INVERTER_3_FAULT           = 33,
    MOTOR_WARNING_INVERTER_4_FAULT           = 34,
    MOTOR_WARNING_INVERTER_5_FAULT           = 35,
    MOTOR_WARNING_INVERTER_6_FAULT           = 36,
    MOTOR_WARNING_CAN_SEND_WARNING           = 37,
    MOTOR_WARNING_LOST_FRAMES_ON_BUS         = 38,
    MOTOR_WARNING_OVERSPEED                  = 39,
    MOTOR_WARNING_CPU_OVERLOAD               = 40,
    MOTOR_WARNING_TORQUE_LIMITED             = 41,
    MOTOR_WARNING_STARTING_HIGH_RPM          = 42
  };

 private:
  int controlMode = CONTROL_SPEED;
  int mode        = MOTOR_NORMAL;
  int rpm;
  int rpmTarget;
  int TX_id, RX_id;
  byte motorState;
  int temperature;
  int controlValue;
  int controlValueRx;
  int inverterPeakCurr;
  int power;
  bool validStatus;
  bool swEnable;
  bool canTxStatus;
  bool canRxStatus;
  unsigned long canRxTimeoutStart;
  unsigned long firstErrorMillis;
  bool canRxTimeout;
  bool motorStatus = true;
  bool motorError;
  bool warning[64];
  bool error[64];
  struct can_frame canMsg;
  struct can_frame* canMsgPtr = &canMsg;

  // Constructs control CAN msg
  void BuildCanMsg();
  // Primary motor parameters
  void ParseCanControl(struct can_frame _canMsg);
  // Motor power states, inverter peak current and motor power
  void PraseCanInverterState(struct can_frame _canMsg);
  // Current motor warnings
  void ParseCanWarning(struct can_frame _canMsg);
  // Current motor errors
  void ParseCanError(struct can_frame _canMsg);

  void MotorStateUpdate();

  void PrintControl(bool endline);
  void PrintInverterState(bool endline);
  void PrintWarning(bool endline);
  void PrintError(bool endline);

 public:
  GemMotor(int _TX_id, int _RX_id);

  int GetRpm();
  bool GetCanTxStatus();
  bool GetCanRxStatus();

  // Indicates CAN msg sent.
  // Unset TX flag, Set RX flag indicate awaiting response
  void SetCanRxStatus();

  // Indicates CAN msg ready.
  // Set TX flag, Unset RX flag indicate ready to send msg
  void SetCanTxStatus();

  // Checks time passed since transmitted msg if > timeout duration, times out
  CAN_ERROR CheckCanRxTimeout();

  // Unsets TX and RX flag
  void ResetCanStatus();

  // Updates controlvalues based on input velocity
  void Update(float velocity);

  // Prints latest Motor Status
  ERROR PrintStatus();

  // Returns CanMsg ptr and sets RxStatus (indicate awaiting response)
  struct can_frame* GetCanMsg();

  // Returns status of motor (Error)
  bool Status();

  bool ParseCanMsg(struct can_frame _canMsg);
};
