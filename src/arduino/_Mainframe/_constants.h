/*
  GeoRover system constants

  Mads Rosenhøj Jeppesen
  Aarhus University
  2021
*/

#pragma once

// ------------------------------------------------------------ //
//                            DEBUG                             //
// ------------------------------------------------------------ //

// Debug configuration flag - Comment out to unset
#define DEBUG

// Executes if DEBUG flag is set.
#if defined(DEBUG)
#define DBG_ONLY(x) x
#define DEBUG_PRINT(x) \
  BlackBoxAppend(x);   \
  Serial.print(x)
#define DEBUG_PRINTLN(x) \
  BlackBoxAppendln(x);   \
  Serial.println(x)
#define DEBUG_PRINTLN2(x, y) \
  BlackBoxAppendln(x, y);    \
  Serial.println(x, y)
#define DEBUG_PRINTLINE()                                      \
  BlackBoxAppendln(F("------------------------------------")); \
  Serial.println(F("------------------------------------"))
#define DEBUG_WRITE(x) Serial.write(x)
#define RECEIVE_CMDS() recvWithStartEndMarkers()
#else
#define DBG_ONLY(x)
#define DEBUG_PRINT(x)       BlackBoxAppend(x)
#define DEBUG_PRINTLN(x)     BlackBoxAppendln(x)
#define DEBUG_PRINTLN2(x, y) BlackBoxAppendln(x, y)
#define DEBUG_PRINTLINE()    BlackBoxAppendln(F("------------------------------------"))
#define DEBUG_WRITE(x)
#define RECEIVE_CMDS()
#endif

#define ToBoolString(m)        ((m) == 1 ? "T" : ((m) == 0 ? "F" : "Unknown"))
#define ToAnalogWrite(x)       ((x)*255.0f / 1.0f)

// ------------------------------------------------------------ //
//                           SYSTEM                             //
// ------------------------------------------------------------ //
#define SystemVersion          "0.91.126"

// Binary codes for Status LED flags, Red Yellow Green
#define BINARY_CODE_LED_GRN    B001  // 001
#define BINARY_CODE_LED_YEL    B010  // 010
#define BINARY_CODE_LED_RED    B100  // 100

// LED blink durations (ms)
#define LED_BLINK_VERY_LONG    2500
#define LED_BLINK_LONG         1500
#define LED_BLINK_NORMAL       750
#define LED_BLINK_SHORT        250
#define LED_BLINK_VERY_SHORT   100

// Button debounce
#define BUTTON_DBOUNCE_TIME    200
#define BTN_DEBOUNCE_TIME_LONG 2500

// Relay
#define RELAY_SWITCHING_TIME   10

// Sensor and Module status
#define SYSTEM_CHECK_DT        30000
#define SYSTEM_REBOOT_DT       5000

#define MODULE_COUNT           24

#define MODULE_PWR             0
#define MODULE_PWR_5V          1
#define MODULE_PWR_12V         2
#define MODULE_PWR_24V         3
#define MODULE_PWR_MOTOR       4
#define MODULE_MOTORS          5
#define MODULE_MOTOR_L         6
#define MODULE_MOTOR_R         7
#define MODULE_MOTOR_ACT       8
#define MODULE_CANBUS          9
#define MODULE_RF              10
#define MODULE_IRIDIUM         11
#define MODULE_GNSS            12
#define MODULE_ACCEL           13
#define MODULE_SD              14
#define MODULE_BLACKBOX        15
#define MODULE_ROUTE           16
#define MODULE_DBGCOMM         17
#define MODULE_LED             18
#define MODULE_HEATING         19
#define MODULE_TEMP            20
#define MODULE_BACKUPCPU       21
#define MODULE_ESTOP           22
#define MODULE_RESERVED        23

