/*
  GeoRover system constants

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021
*/

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

#define ToBoolString(m) ((m) == 1 ? "T" : ((m) == 0 ? "F" : "Unknown"))

#define ToAnalogWrite(x)    ((x) * 255.0f/1.0f)
// ------------------------------------------------------------ //
//                           SYSTEM                             //
// ------------------------------------------------------------ //
#define SystemVersion   "0.28.116"

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

// Relay
#define RELAY_SWITCHING_TIME  10

// Sensor and Module status
#define SYSTEM_CHECK_DT       1000
#define SYSTEM_REBOOT_DT      5000

#define MODULE_COUNT          17

#define MODULE_PWR            0
#define MODULE_PWR_5V         1
#define MODULE_PWR_12V        2
#define MODULE_PWR_MOTOR      3
#define MODULE_MOTORS         4
#define MODULE_MOTOR_ACT      5
#define MODULE_CANBUS         6
#define MODULE_RF             7
#define MODULE_IRIDIUM        8
#define MODULE_GNSS           9
#define MODULE_ACCEL          10
#define MODULE_SD             11
#define MODULE_BLACKBOX       12
#define MODULE_DBGCOMM        13
#define MODULE_BACKUPCPU      14
#define MODULE_ESTOP          15
#define MODULE_RESERVED       16
#define MODULE_LED            17

#define ModuleToString(m) \
((m) == 0 ? "Primary Power" : \
((m) == 1 ? "Secondary Power (5V)" : \
((m) == 2 ? "Secondary Power (12V)" : \
((m) == 3 ? "Secondary Power (Motors)" : \
((m) == 4 ? "Motors" : \
((m) == 5 ? "Motor Active" : \
((m) == 6 ? "CAN-bus" : \
((m) == 7 ? "Short Range Communication (RF)" : \
((m) == 8 ? "Long Range Communication (Iridium)" : \
((m) == 9 ? "Global Navigation Satellite System" : \
((m) == 10 ? "Accelerometer" : \
((m) == 11 ? "Local Storage" : \
((m) == 12 ? "Blackbox" : \
((m) == 13 ? "Debug Comm." : \
((m) == 14 ? "Backup CPU" : \
((m) == 15 ? "Emergency Stop" : \
((m) == 16 ? "Reserved" : \
"Unknown")))))))))))))))))

const unsigned long SYSREQ_REMOTE_CONTROL = \
(1L << MODULE_PWR)             + \
(1L << MODULE_PWR_MOTOR)       + \
(1L << MODULE_MOTORS)          + \
(1L << MODULE_RF)              + \
(1L << MODULE_ESTOP);

const unsigned long SYSREQ_AUTONOMOUS =  \
(1L << MODULE_PWR)           + \
(1L << MODULE_PWR_MOTOR)     + \
(1L << MODULE_MOTORS)        + \
(1L << MODULE_IRIDIUM)       + \
(1L << MODULE_GNSS)          + \
(1L << MODULE_ACCEL)         + \
(1L << MODULE_SD)            + \
(1L << MODULE_BLACKBOX)      + \
(1L << MODULE_BACKUPCPU)     + \
(1L << MODULE_ESTOP);


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

#define ModeToString(m) \
((m) == 0 ? "Emergency" : \
((m) == 1 ? "Mode Library" : \
((m) == 2 ? "Idle" : \
((m) == 3 ? "System Test" : \
((m) == 4 ? "Remote Control" : \
((m) == 5 ? "Autonomous" : \
"Unknown"))))))

// ------------------------------------------------------------ //
//                         HEARTBEAT                            //
// ------------------------------------------------------------ //
#define HRTBEAT_FRQ_OUT     1       // times per minute
#define HRTBEAT_DT_OUT      60000 / HRTBEAT_FRQ_OUT

#define HRTBEAT_TRESHOLD    300000  // Treshold for late heartbeat

#define BACKUP_RST_DT       300000  // Time between each reset attempt

#define HRTBEAT_DT_LOG      300000  // Time between system status log


// ------------------------------------------------------------ //
//                         NAVIGATION                           //
// ------------------------------------------------------------ //

// Orientation calculations
#define MIN_DISTANCE_VALID_BEARING      1           // min valid distance to calculate bearing (meters)
#define MAX_DISTANCE_VALID_WAYPOINT     1000        // max valid waypoint distance (meters)
#define MIN_DISTANCE_WAYPOINT_ACCEPT    5           // max distance in to accept waypoint (meters)

