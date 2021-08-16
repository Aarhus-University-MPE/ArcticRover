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
  #define DEBUG_PRINT(x) Serial.print(x); BlackBoxAppend(x);
  #define DEBUG_PRINTLN(x) Serial.println(x); BlackBoxAppendln(x);
  #define DEBUG_PRINTLN2(x,y) Serial.println(x,y); BlackBoxAppendln(x,y);
  #define DEBUG_WRITE(x) Serial.write(x)
  #define DEBUG_PRINTLINE() DEBUG_PRINTLN("------------------------------------"); BlackBoxAppendln("------------------------------------");
  #define RECEIVE_CMDS()  recvWithStartEndMarkers()
#else
  #define DBG_ONLY(x)
  #define DEBUG_PRINT(x)  BlackBoxAppend(x)
  #define DEBUG_PRINTLN(x)  BlackBoxAppendln(x)
  #define DEBUG_PRINTLN2(x,y)  BlackBoxAppendln(x,y)
  #define DEBUG_PRINTLINE() BlackBoxAppendln("------------------------------------");
  #define DEBUG_WRITE(x)
  #define RECEIVE_CMDS()  recvWithStartEndMarkers()
#endif

// ------------------------------------------------------------ //
//                           SYSTEM                             //
// ------------------------------------------------------------ //
#define SystemVersion   "0.9.21"

// Binary codes for Status LED flags, Red Yellow Green
#define BINARY_CODE_LED_GRN B001   // 001
#define BINARY_CODE_LED_YEL B010   // 010
#define BINARY_CODE_LED_RED B100   // 100

// LED blink durations (ms)
#define LED_BLINK_VERY_LONG     2000
#define LED_BLINK_LONG          1000
#define LED_BLINK_SHORT         250
#define LED_BLINK_VERY_SHORT    100

// Button debounce
#define BUTTON_DBOUNCE_TIME     200
#define BTN_DEBOUNCE_TIME_LONG  2500

// Sensor and Module status
#define MODULE_COUNT          16

#define MODULE_PWR_MOTOR      0
#define MODULE_PWR_12V        1
#define MODULE_PWR_5V         2
#define MODULE_RF             3
#define MODULE_IRIDIUM        4
#define MODULE_PWR            5
#define MODULE_MOTOR          6
#define MODULE_MOTOR_EN       7
#define MODULE_GNSS           8
#define MODULE_SD             9
#define MODULE_ACCEL          10
#define MODULE_DBGCOMM        11
#define MODULE_BACKUPCPU      12
#define MODULE_ESTOP          13
#define MODULE_BLACKBOX       14
#define MODULE_RESERVED       15



// ------------------------------------------------------------ //
//                       STRATEGY MODES                         //
// ------------------------------------------------------------ //

#define MODES_MAX           6 // Total number of modes
#define MODES_MIN_BROWSABLE 2 // Minimum mode index, that could be set via mode button.

#define MODE_EMERGENCY      0 // Emergency Stop Mode
#define MODE_MODELIBRARY    1 // Mode Library
#define MODE_IDLE           2 // Standby mode
#define MODE_SYSTEMTEST     3 // Test main systems
#define MODE_REMOTECONTROL  4 // System remotely controllable 
#define MODE_AUTONOMOUS     5 // Autonomous driving mode

#define ModeToString(m) ((m) == 0 ? "Emergency" : ((m) == 1 ? "Mode Library" : ((m) == 2 ? "Idle" : ((m) == 3 ? "System Test" : ((m) == 4 ? "Remote Control" : ((m) == 5 ? "Autonomous" : "Unknown"))))))

// ------------------------------------------------------------ //
//                         HEARTBEAT                            //
// ------------------------------------------------------------ //
#define HRTBEAT_FRQ_OUT     1       // times per minute
#define HRTBEAT_DT_OUT      60000 / HRTBEAT_FRQ_OUT

#define HRTBEAT_TRESHOLD    300000  // Treshold for late heartbeat

#define BACKUP_RST_DT       300000  // Time between each reset attempt

#define HRTBEAT_DT_LOG      300000  // Time between system status log

// ------------------------------------------------------------ //
//                          BLACKBOX                            //
// ------------------------------------------------------------ //


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

// Buttons
#define BTN_DEBOUNCE_TIME       500
#define ESTOP_DEBOUNCE_TIME     1000


// DEBUG
#define DEBUG_BAUDRATE          115200


// CAN BUS

#define CANBUS_ID_MOTOR1        0x036
#define CANBUS_ID_MOTOR2        0x037

#define CANBUS_DATA_LENGTH      8

#define CANBBUS_SPEED           CAN_500KBPS

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
//                          COMMANDS                            //
// ------------------------------------------------------------ //
#define CMD_START_MARK          '<'
#define CMD_END_MARK            '>'

#define CMD_FILES               'F'
#define CMD_FILES_LIST          'L'
#define CMD_FILES_SIZE          'S'
#define CMD_FILES_DOWNLOAD      'D'
#define CMD_FILES_DELETE        'R'

#define CMD_STRATEGY            'S'
#define CMD_STRATEGY_SET        'S'
#define CMD_STRATEGY_FUNCTION   'F'
#define CMD_STRATEGY_OVERRIDE   'O'

#define CMD_BACKUP              'B'
#define CMD_BACKUP_RST          'R'
#define CMD_BACKUP_PRIMSTATUS   'S'
#define CMD_BACKUP_HB           'H'

#define CMD_MODULE              'M'
#define CMD_MODULE_ENABLE       'E'
#define CMD_MODULE_DISABLE      'D'
#define CMD_MODULE_STATUS       'S'
#define CMD_MODULE_RESET        'R'

