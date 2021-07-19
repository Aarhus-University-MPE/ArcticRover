// Function Prototypes
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <EEPROM.h>

void MoveBackward(byte powerPercent, unsigned int distanceCm = 0, boolean delayWhileMoving = false);
void MoveForward(byte powerPercent, unsigned int distanceCm = 0, boolean delayWhileMoving = false);
void MoveWheels(byte wheelDirections, byte powerPercentTL, byte powerPercentTR, byte powerPercentBR, byte powerPercentBL);
void TurnLeft(byte powerPercent, unsigned int deltaDegrees = 0, boolean delayWhileMoving = false);
void TurnRight(byte powerPercent, unsigned int deltaDegrees = 0, boolean delayWhileMoving = false);
void InitAllPins();
void InitButtons();
void InitServiceInterrupt();
void EstopButtonInterruptHandler();
void ModeButtonInterruptHandler();
void InitStatusLed();
void InitMode();
void InitStrategyMethods();
void LedBlink(byte color, unsigned int onDuration, unsigned int offDuration);
void LedSet(byte color);
void LedBlinkHalt(byte color, unsigned int duration, unsigned int afterHalt);
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
unsigned int ToByte(bool b[]);

void HeartBeat();
void HeartBeatOut();
void HeartBeatIn();
void HeartBeatInInterrupt();
void ResetBackupCPU();

void appendCharArray(char *s, char c);
void appendCsv(char *s);

void LedBlinkDoubleShort(byte color);
void LedBlinkDoubleShort(byte color1, byte color2);

bool GetStatus();
bool GetStatus(int module);
void SetStatus(bool status);
void SetStatus(int module, bool status);


typedef void (*functionPtr)();

// pointers to key strategy methods.
// [0][MODES_MAX] - start sequence
// [1][MODES_MAX] - main sequence
// [2][MODES_MAX] - end sequence
functionPtr strategyMethods[3][MODES_MAX];

static double DistanceBetween(double lat1, double long1, double lat2, double long2);
static double CourseTo(double lat1, double long1, double lat2, double long2);
static const char Cardinal(double course);

boolean SetMode(byte newMode);
bool SystemStatus[MODULE_COUNT];

bool emergencyStop = false;

void EstopButtonInterruptHandler();
void ModeButtonInterruptHandler();

void InitBluetooth(unsigned long baudRate = 115200);

//current and previous mode (strategy)
unsigned long lastMillisSelect = 0;
byte mode;
byte prevMode;
boolean isModeUpdated = false;

bool GetStatus(int module)
{
  return SystemStatus[module];
}
void SetStatus(int module, bool status)
{
  SystemStatus[module] = status;
}
void SetStatus(bool status)
{
  for (int i = 0; i < MODULE_COUNT; i++)
  {
    SystemStatus[i] = status;
  }
}

void BlackBoxAppend();
void BlackBoxAppend(String blackBoxInput);
void BlackBoxAppend(byte blackBoxInput);
void BlackBoxAppend(bool blackBoxInput);
void BlackBoxAppend(char blackBoxInput);
void BlackBoxAppend(int blackBoxInput);
void BlackBoxAppend(long int blackBoxInput);
void BlackBoxAppend(long int blackBoxInput, int Type);
