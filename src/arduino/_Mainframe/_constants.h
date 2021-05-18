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
    #define DBG_ONLY(x) x
    #define DEBUG_PRINT(x) Serial.print(x)
    #define DEBUG_PRINTLN(x) Serial.println(x)    
#else
    #define DBG_ONLY(x) 
    #define DEBUG_PRINT(x)
    #define DEBUG_PRINTLN(x)
#endif

// ------------------------------------------------------------ //
//                           SYSTEM                             //
// ------------------------------------------------------------ //

// Strategy Modes
#define MODES_MAX           5 // Total number of modes
#define MODES_MIN_BROWSABLE 1 // Minimum mode index, that could be set via mode button.

#define MODE_EMERGENCY      0 // Emergency Stop Mode
#define MODE_SYSTEMTEST     1 // Test main systems
#define MODE_IDLE           2 // Standby mode
#define MODE_REMOTECONTROL  3 // System remotely controllable 
#define MODE_AUTONOMOUS     4 // Autonomous driving mode

// Binary codes for Status LED flags, Red Yellow Green 
#define BINARY_CODE_LED_GRN B001   // 001
#define BINARY_CODE_LED_YEL B010   // 010
#define BINARY_CODE_LED_RED B100   // 100

// LED blink durations (ms)
#define LED_BLINK_VERY_LONG     2000
#define LED_BLINK_LONG          1000
#define LED_BLINK_SHORT         250
#define LED_BLINK_VERY_SHORT    100

#define BUTTON_TIMEOUT_DURATION 300



// ------------------------------------------------------------ //
//                         NAVIGATION                           //
// ------------------------------------------------------------ //

// Orientation calculations
#define MIN_DISTANCE_VALID_BEARING      1           // min valid distance to calculate bearing (meters)
#define MAX_DISTANCE_VALID_WAYPOINT     1000        // max valid waypoint distance (meters)
#define MIN_DISTANCE_WAYPOINT_ACCEPT    5           // max distance in to accept waypoint (meters)

#define EARTH_RADIUS                    6371000     // (meters)

#define BINARY_CODE_PRECHECK_REQUIRED   B11111100
#define BINARY_CODE_PRECHECK_OPTIONAL   B00000011
#define BINARY_CODE_PRECHECK_FULL       B11111111

#define GNSS_QUERY_UPDATE_FREQUENCY     1000        // in milliseconds

// ------------------------------------------------------------ //
//                           MOTORS                             //
// ------------------------------------------------------------ //

#define MOTOR_MAX_SPEED_FWD             10
#define MOTOR_MAX_SPEED_BWD             5


// ------------------------------------------------------------ //
//                        COMMUNICATION                         //
// ------------------------------------------------------------ //

// CAN BUS

#define CANBUS_ID_MOTOR1        0x036
#define CANBUS_ID_MOTOR2        0x037

#define CANBUS_DATA_LENGTH      8


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






// ------------------------------------------------------------ //
//                             OLD                              //
// ------------------------------------------------------------ //
// Bluetooth?
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
