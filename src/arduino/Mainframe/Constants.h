/*
GeoRover system constants

Mads Rosenhøj Jepepsen
Aarhus University 
2021
*/

// #include <_shared.h>

// ------------------------------------------------------------ //
//                            DEBUG                             //
// ------------------------------------------------------------ //

// Debug configuration flag - Comment out to unset
#define DEBUG

// Executes one line of code only if DEBUG flag is set.
#if defined(DEBUG)
    #define DBG_ONLY(x) (x)
    #define DEBUG_PRINT(x) Serial.print(x)
    #define DEBUG_PRINTLN(x) Serial.println(x)    
#else
    #define DBG_ONLY(x) 
    #define DEBUG_PRINT(x)
    #define DEBUG_PRINTLN(x)
#endif

// ------------------------------------------------------------ //
//                             PINS                             //
// ------------------------------------------------------------ //

// External Inputs
#define inputBtn1           1
#define inputBtn2           2
#define emergencyStopBtn    3

// Status LED/Sound
#define ledStatus1  3
#define ledStatus2  4
#define alarm1      5

// Power control (Relays)
#define pwrMotor    40
#define pwr12VMain  42
#define pwr5VMain   44
#define pwrRF       46
#define pwrIridium  48

// Motor Control
#define motorPWMLeft    13
#define motorPWMRight   14

// Analog Sensors
#define sensorWind  PIN_A6
#define sensorTemp3 PIN_A5
#define sensorTemp2 PIN_A4
#define sensorTemp1 PIN_A3
#define sensorRelH3 PIN_A2
#define sensorRelH2 PIN_A1
#define sensorRelH1 PIN_A0

// Communication
#define SD_CS 53


// ------------------------------------------------------------ //
//                           SYSTEM                             //
// ------------------------------------------------------------ //

// Strategy Modes
#define MODES_MAX           5 // Total number of modes
#define MODES_MIN_BROWSABLE 1 // Minimum mode index, that could be set via mode button.

#define MODE_EMERGENCY      0 // Emergency Mode
#define MODE_SYSTEMTEST     1 // Test main systems
#define MODE_IDLE           2 // Standby mode
#define MODE_REMOTECONTROL  3 // System remotely controllable 
#define MODE_AUTONOMOUS     4 // 


#define BT_NAME "Rover5"
#define BT_CODE 7463
#define BT_COM_BUFFER_SIZE 10
#define BT_START_DELIMITER "<!!"
#define BT_END_DELIMITER "!!>"
#define BT_PING_INTERVAL 500 //milliseconds
#define BT_KEEP_TTL 10 //sets how long the commands are kept in buffer since they were sent

//indicates correct forward direction flag for each shaft.
#define MOTOR_FWD_T 0
#define MOTOR_FWD_B 1

#define SERVICE_TIMER_INTERVAL  20//timer tick interval in MILLIseconds
#define MOTOR_TIMER_INTERVAL  1000//timer tick interval in MICROseconds
#define MOTOR_CALIBRATION_TICK 200 // timer tick number to run motor calibration 

//binary codes for flags - TopLeft, TopRight, BottomRight, BottomLeft 
#define BINARY_CODE_TL 1
#define BINARY_CODE_TR 2
#define BINARY_CODE_BR 4
#define BINARY_CODE_BL 8

//head servo constants
#define HEAD_PAN_CENTER 86
#define HEAD_TILT_CENTER 100
#define HEAD_ABS_PAN_MAX 178
#define HEAD_ABS_PAN_MIN 0
#define HEAD_ABS_TILT_MAX 140
#define HEAD_ABS_TILT_MIN 15
#define HEAD_REL_PAN_MAX HEAD_PAN_CENTER - HEAD_ABS_PAN_MIN
#define HEAD_REL_PAN_MIN HEAD_PAN_CENTER - HEAD_ABS_PAN_MAX
#define HEAD_REL_TILT_MAX HEAD_ABS_TILT_MAX - HEAD_TILT_CENTER
#define HEAD_REL_TILT_MIN HEAD_ABS_TILT_MAX - HEAD_TILT_CENTER

//freeimu signature number !important - this constant is also in Freeimu.cpp
#define FREEIMU_EEPROM_SIGNATURE 0x19


// ------------------------------------------------------------ //
//                           EEPROM                             //
// ------------------------------------------------------------ //
#define EEPROM_READ_INT(addr) (int)(((EEPROM.read(addr) << 0) & 0xFF) + ((EEPROM.read(addr+1) << 8) & 0xFF00))

// Modes
#define MEMADDR_LASTMODE 0
// motor calibration cache

#define MEMADDR_MOTORCACHE_START 1 
#define MEMADDR_MOTORCACHE_END MEMADDR_MOTORCACHE_START + 160

// analog IR cache
#define MEMADDR_IRCACHE_START MEMADDR_MOTORCACHE_END
#define MEMADDR_IRCACHE_END MEMADDR_IRCACHE_START + 32

// compass and accelerometer cache
#define MEMADDR_FREEIMU_START MEMADDR_IRCACHE_END //!important - this constant is also in Freeimu.cpp
#define MEMADDR_FREEIMU_END MEMADDR_FREEIMU_START + 36 + 1 + 3  //36 bytes for values , 1 for signature, 3 empty space

