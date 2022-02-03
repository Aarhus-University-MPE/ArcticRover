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
  if (Serial)
    SetStatus(MODULE_DBGCOMM, true);
  else
    SetStatus(MODULE_DBGCOMM, false);
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
      } else {
        receivedCMD[ndx] = '\0';  // terminate the string
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
  switch (receivedCMD[0]) {
    case CMD_BACKUP:
      parseCommandBackup();
      break;
    case '\0':
      break;
    default:
      DEBUG_PRINTLN("NACK");
      break;
  }
}

void parseCommandBackup() {
  switch (receivedCMD[1]) {
    case CMD_BACKUP_RST:
      DEBUG_PRINTLN("Manual Reset of Primary System.");
      ResetPrimaryCPU();
      break;
    case CMD_BACKUP_BCKUPSTATUS:
      DEBUG_PRINT("Primary System Status: ");
      DEBUG_PRINTLN(GetStatus(MODULE_PRIMARYCPU));
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