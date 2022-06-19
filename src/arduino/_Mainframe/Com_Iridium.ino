/*
  GeoRover Iridium communication protocols for long range communication
  - Library Dependency: https://github.com/sparkfun/SparkFun_IridiumSBD_I2C_Arduino_Library

  Mads Rosenhøj Jeppesen
  Aarhus University
  2021
*/

#include <IridiumSBD.h>
#define DIAGNOSTICS false  // Change this to see diagnostics

IridiumSBD modem(COM_SERIAL_IRID);

uint8_t sendBuffer[200];
uint8_t receiveBuffer[200];
const byte numCharsIridium = 32;
size_t sendBuferSize;
size_t bufferSize = 200;
int signalQuality = -1;

unsigned long millisLastIridiumProcess = 0;
unsigned long millisLastIridiumSend    = 0;

bool InitializeIridium() {
  bool status = true;
  COM_SERIAL_IRID.begin(IRID_BAUDRATE);

  modem.adjustATTimeout(IRID_ATT_TIMEOUT);
  modem.adjustStartupTimeout(IRID_START_TIMEOUT);
  digitalWrite(PO_POWER_IRIDIUM, HIGH);

  // modem.setPowerProfile(IridiumSBD::USB_POWER_PROFILE);

  status = (modem.begin() == ISBD_SUCCESS);
  return status;
}

void TerminateIridium() {
  digitalWrite(PO_POWER_IRIDIUM, LOW);
  COM_SERIAL_IRID.end();
}

bool IridiumStatus() {
  return modem.isConnected();
}

// Primary Iridium Process
void IridiumProcess() {
  if (!GetStatus(MODULE_IRIDIUM)) return;

  if (signalQuality < 1) return;

  if ((millis() - millisLastIridiumProcess < IRID_PROCESS_PERIOD)) return;

  millisLastIridiumProcess = millis();

  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Starting Iridium Transmit Process, halting motors"));
  DEBUG_PRINTLINE();

  // Sets motor speed to 0 and halts - TODO: alternatively power off motors
  MotorCycle();

  IridiumReceive();
  DEBUG_PRINTLINE();
  IridiumSend();

  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Iridium Transmit Process Complete"));
  DEBUG_PRINTLINE();

  // TODO: Potentialy reenable motors
}

// Check modem for waiting messages
// TODO: add retry receive msg if error
void IridiumReceive() {
  DEBUG_PRINTLN(F("Checking Messages"));

  if (!modem.getWaitingMessageCount() > 0) {
    DEBUG_PRINTLN(F("No New messages"));
    return;
  }
  DEBUG_PRINTLN(F("New message available"));

  int err;

  err = modem.sendReceiveSBDText(NULL, receiveBuffer, bufferSize);
  if (err != ISBD_SUCCESS) {
    DEBUG_PRINT(F("Receive msg failed, Error: "));
    DEBUG_PRINTLN(err);
  } else {
    // Parse iridium message and act upon
    parseIridium();
  }
  // Clear the Mobile Originated message buffer to avoid re-sending the message during subsequent loops
  DEBUG_PRINTLN(F("Clearing the MO buffer."));
  err = modem.clearBuffers(ISBD_CLEAR_MO);  // Clear MO buffer
  if (err != ISBD_SUCCESS) {
    DEBUG_PRINT(F("clearBuffers failed, Error: "));
    DEBUG_PRINTLN(err);
  }
}

// Send iridium message awaiting to be sent
// TODO: add retry send msg if error
void IridiumSend() {
  if (millis() - millisLastIridiumSend < IRID_BROADCAST_PERIOD) return;

  millisLastIridiumSend = millis();

  DEBUG_PRINTLN(F("Populating Iridium Message"));
  PopulateSendBuffer();
  DEBUG_PRINT(F("Sending Iridium Broadcast Message"));

  int err = modem.sendSBDBinary(sendBuffer, sendBuferSize);
  if (err == ISBD_SUCCESS) {
    DEBUG_PRINTLN("Success!");
  } else {
    DEBUG_PRINT("Error: ");
    DEBUG_PRINTLN(err);
  }
}

// Decode iridium message and parse data
void parseIridium() {
  DEBUG_PRINT(F("Received command (Iridium): \""));
  DEBUG_PRINT((char *)receiveBuffer);
  DEBUG_PRINTLN(F("\""));

  switch (receiveBuffer[0]) {
    case CMD_FILES:
      parseCommandFilesIridium();
      break;
    case CMD_STRATEGY:
      parseCommandStrategyIridium();
      break;
    case CMD_BACKUP:
      parseCommandBackupIridium();
      break;
    case CMD_MODULE:
      parseCommandModuleIridium();
      break;
    case CMD_ROUTE:
      parseCommandRouteIridium();
      break;
    case '\0':
      break;
    default:
      DEBUG_PRINTLN(F("NACK"));
      break;
  }
}

