#pragma once

// Function Prototypes
#include <Arduino.h>
#include <EEPROM.h>
#include <SD.h>
#include <SPI.h>
#include <Wire.h>

#include "src/GemMotor/GemMotor.h"

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
void HeartBeatTimeout();
void HeartBeatInInterrupt();
void ResetBackupCPU();

void appendCharArray(char *s, char c);
void appendCsv(char *s);

bool DebugCommStatus();
bool LedBlink(byte color, unsigned int onDuration, unsigned int offDuration);
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

// current and previous mode (strategy)
unsigned long lastMillisSelect      = 0;
unsigned long lastMillisMode        = 0;
unsigned long lastMillisEstop       = 0;
unsigned long lastSystemReboot      = 9999999;
unsigned long lastMillistHeatingOff = 0;
unsigned long lastMillistHeatingOn  = 0;
unsigned long lastSystemCheck       = 9999999;
unsigned long millisAutonomyStart   = 0;

int systemTestState = 0;
byte mode;
byte modeCycle;
byte prevMode;

// Navigation
long latCurrent = 0;
long lonCurrent = 0;

int waypointIndex = 0;

boolean isModeUpdated = false;

bool GetStatus(int module) {
  return SystemStatus[module];
}
void SetStatus(int module, bool status) {
  // Serial.print("Setting module: ");
  // Serial.print(ModuleToString(module));
  // Serial.print("\t");
  // Serial.println(status);
  SystemStatus[module] = status;
}
void SetStatus(bool status) {
  for (int i = 0; i < MODULE_COUNT; i++) {
    SystemStatus[i] = status;
  }
  SystemStatus[MODULE_ESTOP]    = true;
  SystemStatus[MODULE_RESERVED] = true;
}

// Run full system check
void GetStatus(bool printRes);

bool AccelTest(bool printRes);

GemMotor motorLeft  = GemMotor(CANBUS_TX_MOTOR_LEFT, CANBUS_RX_MOTOR_LEFT);
GemMotor motorRight = GemMotor(CANBUS_TX_MOTOR_RIGHT, CANBUS_RX_MOTOR_RIGHT);

void BlackBoxAppendln();
void BlackBoxAppend(String blackBoxInput);
void BlackBoxAppendln(String blackBoxInput);

void BlackBoxAppend(int blackBoxInput);
void BlackBoxAppendln(int blackBoxInput);

void BlackBoxAppend(long int blackBoxInput, int Type);
void BlackBoxAppendln(long int blackBoxInput, int Type);

void BlackBoxAppend(float blackBoxInput);
void BlackBoxAppendln(float blackBoxInput);

void BlackBoxAppend(long int blackBoxInput);
void BlackBoxAppendln(long int blackBoxInput);

void BlackBoxAppend(unsigned long blackBoxInput);
void BlackBoxAppendln(unsigned long blackBoxInput);

double CoordLong2Double(long longValue);

bool DebugInitialize();
void DebugTerminate();

bool SDWriteStream(char fileNameOrData[]);
void SDQuit();

bool remoteActive, remoteStart;

void RemoteControlStatusLed();
void RemoteControlProcess();
void RemoteControlInitialize();

bool SelectButtonDebounce();
bool ModeButtonDebounce();

enum LED_SIGNAL {
  LED_SIGNAL_OK = 0,
  LED_SIGNAL_OK_SHORT,
  LED_SIGNAL_OK_SHORT_HALT,
  LED_SIGNAL_ERROR,
  LED_SIGNAL_ERROR_SHORT,
  LED_SIGNAL_ERROR_SHORT_HALT,
  LED_SIGNAL_IDLE,
  LED_SIGNAL_LOADING
};