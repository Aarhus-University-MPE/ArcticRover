/*
  GeoRover Backup CPU system constants

  Mads Rosenhøj Jeppesen
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
#define DBG_ONLY(x)          x
#define DEBUG_PRINT(x)       Serial.print(x)
#define DEBUG_PRINTLN(x)     Serial.println(x)
#define DEBUG_PRINTLN2(x, y) Serial.println(x, y)
#define DEBUG_WRITE(x)       Serial.write(x)
#else
#define DBG_ONLY(x)
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#define DEBUG_PRINTLN2(x, y)
#define DEBUG_WRITE(x)
#endif

// ------------------------------------------------------------ //
//                         HEARTBEAT                            //
// ------------------------------------------------------------ //
#define HRTBEAT_FRQ_OUT        12  // times per minute
#define HRTBEAT_DT_OUT         60000 / HRTBEAT_FRQ_OUT

#define HRTBEAT_TRESHOLD       30000

#define BACKUP_RST_DT          HRTBEAT_TRESHOLD

// ------------------------------------------------------------ //
//                        COMMUNICATION                         //
// ------------------------------------------------------------ //

// Buttons
#define BTN_DEBOUNCE_TIME      1000

// DEBUG
#define DEBUG_BAUDRATE         115200

// Sensor and Modules
#define MODULE_COUNT           5

#define MODULE_DBGCOMM         0
#define MODULE_PRIMARYCPU      1

// ------------------------------------------------------------ //
//                          COMMANDS                            //
// ------------------------------------------------------------ //
#define CMD_START_MARK         '<'
#define CMD_END_MARK           '>'
#define CMD_SPLIT_MARK         ','

#define CMD_BACKUP             'B'
#define CMD_BACKUP_RST         'R'
#define CMD_BACKUP_BCKUPSTATUS 'S'
#define CMD_BACKUP_HB          'H'
#define CMD_BACKUP_FREEZE      'F'