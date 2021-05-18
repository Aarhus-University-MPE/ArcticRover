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
#define PI_BUTTON_MODE      2
#define PI_BUTTON_SELECT    3
#define PI_BUTTON_ESTOP     18

// Status LED/Sound
#define PO_LED_STATUS_RED   11
#define PO_LED_STATUS_YEL   11
#define PO_LED_STATUS_GRN   11
#define PO_ALARM_SIGNAL     5

// Power control (Relays)
#define PO_POWER_MOTOR      40
#define PO_POWER_12V        42
#define PO_POWER_5V         44
#define PO_POWER_RF         46
#define PO_POWER_IRIDIUM    48

// Motor Control
#define PP_MOTOR_L          13
#define PP_MOTOR_R          14

// Analog Sensors
#define PA_SENSOR_WIND     PIN_A6
#define PA_SENSOR_TEMP3    PIN_A5
#define PA_SENSOR_TEMP2    PIN_A4
#define PA_SENSOR_TEMP1    PIN_A3
#define PA_SENSOR_RELH3    PIN_A2
#define PA_SENSOR_RELH2    PIN_A1
#define PA_SENSOR_RELH1    PIN_A0

// Communication
#define PO_SPISS_SDCARD     53
#define PO_SPISS_CANBUS     00