void parseCommandFilesIridium() {
  char *fileNamePtr              = (char *)receiveBuffer + 2;
  char fileName[numCharsIridium] = {0};
  strcpy(fileName, fileNamePtr);

  switch (receiveBuffer[1]) {
    case CMD_FILES_LIST:
      SDQuery();
      break;
    case CMD_FILES_SIZE:
      SDSize(fileName);
      break;
    case CMD_FILES_DOWNLOAD:
      SDDownload(fileName);
      break;
    case CMD_FILES_CREATE:
      SDCreate(fileName);
      break;
    case CMD_FILES_REMOVE:
      SDDelete(fileName);
      break;
    case CMD_FILES_WRITE:
      SDWriteStream(fileName);
      break;
    case CMD_FILES_WRITENEWLINE:
      SDWriteStreamNewLine();
      break;
    case CMD_FILES_QUIT:
      SDQuit();
      break;
    case CMD_FILES_BLCKBOX:
      BlackBoxPrint();
      break;
    case CMD_FILES_BLCKBOXEMPTY:
      BlackBoxEmpty();
      break;
    case '\0':
      break;
    default:
      DEBUG_PRINTLN(F("NACK"));
      break;
  }
}

void parseCommandStrategyIridium() {
  switch (receiveBuffer[1]) {
    case CMD_STRATEGY_SET:
      DEBUG_PRINT(F("Manual Strategy Set to: "));
      DEBUG_PRINTLN((int)(receiveBuffer[2] - '0'));
      if (!SetMode((int)(receiveBuffer[2] - '0')))
        DEBUG_PRINTLN(F("Mode not found!"));
      break;
    case CMD_STRATEGY_FUNCTION:
      strategyMethods[3][mode]();
      break;
    case CMD_STRATEGY_OVERRIDE:
      // Override
      break;
    case '\0':
      break;
    default:
      DEBUG_PRINTLN(F("NACK"));
      break;
  }
}

void parseCommandBackupIridium() {
  switch (receiveBuffer[1]) {
    case CMD_BACKUP_RST:
      DEBUG_PRINTLN(F("Manual Reset of Backup System."));
      ResetBackupCPU();
      break;
    case CMD_BACKUP_PRIMSTATUS:
      DEBUG_PRINT(F("Backup System Status: "));
      DEBUG_PRINTLN(GetStatus(MODULE_BACKUPCPU));
      break;
    case CMD_BACKUP_HB:
      DEBUG_PRINTLN(F("Virtual Heartbeat"));
      HeartBeatInInterrupt();
      break;
    case CMD_BACKUP_FREEZE:
      DEBUG_PRINTLN(F("Simulating System Halt for 60 sec"));
      delay(60000);
      break;
    case '\0':
      break;
    default:
      DEBUG_PRINTLN(F("NACK"));
      break;
  }
}

void parseCommandModuleIridium() {
  char *modulePtr                  = (char *)receiveBuffer + 2;
  char moduleChar[numCharsIridium] = {0};
  strcpy(moduleChar, modulePtr);

  int moduleSlct = atoi(moduleChar);

  switch (receiveBuffer[1]) {
    case CMD_MODULE_ENABLE:
      DEBUG_PRINT(F("Manual Enable of Module: "));
      switch (receiveBuffer[2]) {
        case '\0':
          DEBUG_PRINTLN(F("ALL SYSTEMS"));
          break;
        default:
          DEBUG_PRINTLN(moduleSlct);
          SystemEnable(moduleSlct);
          break;
      }
      break;
    case CMD_MODULE_DISABLE:
      DEBUG_PRINT(F("Manual Disable of Module: "));
      switch (receiveBuffer[2]) {
        case '\0':
          DEBUG_PRINTLN(F("ALL SYSTEMS"));
          SystemDisable();
          break;
        default:
          DEBUG_PRINTLN(moduleSlct);
          SystemDisable(moduleSlct);
          break;
      }
      break;
    case CMD_MODULE_OVERRIDE:
      DEBUG_PRINT(F("Manual Override of Module: "));
      switch (receiveBuffer[2]) {
        case '\0':
          DEBUG_PRINTLN(F("NACK"));
          // SystemDisable();
          break;
        default:
          DEBUG_PRINT(moduleSlct);
          DEBUG_PRINT(F("\t"));
          DEBUG_PRINTLN(ToBoolString(!GetStatus(moduleSlct)));
          SetStatus(moduleSlct, !GetStatus(moduleSlct));
          break;
      }
      break;
    case CMD_MODULE_STATUS:
      DEBUG_PRINTLN(F("Manual System Status Check"));
      // GetStatus(true);
      DEBUG_PRINT(F("System Status: "));
      DEBUG_PRINTLN(String(ToLong(SystemStatus)));
      break;
    case CMD_MODULE_RESET:
      DEBUG_PRINT(F("Manual System Reset in: "));
      CountDownPrint();
      systemReset();
      break;
    case '\0':
      break;
    default:
      DEBUG_PRINTLN(F("NACK"));
      break;
  }
}