#define ModuleToString(m) \
  ((m) == 0 ? "Primary Power" : ((m) == 1 ? "Secondary Power (5V)" : ((m) == 2 ? "Secondary Power (12V)" : ((m) == 3 ? "Secondary Power (24V)" : ((m) == 4 ? "Secondary Power (Motors)" : ((m) == 5 ? "Motors" : ((m) == 6 ? "Motor Left" : ((m) == 7 ? "Motor Right" : ((m) == 8 ? "Motor Active" : ((m) == 9 ? "CAN-bus" : ((m) == 10 ? "Short Range Communication (RF)" : ((m) == 11 ? "Long Range Communication (Iridium)" : ((m) == 12 ? "Global Navigation Satellite System" : ((m) == 13 ? "Accelerometer" : ((m) == 14 ? "Local Storage" : ((m) == 15 ? "Blackbox" : ((m) == 16 ? "Waypoint Route" : ((m) == 17 ? "Debug Comm." : ((m) == 18 ? "System LED" : ((m) == 19 ? "Heating" : ((m) == 20 ? "Temperature" : ((m) == 21 ? "Backup CPU" : ((m) == 22 ? "Emergency Stop" : ((m) == 23 ? "Reserved" : ("Unknown")))))))))))))))))))))))))

const unsigned long SYSREQ_REMOTE_CONTROL =
    (1L << MODULE_PWR) +
    (1L << MODULE_PWR_MOTOR) +
    (1L << MODULE_MOTORS) +
    (1L << MODULE_RF) +
    (1L << MODULE_CANBUS) +
    (1L << MODULE_ESTOP);

const unsigned long SYSREQ_AUTONOMOUS =
    (1L << MODULE_PWR) +
    (1L << MODULE_PWR_MOTOR) +
    (1L << MODULE_MOTORS) +
    (1L << MODULE_CANBUS) +
    (1L << MODULE_IRIDIUM) +
    (1L << MODULE_GNSS) +
    (1L << MODULE_ACCEL) +
    (1L << MODULE_SD) +
    (1L << MODULE_BLACKBOX) +
    (1L << MODULE_BACKUPCPU) +
    (1L << MODULE_ESTOP);

// ------------------------------------------------------------ //
//                          COMMANDS                            //
// ------------------------------------------------------------ //
#define CMD_START_MARK         '<'
#define CMD_END_MARK           '>'

#define CMD_FILES              'F'
#define CMD_FILES_LIST         'L'
#define CMD_FILES_SIZE         'S'
#define CMD_FILES_DOWNLOAD     'D'
#define CMD_FILES_DELETE       'R'
#define CMD_FILES_BLCKBOX      'B'
#define CMD_FILES_BLCKBOXCLEAR 'C'

#define CMD_STRATEGY           'S'
#define CMD_STRATEGY_SET       'S'
#define CMD_STRATEGY_FUNCTION  'F'
#define CMD_STRATEGY_OVERRIDE  'O'

#define CMD_BACKUP             'B'
#define CMD_BACKUP_RST         'R'
#define CMD_BACKUP_PRIMSTATUS  'S'
#define CMD_BACKUP_HB          'H'
#define CMD_BACKUP_FREEZE      'F'

#define CMD_MODULE             'M'
#define CMD_MODULE_ENABLE      'E'
#define CMD_MODULE_DISABLE     'D'
#define CMD_MODULE_OVERRIDE    'O'
#define CMD_MODULE_STATUS      'S'
#define CMD_MODULE_RESET       'R'
#define CMD_MODULE_TEST        'T'
#define CMD_MODULE_STOPTEST    'Q'

#define CMD_ROUTE              'R'
#define CMD_ROUTE_SET          'S'
#define CMD_ROUTE_PRINT        'P'
#define CMD_ROUTE_STREAM       'B'

// ------------------------------------------------------------ //
//                       STRATEGY MODES                         //
// ------------------------------------------------------------ //

#define MODES_MAX              6  // Total number of modes
#define MODES_MIN_BROWSABLE    2  // Minimum mode index, that could be set via mode button.

