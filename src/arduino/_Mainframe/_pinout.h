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
#define PI_BUTTON_SELECT      18
#define PI_BUTTON_ESTOP       2

#define PI_INT_BUTTON_MODE    digitalPinToInterrupt(PI_BUTTON_MODE)
#define PI_INT_BUTTON_SELECT  digitalPinToInterrupt(PI_BUTTON_SELECT)
#define PI_INT_BUTTON_ESTOP   digitalPinToInterrupt(PI_BUTTON_ESTOP)

// Heartbeat
#define PI_BACKUP_HRTBEAT     3
#define PO_BACKUP_HRTBEAT     7
#define PI_INT_HRTBEAT        digitalPinToInterrupt(PI_BACKUP_HRTBEAT)

// Reset pin
#define PI_BACKUP_RST         RESET
#define PO_BACKUP_RST         6

// Status LED/Sound
#define PO_LED_STATUS_RED     32
#define PO_LED_STATUS_YEL     34
#define PO_LED_STATUS_GRN     36

// Power control (Relays)
#define PO_POWER_MOTOR        38
#define PO_POWER_24V          40
#define PO_POWER_12V          42
#define PO_POWER_5V           44
#define PO_POWER_RF           46
#define PO_POWER_IRIDIUM      48
#define PO_POWER_HEATING      50

// Analog Sensors
#define PA_SENSOR_TEMP1       PIN_A0
#define PA_SENSOR_TEMP2       PIN_A1
#define PA_SENSOR_TEMP3       PIN_A2
#define PA_SENSOR_BATT        PIN_A3

// Motor Control
#define PO_MOTOR_EN_LEFT      10
#define PO_MOTOR_EN_RIGHT     11

// Communication
#define PO_SPISS_SDCARD       28        // 50 (MISO), 51 (MOSI), 52 (SCK)
#define PO_SPISS_CANBUS       30        // 50 (MISO), 51 (MOSI), 52 (SCK)

#define COM_SERIAL_DBG        Serial    // USB
#define COM_SERIAL_IRID       Serial2   // 16 (TX), 17 (RX)
#define COM_SERIAL_RF         Serial3   // 15 (RX)

#define COM_SERIAL_GPS        I2C       // 20 (SDA), 21 (SCL)
#define COM_SERIAL_ACC        I2C       // 20 (SDA), 21 (SCL)