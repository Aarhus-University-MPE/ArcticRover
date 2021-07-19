/*
    USB communication, handles reading serial and parsing commands.

    By
    Mads Rosenhoej Jeppesen - Aarhus 2021
    mrj@mpe.au.dk

    Query data from Drill Logger SD card
*/

const byte numChars = 32;
char receivedCMD[numChars];

void initializeDebugComm() {
  Serial.begin(DEBUG_BAUDRATE);
  if (Serial) SetStatus(MODULE_DEBUGCOMM, true);
  else SetStatus(MODULE_DEBUGCOMM, false);
}

// Receive Commands
void recvWithStartEndMarkers() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
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
      }
      else {
        receivedCMD[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        parseCommand();
      }
    }

    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
}

// Parse read Command
void parseCommand() {
  Serial.println(receivedCMD);
  
  switch (receivedCMD[0])
  {
    case CMD_FILES:
      parseCommandFiles();
      break;
    case CMD_STRATEGY:
      parseCommandStrategy();
    case CMD_BACKUP:
      parseCommandBackup();
      break;
    case CMD_MODULE:
      praseCommandModule();
    case '\0':
      break;
    default:
      DEBUG_PRINTLN("NACK");
      break;
  }
}


void parseCommandFiles() {
  char* fileNamePtr = receivedCMD + 2;
  char fileName[numChars] = {0};
  strcpy(fileName, fileNamePtr);

  switch (receivedCMD[1])
  {
    case CMD_FILES_LIST:
      SDQuery();
      break;
    case CMD_FILES_SIZE:
      SDSize(fileName);
      break;
    case CMD_FILES_DOWNLOAD:
      SDDownload(fileName);
      break;
    case CMD_FILES_DELETE:
      SDDelete(fileName);
      break;
    default:
      DEBUG_PRINTLN("NACK");
      break;
  }
}

void parseCommandStrategy() {
  switch (receivedCMD[1])
  {
    case CMD_STRATEGY_SET:
      DEBUG_PRINTLN("Manual Strategy Set");
      if (!SetMode((int)(receivedCMD[2] - '0'))) DEBUG_PRINTLN("Mode not found!");
      break;
    case CMD_STRATEGY_OVERRIDE:
      // Override
      break;
    default:
      DEBUG_PRINTLN("NACK");
      break;
  }
}

void parseCommandBackup()
{
  switch (receivedCMD[1])
  {
  case CMD_BACKUP_RST:
    DEBUG_PRINTLN("Manual Reset of Backup System.");
    ResetBackupCPU();
    break;
  case CMD_BACKUP_PRIMSTATUS:
    DEBUG_PRINT("Backup System Status: ");
    DEBUG_PRINTLN(GetStatus(MODULE_BACKUPCPU));
    break;
  case CMD_BACKUP_HB:
    DEBUG_PRINTLN("Virtual Heartbeat");
    HeartBeatInInterrupt();
    break;
  default:
    DEBUG_PRINTLN("NACK");
    break;
  }
}

void praseCommandModule(){
  switch (receivedCMD[1])
  {
  case CMD_MODULE_ENABLE:
    DEBUG_PRINTLN("Manual Enable of Module.");
    DEBUG_PRINTLN((int)(receivedCMD[2] - '0'));
    SystemEnable((int)(receivedCMD[2] - '0'));
    break;
  case CMD_MODULE_DISABLE:
    DEBUG_PRINT("Manual Disable of Module.");
    SystemDisable((int)receivedCMD[2]);
    break;
  default:
    DEBUG_PRINTLN("NACK");
    break;
  }
}