/*
    USB communication, handles reading serial and parsing commands.

    By
    Mads Rosenhoej Jeppesen - Aarhus 2021
    mrj@mpe.au.dk

    Query data from Drill Logger SD card
*/

const byte numChars = 32;
char receivedCMD[numChars];
int moduleSlct;

void initializeDebugComm() {
  Serial.begin(DEBUG_BAUDRATE);
  if (COM_SERIAL_DBG)
    SetStatus(MODULE_DBGCOMM, true);
  else
    SetStatus(MODULE_DBGCOMM, false);

  DEBUG_PRINTLN(F("#---------------------------#"));
  DEBUG_PRINTLN(F("#     GeoRover Firmware     #"));
  DBG_ONLY(DEBUG_PRINTLN(F("#       #DEBUG MODE#        #")));
  DEBUG_PRINT(F("#  System Version: "));
  DEBUG_PRINT(SystemVersion);
  DEBUG_PRINTLN(F(" #"));
  DEBUG_PRINTLN(F("#---------------------------#"));
  DEBUG_PRINTLN();
}

// Receive Commands
void recvWithStartEndMarkers() {
  if (Serial.available() <= 0) {
    return;
  }

  static boolean recvInProgress = false;
  static byte ndx               = 0;
  char startMarker              = '<';
  char endMarker                = '>';
  char rc;

  while (Serial.available() > 0) {
    rc = Serial.read();

    if (recvInProgress == true) {
      if (rc != endMarker) {
        receivedCMD[ndx] = rc;
        ndx++;
        if (ndx >= numChars) {
          ndx = numChars - 1;
        }
      } else {
        receivedCMD[ndx] = '\0';  // terminate the string
        recvInProgress   = false;
        ndx              = 0;
        parseCommand();
      }
    }

    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
}

bool activeCommand = false;
void PerformCommand() {
  if (!activeCommand) {
    return;
  }

  SystemTestModule(moduleSlct);
}

bool DebugCommStatus() {
  return COM_SERIAL_DBG;
}

// Parse read Command
void parseCommand() {
  DEBUG_PRINT(F("Received command (DBG_PRT): \""));
  DEBUG_PRINT(receivedCMD);
  DEBUG_PRINTLN(F("\""));

  switch (receivedCMD[0]) {
    case CMD_FILES:
      parseCommandFiles();
      break;
    case CMD_STRATEGY:
      parseCommandStrategy();
      break;
    case CMD_BACKUP:
      parseCommandBackup();
      break;
    case CMD_MODULE:
      parseCommandModule();
      break;
    case CMD_ROUTE:
      parseCommandRoute();
      break;
    case '\0':
      break;
    default:
      DEBUG_PRINTLN(F("NACK"));
      break;
  }
}

void parseCommandFiles() {
  char *fileNamePtr       = receivedCMD + 2;
  char fileName[numChars] = {0};
  strcpy(fileName, fileNamePtr);

  switch (receivedCMD[1]) {
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

void parseCommandStrategy() {
  switch (receivedCMD[1]) {
    case CMD_STRATEGY_SET:
      DEBUG_PRINT(F("Manual Strategy Set to: "));
      DEBUG_PRINTLN((int)(receivedCMD[2] - '0'));
      if (!SetMode((int)(receivedCMD[2] - '0')))
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

void parseCommandBackup() {
  switch (receivedCMD[1]) {
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

void parseCommandModule() {
  char *modulePtr           = receivedCMD + 2;
  char moduleChar[numChars] = {0};
  strcpy(moduleChar, modulePtr);

  moduleSlct = atoi(moduleChar);

  switch (receivedCMD[1]) {
    case CMD_MODULE_ENABLE:
      DEBUG_PRINT(F("Manual Enable of Module: "));
      switch (receivedCMD[2]) {
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
      switch (receivedCMD[2]) {
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
      switch (receivedCMD[2]) {
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
    case CMD_MODULE_TEST:
      DEBUG_PRINT(F("Testing Module: "));
      DEBUG_PRINTLN(ModuleToString(moduleSlct));
      DEBUG_PRINTLINE();
      activeCommand = true;
      break;
    case CMD_MODULE_STOPTEST:
      DEBUG_PRINTLINE();
      DEBUG_PRINTLN(F("Manual System Test Stop"));
      SystemDisable(moduleSlct);
      DEBUG_PRINTLINE();
      activeCommand   = false;
      systemTestState = -1;
      break;
    case '\0':
      break;
    default:
      DEBUG_PRINTLN(F("NACK"));
      break;
  }
}

void parseCommandRoute() {
  char tempChars[numChars];
  char *strtokIndx;
  strcpy(tempChars, receivedCMD);

  long latLong;
  long lonLong;

  strtokIndx = strtok(tempChars, ",");
  strtokIndx = strtok(NULL, ",");
  latLong    = atol(strtokIndx);
  strtokIndx = strtok(NULL, ",");
  lonLong    = atol(strtokIndx);

  switch (receivedCMD[1]) {
    case CMD_ROUTE_SET:
      // write coordinate
      DEBUG_PRINT(F("Lat: "));
      DEBUG_PRINT(latLong);
      DEBUG_PRINT(F(" Lon: "));
      DEBUG_PRINTLN(lonLong);

      EEPROM_WRITE_LAT((long)(receivedCMD[2] - '0'), latLong);
      EEPROM_WRITE_LON((long)(receivedCMD[2] - '0'), lonLong);
      break;
    case CMD_ROUTE_PRINT:
      EEPROM_READ_LAT((long)(receivedCMD[2] - '0'), latLong);
      EEPROM_READ_LON((long)(receivedCMD[2] - '0'), lonLong);

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

void parseData() {  // split the data into its parts

  char *strtokIndx;  // this is used by strtok() as an index
}

void CountDownPrint() {
  DEBUG_PRINT(F("3"));
  delay(333);
  DEBUG_PRINT(F("."));
  delay(333);
  DEBUG_PRINT(F(". "));
  delay(333);
  DEBUG_PRINT(F("2"));
  delay(333);
  DEBUG_PRINT(F("."));
  delay(333);
  DEBUG_PRINT(F(". "));
  delay(333);
  DEBUG_PRINT(F("1"));
  delay(333);
  DEBUG_PRINT(F("."));
  delay(333);
  DEBUG_PRINTLN(F(". "));
  delay(333);
}