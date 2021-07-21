// Function Prototypes
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <EEPROM.h>

void(* systemReset) (void) = 0;

bool MotorState();
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
bool AccelStatus();
void CountDownPrint();

bool InitializeIridium();
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

void HeartBeat();
void HeartBeatOut();
void HeartBeatIn();
void HeartBeatInInterrupt();
void ResetBackupCPU();

void appendCharArray(char *s, char c);
void appendCsv(char *s);

bool DebugCommStatus();
void LedBlinkDoubleShort(byte color);
void LedBlinkDoubleShort(byte color1, byte color2);
void LedBlinkTripleShort(byte color);
void LedBlinkTripleShort(byte color1, byte color2, byte color3);

typedef void (*functionPtr)();

// pointers to key strategy methods.
// [0][MODES_MAX] - start sequence
// [1][MODES_MAX] - main sequence
// [2][MODES_MAX] - end sequence
functionPtr strategyMethods[4][MODES_MAX];

static double DistanceBetween(double lat1, double long1, double lat2, double long2);
static double CourseTo(double lat1, double long1, double lat2, double long2);
static const char Cardinal(double course);

boolean SetMode(byte newMode);
bool SystemStatus[MODULE_COUNT];

bool emergencyStop = false;
bool HeartBeatStatus();
bool BlackBoxStatus();

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


// Run full system check
void GetStatus(){
  SetStatus(MODULE_PWR_MOTOR, digitalRead(PO_POWER_MOTOR));
  SetStatus(MODULE_PWR_12V,   digitalRead(PO_POWER_12V));
  SetStatus(MODULE_PWR_5V,    digitalRead(PO_POWER_5V));
  SetStatus(MODULE_RF,       (digitalRead(PO_POWER_RF)      &&  digitalRead(PO_POWER_5V)   && SBusStatus()));
  SetStatus(MODULE_IRIDIUM,  (digitalRead(PO_POWER_IRIDIUM) &&  digitalRead(PO_POWER_5V)   && IridiumStatus()));
  SetStatus(MODULE_PWR,       BatteryStatus());
  SetStatus(MODULE_MOTOR,    (digitalRead(PO_POWER_MOTOR)   &&  MotorStatus()));
  SetStatus(MODULE_MOTOR_EN, (digitalRead(PO_POWER_MOTOR)   &&  MotorState()));
  SetStatus(MODULE_GNSS,     (digitalRead(PO_POWER_5V)      &&  GnssStatus()));
  SetStatus(MODULE_SD,       (digitalRead(PO_POWER_5V)      &&  SDReaderStatus()));
  SetStatus(MODULE_ACCEL,    (digitalRead(PO_POWER_5V)      &&  AccelStatus()));
  SetStatus(MODULE_DBGCOMM,   DebugCommStatus());
  SetStatus(MODULE_BACKUPCPU, HeartBeatStatus());
  SetStatus(MODULE_ESTOP,     emergencyStop);
  SetStatus(MODULE_BLACKBOX,  BlackBoxStatus());
  SetStatus(MODULE_RESERVED,  true);
}



void BlackBoxAppendln();
void BlackBoxAppendln(String blackBoxInput);
void BlackBoxAppendln(byte blackBoxInput);
void BlackBoxAppendln(bool blackBoxInput);
void BlackBoxAppendln(char blackBoxInput);
void BlackBoxAppendln(int blackBoxInput);
void BlackBoxAppendln(long int blackBoxInput);
void BlackBoxAppendln(long int blackBoxInput, int Type);



void BlackBoxAppend(String blackBoxInput);
void BlackBoxAppend(byte blackBoxInput);
void BlackBoxAppend(bool blackBoxInput);
void BlackBoxAppend(char blackBoxInput);
void BlackBoxAppend(int blackBoxInput);
void BlackBoxAppend(long int blackBoxInput);
void BlackBoxAppend(long int blackBoxInput, int Type);