#define MODE_EMERGENCY         0  // Emergency Stop Mode
#define MODE_MODELIBRARY       1  // Mode Library
#define MODE_IDLE              2  // Standby mode
#define MODE_SYSTEMTEST        3  // Test main systems
#define MODE_REMOTECONTROL     4  // System remotely controllable
#define MODE_AUTONOMOUS        5  // Autonomous driving mode

#define ModeToString(m) \
  ((m) == 0 ? "Emergency" : ((m) == 1 ? "Mode Library" : ((m) == 2 ? "Idle" : ((m) == 3 ? "System Test" : ((m) == 4 ? "Remote Control" : ((m) == 5 ? "Autonomous" : "Unknown"))))))

// ------------------------------------------------------------ //
//                         HEARTBEAT                            //
// ------------------------------------------------------------ //
#define HRTBEAT_FRQ_OUT                   12  // times per minute
#define HRTBEAT_DT_OUT                    60000 / HRTBEAT_FRQ_OUT

#define HRTBEAT_TRESHOLD                  60000  // Treshold for late heartbeat

#define BACKUP_RST_DT                     HRTBEAT_TRESHOLD  // Time between each reset attempt

#define HRTBEAT_DT_LOG                    300000  // Time between system status log

// ------------------------------------------------------------ //
//                         NAVIGATION                           //
// ------------------------------------------------------------ //

// Orientation calculations
#define MAX_VALID_BEARING                 250000  // (deg * 10^-5) (2.5 degres)

#define MAX_DISTANCE_VALID_WAYPOINT       1000  // max valid waypoint distance (meters)

#define MAX_DISTANCE_WAYPOINT_ACCEPT      10  // max distance to accept waypoint (meters)

#define EARTH_RADIUS                      6371000  // (meters)

#define GNSS_QUERY_UPDATE_FREQUENCY       1000  // in milliseconds

#define AUTONOMY_PWR_CYCLE_DT             10000
#define NAVIGATION_CYCLE_DT               1000
#define NAVIGATION_RUNCHECK_DT            300000

#define MAX_LAT_VALUE                     900000000   // deg * 10^-7
#define MAX_LON_VALUE                     1800000000  // deg * 10^-7

#define MAX_ROUTE_LEN                     1

#define MIN_ACCEL_TILT                    0.8f  // approx 35°

#define MAX_AUTONOMOUS_SPEED              1.0f
#define AUTONOMY_SPEED_SCALE              0.9f / 30000.0f  // linear scale, max speed after 5 minutes
#define MAX_AUTONOMOUS_TURN               0.5f

#define COORDINATE_DEGREE_SCALE           1.0d / 10000000.0d

// ------------------------------------------------------------ //
//                       REMOTE CONTROL                         //
// ------------------------------------------------------------ //
#define REMOTE_CHANNEL_THROTTLE           0
#define REMOTE_CHANNEL_STEER              1
#define REMOTE_CHANNEL_ENABLE             2

#define REMOTE_PROCESS_DT                 100

#define REMOTE_SIGNAL_SCALE               100.0 / 87.0

#define REMOTE_CHANNEL_LOW                50
#define REMOTE_CHANNEL_HIGH               200
#define CONTROLLER_DEADZONE               10
#define CONTROLLER_DEADZONE_FLOAT         0.05

// ------------------------------------------------------------ //
//                           MOTORS                             //
// ------------------------------------------------------------ //
#define MOTOR_STARTUP_TIMEOUT             5000

// CAN BUS
#define CANBUS_TX_MOTOR_LEFT              0x12
#define CANBUS_RX_MOTOR_LEFT              0x64
#define CANBUS_TX_MOTOR_RIGHT             0x22
#define CANBUS_RX_MOTOR_RIGHT             0x74

// ------------------------------------------------------------ //
//                           HEATING                            //
// ------------------------------------------------------------ //
#define TEMP_SYSTEM_MIN                   10  // ~28 V
#define HEATING_DURATION                  20000
#define HEATING_TIMEOUT                   40000

