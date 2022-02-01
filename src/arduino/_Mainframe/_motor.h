#define CONTROL_MODE_DEFAULT    0
#define CONTROL_MODE_TORQUE     1
#define CONTROL_MODE_SPEED      2

#define MOTOR_MODE_NORMAL       0
#define MOTOR_MODE_BOOST        1
#define MOTOR_MODE_REVERSE      2
#define MOTOR_MODE_REGENERATION 3

#define SW_ENABLE_IDLE          0
#define SW_ENABLE_RUN           1

class _motor
{
private:
  int controlMode;
  int mode;
  int rpm;
  int TX_id;
  int RX_id;
  int motorState;
  int temperature;
  bool position;
  bool SWEnable;
  int controlValue;
  int controlValueRx;

public:
  struct can_frame canMsg;

  _motor(bool _position, int _TX_id, int _RX_id)
  {
    position = _position;
    TX_id = _TX_id;
    RX_id = _RX_id;
    mode = MOTOR_MODE_NORMAL;
    controlMode = CONTROL_MODE_DEFAULT;
    SWEnable = false;
    canMsg.can_id = TX_id;
  }

  void Update(bool _SWEnable, int _controlValue, int _mode){
    SWEnable = _SWEnable;
    controlValue = _controlValue;
    mode = _mode;
  }

  void CanMsgUpdate(){
    canMsg.can_dlc = 3;

    // Control values
    canMsg.data[0] = controlValue >> 8 & 0xFF;
    canMsg.data[1] = controlValue & 0xFF;;

    // 0 .. 1 Control Mode
    // 2 .. 4 Motor Mode
    // 5      SW enable
    // 6      reserved
    // 7      debug_mode
    canMsg.data[2] = \
    controlMode         & B11   |\
    mode          >> 2  & B111  |\
    SWEnable      >> 5  & B1;
    // 
  }

  void ParseCanMsg(struct can_frame _canMsg, bool print){
    switch (_canMsg.can_id - RX_id)
    {
    // control value, motor state, rpm, temp, etc.
    case 0:
      controlValueRx = (int)((canMsg.data[1] << 8) | canMsg.data[0]);
      motorState = (int)(canMsg.data[3] >> 6); 
      rpm = (int)((canMsg.data[6] << 8) | canMsg.data[5]) / 10.0f;
      temperature = (int)canMsg.data[7];

      if(print){
        DEBUG_PRINT("Control value: ");
        DEBUG_PRINT(controlValueRx);
        DEBUG_PRINT("\t Motor State: ");
        DEBUG_PRINT(motorState);
        DEBUG_PRINT("\t rpm: ");
        DEBUG_PRINT(rpm);
        DEBUG_PRINT("\t Temperature: ");
        DEBUG_PRINTLN(temperature);
      }
      break;
    
    // inverter peak current, motor power, etc.
    case 1:

      break;

    // Warnings
    case 2:
      bool warning[64];
      for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
          warning[i*8+j] = canMsg.data[i] >> j & 1;
        }
      }
      if(print){
        DEBUG_PRINT("Warning: ");
        for (int i = 0; i < 64; i++) {
          if(warning[i] == 1){
            DEBUG_PRINT(i);
            DEBUG_PRINT("\t");
          }
        }
        DEBUG_PRINTLN();
      }
      
      break;

    // Errors
    case 3:
      bool error[64];
    
      for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
          error[i*8+j] = canMsg.data[i] >> j & 1;
        }
      }

      if(print){
        DEBUG_PRINT("Error: ");
        for (int i = 0; i < 64; i++) {
          if(error[i] == 1){
            DEBUG_PRINT(i);
            DEBUG_PRINT("\t");
          }
        }
        DEBUG_PRINTLN(); 
      }
      break;

    default:
      break;
    }
  }

  void Enable(){
    SWEnable = true;
    SystemEnable(MODULE_MOTORS);
  }

  void Disable(){
    SWEnable = false;
    SystemDisable(MODULE_MOTORS);
  }
};