#define EARTH_RADIUS                    6371000     // (meters)

#define GNSS_QUERY_UPDATE_FREQUENCY     1000        // in milliseconds

#define MAX_LAT_VALUE                   900000000   // deg * 10^-7
#define MAX_LONG_VALUE                  1800000000  // deg * 10^-7


// ------------------------------------------------------------ //
//                       REMOTE CONTROL                         //
// ------------------------------------------------------------ //
#define REMOTE_CHANNEL_THROTTLE   0
#define REMOTE_CHANNEL_STEER      1
#define REMOTE_CHANNEL_ENABLE     2

#define REMOTE_PROCESS_DT         250

#define REMOTE_SIGNAL_SCALE       100.0/87.0


// ------------------------------------------------------------ //
//                           MOTORS                             //
// ------------------------------------------------------------ //


#define MOTOR_MAX_SPEED_FWD             1.0f
#define MOTOR_MAX_SPEED_BWD             1.0f
#define MOTOR_MIN_SPEED                 0.05f
#define MOTOR_LEFT                      true
#define MOTOR_RIGHT                     false

#define MOTOR_RAMP_TIME                 20



// ------------------------------------------------------------ //
//                        COMMUNICATION                         //
// ------------------------------------------------------------ //

// Buttons
#define BTN_DEBOUNCE_TIME       500
#define ESTOP_DEBOUNCE_TIME     1000


// DEBUG
#define DEBUG_BAUDRATE          115200

// Iridium
#define IRID_BAUDRATE           19200
#define IRID_START_TIMEOUT      2     // default value = 240 sec
#define IRID_ATT_TIMEOUT        2     // default value = 20 sec

// SBUS
#define SBUS_RX_PERIOD          300

// CAN BUS
#define CANBUS_TX_MOTOR_LEFT    0x12
#define CANBUS_RX_MOTOR_LEFT    0x64
#define CANBUS_TX_MOTOR_RIGHT   0x22
#define CANBUS_RX_MOTOR_RIGHT   0x74

#define CANBUS_DATA_LENGTH      8

#define CANBBUS_SPEED           CAN_125KBPS
#define COM_TEST_PERIOD         5000
#define CANBUS_TX_PERIOD        100

// ------------------------------------------------------------ //
//                           EEPROM                             //
// ------------------------------------------------------------ //
#define EEPROM_READ_INT(addr)   (int)(((EEPROM.read(addr) << 0) & 0xFF) + ((EEPROM.read(addr+1) << 8) & 0xFF00))

// #define EEPROM_READ_FLOAT(addr, flt) (float)(((EEPROM.read(addr) << 0) & 0xFF) + ((EEPROM.read(addr+1) << 8) & 0xFF00) + ((EEPROM.read(addr+2) << 16) & 0xFF0000) + ((EEPROM.read(addr+3) << 24) & 0xFF000000))

// Modes
#define MEMADDR_LASTMODE 0

// Navigation
#define MEMADDR_HOME_START        1
#define MEMADDR_HOME_END          MEMADDR_HOME_START + 8

#define MEMADDR_ROUTELEN_START    MEMADDR_HOME_END
#define MEMADDR_ROUTELEN_END      MEMADDR_ROUTELEN_START + 2   

#define MEMADDR_ROUTE_START       MEMADDR_ROUTELEN_END

// Read write
#define EEPROM_READ_LAT(index, latInt)    EEPROM.get(MEMADDR_ROUTE_START + index * 8, latInt)
#define EEPROM_READ_LON(index, lonInt)    EEPROM.get(MEMADDR_ROUTE_START + 4 + index * 8, lonInt)

#define EEPROM_WRITE_LAT(index, latInt)   EEPROM.put(MEMADDR_ROUTE_START + index * 8, latInt)
#define EEPROM_WRITE_LON(index, lonInt)   EEPROM.put(MEMADDR_ROUTE_START + 4 + index * 8, lonInt)

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
#define CMD_MODULE_OVERRIDE     'O'
#define CMD_MODULE_STATUS       'S'
#define CMD_MODULE_RESET        'R'
#define CMD_MODULE_TEST         'T'

#define CMD_ROUTE               'R'
#define CMD_ROUTE_SET           'S'
#define CMD_ROUTE_PRINT         'P'


