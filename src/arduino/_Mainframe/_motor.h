/*
  GeoRover Motor Class - Manages GEM Motor Functionalities.

  Mads Rosenhøj Jepepsen
  Aarhus University
  2022
*/

#include "mcp2515.h"

#define CONTROL_MODE_DEFAULT    0
#define CONTROL_MODE_TORQUE     1
#define CONTROL_MODE_SPEED      2

#define MOTOR_MODE_NORMAL       0
#define MOTOR_MODE_BOOST        1
#define MOTOR_MODE_REVERSE      2
#define MOTOR_MODE_REGENERATION 3

#define SW_ENABLE_IDLE          0
#define SW_ENABLE_RUN           1

#define MIN_VELOCITY            1      // kmh
#define MOTOR_MAX_SPEED_FWD     10.0f  // kmh
#define MOTOR_MAX_SPEED_BWD     5.0f   // kmh

#define MAX_RPM                 200

#define RPM_VEL                 1000.0f / 60.0f
#define WHEEL_CIRCUMFERENCE     1.570f
#define RPM_VEL_FACTOR          RPM_VEL / WHEEL_CIRCUMFERENCE
#define RPM_CONTROL_SCALE       10
#define MAX_CONTROL_VALUE       MAX_RPM* RPM_CONTROL_SCALE
#define CAN_TIMEOUT_DURATION    100

class GemMotor {
 private:
  int controlMode;
  int mode;
  int rpm;
  int TX_id;
  int RX_id;
  int motorState;
  int temperature;
  bool SWEnable;
  int controlValue;
  int controlValueRx;
  int inverterPeakCurr;
  int power;
  bool CANTXStatus;
  bool CANRXStatus;
  long CANRXTimeoutStart;
  bool CANRXTimeout;
  struct can_frame canMsg;
  struct can_frame* canMsgPtr;

 public:
  GemMotor(int _TX_id, int _RX_id);

  enum ERROR {
    ERROR_IDLE = 0,
    ERROR_WAITING = 1,
    ERROR_TIMEOUT = 2
  };

  int GetRpm();
  bool GetCANTXStatus();
  bool GetCANRXStatus();

  // Indicates CAN msg sent.
  // Unset TX flag, Set RX flag indicate awaiting response
  void SetCANRXStatus();

  // Indicates CAN msg ready.
  // Set TX flag, Unset RX flag indicate ready to send msg
  void SetCANTXStatus();

  ERROR CheckCANRXTimeout();

  void ResetCANStatus();

  void Update(int velocity);

  struct can_frame* GetCanMsg();

  bool Status();

  bool ParseCanMsg(struct can_frame _canMsg, bool print);

  // Primary motor parameters
  void ParseCanControl(struct can_frame _canMsg, bool print);

  // Motor power states, inverter peak current and motor power
  void PraseCanInverterState(struct can_frame _canMsg, bool print);

  // Current motor warnings
  void ParseCanWarning(struct can_frame _canMsg, bool print);

  // Current motor errors
  void ParseCanError(struct can_frame _canMsg, bool print);
};
