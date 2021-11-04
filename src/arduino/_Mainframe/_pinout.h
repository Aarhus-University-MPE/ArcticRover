/*
  GeoRover system pinout

  Naming pattern (all uppercase):
  P   + <I:input, A:analog input, O:output, P:pwm output>_
    + <general part name>_
    + <purpose>_
    + <where applicable: left, right, top, bottom, position etc...>

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021
*/

// External Inputs
#define PI_BUTTON_MODE        19
#define PI_BUTTON_SELECT      18   // digitalPinToInterrupt(18)
#define PI_BUTTON_ESTOP       2  // digitalPinToInterrupt(20)

#define PI_INT_BUTTON_MODE      digitalPinToInterrupt(PI_BUTTON_MODE)
#define PI_INT_BUTTON_SELECT    digitalPinToInterrupt(PI_BUTTON_SELECT)
#define PI_INT_BUTTON_ESTOP     digitalPinToInterrupt(PI_BUTTON_ESTOP)

// Heartbeat
#define PI_BACKUP_HRTBEAT     3
#define PO_BACKUP_HRTBEAT     9
#define PI_INT_HRTBEAT        digitalPinToInterrupt(PI_BACKUP_HRTBEAT)

// Reset pin
#define PI_BACKUP_RST       RESET
#define PO_BACKUP_RST       8

// Status LED/Sound
#define PO_LED_STATUS_RED   32
#define PO_LED_STATUS_YEL   34
#define PO_LED_STATUS_GRN   36

// Power control (Relays)
#define PO_POWER_MOTOR_ON     38
#define PO_POWER_MOTOR_OFF    40
#define PO_POWER_12V          42
#define PO_POWER_5V           44
#define PO_POWER_RF           46
#define PO_POWER_IRIDIUM      48

// Analog Sensors
#define PA_SENSOR_RELH1    PIN_A0
#define PA_SENSOR_RELH2    PIN_A1
#define PA_SENSOR_TEMP1    PIN_A2
#define PA_SENSOR_TEMP2    PIN_A3
#define PA_SENSOR_TEMP3    PIN_A4

// Motor Control
#define PP_MOTOR_THRTL_LEFT   8
#define PP_MOTOR_THRTL_RIGHT  9
#define PO_MOTOR_EN_LEFT      10
#define PO_MOTOR_EN_RIGHT     11

// Communication
#define PO_SPISS_SDCARD     28
#define PO_SPISS_CANBUS     30

#define COM_SERIAL_DBG      Serial
#define COM_SERIAL_IRID     Serial2
#define COM_SERIAL_RF       Serial3