// ------------------------------------------------------------ //
//                         SYS TESTS                            //
// ------------------------------------------------------------ //
#define SYS_TEST_DURATION                 10000
#define SYS_TEST_DURATION_LONG            30000
#define SYS_PRINT_PERIOD_LONG             500
#define SYS_PRINT_PERIOD                  250
#define SYS_PRINT_PERIOD_SHORT            100

#define MOTOR_RAMP_TIME                   20

// ------------------------------------------------------------ //
//                        COMMUNICATION                         //
// ------------------------------------------------------------ //

// Buttons
#define BTN_DEBOUNCE_TIME                 500
#define ESTOP_DEBOUNCE_TIME               1000

// DEBUG
#define DEBUG_BAUDRATE                    115200

// Iridium
#define IRID_BAUDRATE                     19200
#define IRID_START_TIMEOUT                2       // default value = 240 sec
#define IRID_ATT_TIMEOUT                  2       // default value = 20 sec
#define IRID_PROCESS_PERIOD               900000  // 15 min

// SBUS
#define SBUS_TIMEOUT                      2000

#define CANBUS_DATA_LENGTH                8

#define CANBBUS_SPEED                     CAN_125KBPS
#define COM_TEST_PERIOD                   5000

// ------------------------------------------------------------ //
//                        BATTERY LEVEL                         //
// ------------------------------------------------------------ //
#define BATTERY_MIN_LEVEL                 15  // unused
#define BATTERY_STD_CHARGE                20
#define BATTERY_STD_RECHARGE              80

// ------------------------------------------------------------ //
//                           EEPROM                             //
// ------------------------------------------------------------ //
#define EEPROM_READ_INT(addr)             (int)(((EEPROM.read(addr) << 0) & 0xFF) + ((EEPROM.read(addr + 1) << 8) & 0xFF00))

// #define EEPROM_READ_FLOAT(addr, flt) (float)(((EEPROM.read(addr) << 0) & 0xFF) + ((EEPROM.read(addr+1) << 8) & 0xFF00) + ((EEPROM.read(addr+2) << 16) & 0xFF0000) + ((EEPROM.read(addr+3) << 24) & 0xFF000000))

// Modes
#define MEMADDR_LASTMODE                  0

// Navigation
#define MEMADDR_HOME_START                1
#define MEMADDR_HOME_END                  MEMADDR_HOME_START + 8

#define MEMADDR_ROUTEIDX_START            MEMADDR_HOME_END
#define MEMADDR_ROUTEIDX_END              MEMADDR_ROUTEIDX_START + 2

#define MEMADDR_ROUTELEN_START            MEMADDR_ROUTEIDX_END
#define MEMADDR_ROUTELEN_END              MEMADDR_ROUTELEN_START + 2

#define MEMADDR_WAYPOINTIDX_START         MEMADDR_ROUTELEN_END
#define MEMADDR_WAYPOINTIDX_END           MEMADDR_WAYPOINTIDX_START + 2

#define MEMADDR_ROUTE_START               MEMADDR_WAYPOINTIDX_END

// Read write
#define EEPROM_READ_LAT(index, latLong)   EEPROM.get(MEMADDR_ROUTE_START + index * 8, latLong)
#define EEPROM_READ_LON(index, lonLong)   EEPROM.get(MEMADDR_ROUTE_START + 4 + index * 8, lonLong)

#define EEPROM_WRITE_LAT(index, latLong)  EEPROM.put(MEMADDR_ROUTE_START + index * 8, latLong)
#define EEPROM_WRITE_LON(index, lonLong)  EEPROM.put(MEMADDR_ROUTE_START + 4 + index * 8, lonLong)

#define EEPROM_WRITE_INDEX(waypointIndex) EEPROM.put(MEMADDR_WAYPOINTIDX_START, waypointIndex)

// ------------------------------------------------------------ //
//                          FUNCTIONS                           //
// ------------------------------------------------------------ //