void parseCommandRouteIridium() {
  char tempChars[numCharsIridium];
  char *strtokIndx;
  strcpy(tempChars, (char *)receiveBuffer);

  long latLong;
  long lonLong;

  strtokIndx = strtok(tempChars, ",");
  strtokIndx = strtok(NULL, ",");
  latLong    = atol(strtokIndx);
  strtokIndx = strtok(NULL, ",");
  lonLong    = atol(strtokIndx);

  switch (receiveBuffer[1]) {
    case CMD_ROUTE_SET:
      // write coordinate
      DEBUG_PRINT(F("Lat: "));
      DEBUG_PRINT(latLong);
      DEBUG_PRINT(F(" Lon: "));
      DEBUG_PRINTLN(lonLong);

      EEPROM_WRITE_LAT((long)(receiveBuffer[2] - '0'), latLong);
      EEPROM_WRITE_LON((long)(receiveBuffer[2] - '0'), lonLong);
      break;
    case CMD_ROUTE_PRINT:
      EEPROM_READ_LAT((long)(receiveBuffer[2] - '0'), latLong);
      EEPROM_READ_LON((long)(receiveBuffer[2] - '0'), lonLong);

      DEBUG_PRINT(F("Lat: "));
      DEBUG_PRINT(latLong);
      DEBUG_PRINT(F(" Lon: "));
      DEBUG_PRINTLN(lonLong);
      break;
    case CMD_ROUTE_OVERRIDE:
      DEBUG_PRINTLN(F("Setting Home Position Override Flag."));
      SetHomePosOverride(true, false);
      break;
    case CMD_ROUTE_OVERRIDE_ALT:
      DEBUG_PRINTLN(F("Setting Alternative Home Position Override Flag."));
      SetHomePosOverride(true, false);
      break;
    default:
      break;
  }
}

// Populate Iridium Package with latest data
void PopulateSendBuffer() {
  union package pack;

  // Battery Level
  pack.i        = BatteryLevel();
  sendBuffer[0] = pack.ui8[0];
  sendBuffer[1] = pack.ui8[1];

  // Current Latitude Position
  pack.l        = GnssGetLat();
  sendBuffer[2] = pack.ui8[0];
  sendBuffer[3] = pack.ui8[1];
  sendBuffer[4] = pack.ui8[2];
  sendBuffer[5] = pack.ui8[3];

  // Current Longitude Position
  pack.l        = GnssGetLong();
  sendBuffer[6] = pack.ui8[0];
  sendBuffer[7] = pack.ui8[1];
  sendBuffer[8] = pack.ui8[2];
  sendBuffer[9] = pack.ui8[3];

  // System Status
  pack.ul        = ToLong(SystemStatus);
  sendBuffer[10] = pack.ui8[0];
  sendBuffer[11] = pack.ui8[1];
  sendBuffer[12] = pack.ui8[2];
  sendBuffer[13] = pack.ui8[3];

  sendBuferSize = 14;
}

// Full Iridium Test
bool IridiumTest() {
  bool status = true;

  int err;
  // err = modem.getSignalQuality(signalQuality);
  // if (err != ISBD_SUCCESS) {
  //   status = false;
  // }

  // DEBUG_PRINT(F("Iridium: "));
  // if (status) {
  //   DEBUG_PRINT(F("Signal Quality: "));
  //   DEBUG_PRINTLN(signalQuality);
  // } else {
  //   DEBUG_PRINTLN(F("ERROR"));
  // }

  // Example: Print the firmware revision
  // char version[12];
  // err = modem.getFirmwareVersion(version, sizeof(version));
  // if (err != ISBD_SUCCESS) {
  //   DEBUG_PRINT(F("Firmware Version failed: Error "));
  //   DEBUG_PRINTLN(err);
  //   status = false;
  // }
  // DEBUG_PRINT(F("Firmware Version is "));
  // DEBUG_PRINT(version);
  // DEBUG_PRINTLN(F("."));

  // // Example: Print the IMEI
  // char IMEI[16];
  // err = modem.getIMEI(IMEI, sizeof(IMEI));
  // if (err != ISBD_SUCCESS) {
  //   DEBUG_PRINT(F("getIMEI failed: Error "));
  //   DEBUG_PRINTLN(err);
  //   status = false;
  // }
  // DEBUG_PRINT(F("IMEI is "));
  // DEBUG_PRINT(IMEI);
  // DEBUG_PRINTLN(F("."));

  // Example: Test the signal quality.
  // This returns a number between 0 and 5.
  // 2 or better is preferred.
  err = modem.getSignalQuality(signalQuality);
  if (err != ISBD_SUCCESS) {
    DEBUG_PRINT(F("SignalQuality failed: Error "));
    DEBUG_PRINTLN(err);
    status = false;
  }

  DEBUG_PRINT(F("On a scale of 0 to 5, signal quality is currently "));
  DEBUG_PRINT(signalQuality);
  DEBUG_PRINTLN(F("."));

  DEBUG_PRINTLINE();
  if (signalQuality <= 0) {
    return status;
  }
  sendBuffer[0] = (uint8_t)BatteryLevel();
  IridiumSend();

  return status;
}

#if DIAGNOSTICS
void ISBDConsoleCallback(IridiumSBD *device, char c) {
  Serial.write(c);
}

void ISBDDiagsCallback(IridiumSBD *device, char c) {
  Serial.write(c);
}
#endif
