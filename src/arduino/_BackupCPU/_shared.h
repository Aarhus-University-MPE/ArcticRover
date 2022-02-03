
// Function Prototypes
#include <Arduino.h>
#include <EEPROM.h>
#include <Wire.h>

void InitAllPins();
void InitButtons();
void InitServiceInterrupt();

void recvWithStartEndMarkers();

void initializeDebugComm();

void HeartBeat();
void HeartBeatOut();
void HeartBeatIn();
void HeartBeatInInterrupt();
void ResetBackupCPU();

bool SystemStatus[MODULE_COUNT];

bool GetStatus(int module);
void SetStatus(int module, bool status);

bool GetStatus(int module) {
  return SystemStatus[module];
}
void SetStatus(int module, bool status) {
  SystemStatus[module] = status;
}
