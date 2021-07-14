// Function Prototypes
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <EEPROM.h>

void MoveBackward(byte powerPercent, unsigned int distanceCm=0, boolean delayWhileMoving=false);
void MoveForward(byte powerPercent, unsigned int distanceCm=0, boolean delayWhileMoving=false);
void MoveWheels(byte wheelDirections, byte powerPercentTL, byte powerPercentTR, byte powerPercentBR, byte powerPercentBL);
void TurnLeft(byte powerPercent, unsigned int deltaDegrees=0, boolean delayWhileMoving=false);
void TurnRight(byte powerPercent, unsigned int deltaDegrees=0, boolean delayWhileMoving=false);
void InitAllPins();
void InitButtons();
void InitServiceInterrupt();
void InitStatusLed();
void InitMode();
void InitStrategyMethods();
void LedBlink(byte color, unsigned int onDuration, unsigned int offDuration);
void LedSet(byte color);
void LedBlinkHalt(byte color, unsigned int duration, unsigned int afterHalt);
void RecvWithEndMarker();

void LedBlinkDoubleShort(byte color);
void LedBlinkDoubleShort(byte color1, byte color2);

// bool GetStatus(int module){
//     return SystemStatus[module];
// } 
// void SetStatus(int module, bool status) {
//     SystemStatus[module] = status;
// }

static double DistanceBetween(double lat1, double long1, double lat2, double long2);
static double CourseTo(double lat1, double long1, double lat2, double long2);
static const char Cardinal(double course);

boolean SetMode(byte newMode);
void InitIRSensor();

float rawCompassVals[11];
bool SystemStatus[8];
bool isGnssValid = false;
bool isMotorEnabled = false;
bool isSystemStable = true;
bool emergencyStop = false;

void ModeButtonInterruptHandler();

void InitBluetooth(unsigned long baudRate=115200);


unsigned long lastMillisSelect = 0;


//current and previous mode (strategy)
byte mode; 
byte prevMode;
boolean isModeUpdated = false;