// Function Prototypes
#include <Arduino.h>
#include <EEPROM.h>
#include <SD.h>
#include <SPI.h>
#include <Wire.h>

#include "_motor.h"

void (*systemReset)(void) = 0;

void SystemEnablePrimary();
bool MotorState();
void InitAllPins();
void InitButtons();
void InitServiceInterrupt();
void EstopButtonInterruptHandler();
void ModeButtonInterruptHandler();
void InitStatusLed();
void InitMode();
void InitStrategyMethods();
void LedSet(byte color);
void recvWithStartEndMarkers();
bool SDReaderStatus();
void SDQuery();
void SDSize(char fileName[]);
void SDDownload(char fileName[]);
void SDDelete(char fileName[]);
void SDCreate(char fileName[]);
void printFiles(File dir);
void initializeDebugComm();
bool InitializeIridium();
void ModeUpdater();
unsigned long ToLong(bool b[]);
bool AccelStatus();
void CountDownPrint();
bool SystemCheck(int mode);
void SystemEnableMode(int mode);

bool IridiumStatus();
bool GnssStatus();
void TerminateAccel();

bool InitializeSBUS();
void TerminateSBUS();
bool SBusStatus();

bool VoltageCheck();
bool BatteryStatus();

bool MotorState();
bool MotorStatus();
void ParseCanMsg(bool motor);

void HeartBeat();
void HeartBeatOut();
void HeartBeatIn();
void HeartBeatInInterrupt();
void ResetBackupCPU();

void appendCharArray(char *s, char c);
void appendCsv(char *s);

bool DebugCommStatus();
void LedBlink(byte color, unsigned int onDuration, unsigned int offDuration);
void LedBlinkHalt(byte color, unsigned int duration, unsigned int afterHalt);
void LedBlinkSingleShort(byte color);
void LedBlinkDoubleShort(byte color);
void LedBlinkDoubleShort(byte color1, byte color2);
void LedBlinkTripleShort(byte color);
void LedBlinkTripleShort(byte color1, byte color2, byte color3);

typedef void (*functionPtr)();

// pointers to key strategy methods.
// [0][MODES_MAX] - start sequence
// [1][MODES_MAX] - main sequence
// [2][MODES_MAX] - end sequence
// [2][MODES_MAX] - Select button function
functionPtr strategyMethods[4][MODES_MAX];

static double DistanceBetween(double lat1, double long1, double lat2, double long2);
static double CourseTo(double lat1, double long1, double lat2, double long2);
static const char Cardinal(double course);

boolean SetMode(byte newMode);
bool SystemStatus[MODULE_COUNT];

bool HeartBeatStatus();
bool BlackBoxStatus();

void EstopButtonInterruptHandler();
void ModeButtonInterruptHandler();

void InitBluetooth(unsigned long baudRate = 115200);

//current and previous mode (strategy)
unsigned long lastMillisSelect = 0;
unsigned long lastMillisMode = 0;
unsigned long lastMillisEstop = 0;
unsigned long lastMillistModeBlink = 0;
unsigned long lastSystemReboot = 9999999;
unsigned long lastMillistHeatingOff = 0;
unsigned long lastMillistHeatingOn = 0;

byte mode;
byte modeCycle;
byte prevMode;

boolean isModeUpdated = false;

bool GetStatus(int module) {
  return SystemStatus[module];
}
void SetStatus(int module, bool status) {
  SystemStatus[module] = status;
}
void SetStatus(bool status) {
  for (int i = 0; i < MODULE_COUNT; i++) {
    SystemStatus[i] = status;
  }
  SystemStatus[MODULE_ESTOP] = true;
  SystemStatus[MODULE_RESERVED] = true;
}

// Run full system check
void GetStatus(bool printRes);

bool AccelTest(bool printRes);

void BlackBoxAppendln();
void BlackBoxAppendln(String blackBoxInput);
void BlackBoxAppendln(byte blackBoxInput);
void BlackBoxAppendln(bool blackBoxInput);
void BlackBoxAppendln(char blackBoxInput);
void BlackBoxAppendln(int blackBoxInput);
void BlackBoxAppendln(long int blackBoxInput);
void BlackBoxAppendln(unsigned long blackBoxInput);
void BlackBoxAppendln(long int blackBoxInput, int Type);

void BlackBoxAppend(String blackBoxInput);
void BlackBoxAppend(byte blackBoxInput);
void BlackBoxAppend(bool blackBoxInput);
void BlackBoxAppend(char blackBoxInput);
void BlackBoxAppend(int blackBoxInput);
void BlackBoxAppend(long int blackBoxInput);
void BlackBoxAppend(unsigned long blackBoxInput);
void BlackBoxAppend(long int blackBoxInput, int Type);

_motor motorLeft = _motor(CANBUS_TX_MOTOR_LEFT, CANBUS_RX_MOTOR_LEFT);
_motor motorRight = _motor(CANBUS_TX_MOTOR_RIGHT, CANBUS_RX_MOTOR_RIGHT);