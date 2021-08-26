/*
    USB communication, handles reading serial and parsing commands.

    By
    Mads Rosenhoej Jeppesen - Aarhus 2021
    mrj@mpe.au.dk

    Query data from Drill Logger SD card
*/

const byte numChars = 32;
char receivedCMD[numChars];

void initializeDebugComm()
{
  Serial.begin(DEBUG_BAUDRATE);
  if (COM_SERIAL_DBG)
    SetStatus(MODULE_DBGCOMM, true);
  else
    SetStatus(MODULE_DBGCOMM, false);

  DEBUG_PRINTLN("#---------------------------#");
  DEBUG_PRINTLN("#     GeoRover Firmware     #");
  DBG_ONLY(DEBUG_PRINTLN("#       #DEBUG MODE#        #"));
  DEBUG_PRINT("#  System Version: ");
  DEBUG_PRINT(SystemVersion);
  DEBUG_PRINTLN(" #");
  DEBUG_PRINTLN("#---------------------------#");
  DEBUG_PRINTLN();
}

// Receive Commands
void recvWithStartEndMarkers()
{
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;

  while (Serial.available() > 0)
  {
    rc = Serial.read();

    if (recvInProgress == true)
    {
      if (rc != endMarker)
      {
        receivedCMD[ndx] = rc;
        ndx++;
        if (ndx >= numChars)
        {
          ndx = numChars - 1;
        }
      }
      else
      {
        receivedCMD[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        parseCommand();
      }
    }

    else if (rc == startMarker)
    {
      recvInProgress = true;
    }
  }
}

bool DebugCommStatus()
{
  return COM_SERIAL_DBG;
}

// Parse read Command
void parseCommand()
{
  DEBUG_PRINT("Received command (DBG_PRT): \"");
  DEBUG_PRINT(receivedCMD);
  DEBUG_PRINTLN("\"");

  switch (receivedCMD[0])
  {
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
    praseCommandModule();
  case '\0':
    break;
  default:
    DEBUG_PRINTLN("NACK");
    break;
  }
}

void parseCommandFiles()
{
  char *fileNamePtr = receivedCMD + 2;
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
  case '\0':
    break;
  default:
    DEBUG_PRINTLN("NACK");
    break;
  }
}

void parseCommandStrategy()
{
  switch (receivedCMD[1])
  {
  case CMD_STRATEGY_SET:
    DEBUG_PRINT("Manual Strategy Set to: ");
    DEBUG_PRINTLN((int)(receivedCMD[2] - '0'));
    if (!SetMode((int)(receivedCMD[2] - '0')))
      DEBUG_PRINTLN("Mode not found!");
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
  case '\0':
    break;
  default:
    DEBUG_PRINTLN("NACK");
    break;
  }
}

void praseCommandModule()
{
  switch (receivedCMD[1])
  {
  case CMD_MODULE_ENABLE:
    DEBUG_PRINT("Manual Enable of Module: ");
    switch (receivedCMD[2])
    {
    case '\0':
      DEBUG_PRINTLN("ALL SYSTEMS");
      SystemEnable();
      break;
    default:
      DEBUG_PRINTLN((int)(receivedCMD[2] - '0'));
      SystemEnable((int)(receivedCMD[2] - '0'));
      break;
    }
  case CMD_MODULE_DISABLE:
    DEBUG_PRINT("Manual Disable of Module: ");
    switch (receivedCMD[2])
    {
    case '\0':
      DEBUG_PRINTLN("ALL SYSTEMS");
      SystemDisable();
      break;
    default:
      DEBUG_PRINTLN((int)(receivedCMD[2] - '0'));
      SystemDisable((int)(receivedCMD[2] - '0'));
      break;
    }
    break;
  case CMD_MODULE_STATUS:
    DEBUG_PRINTLN("Manual System Status Check");
    GetStatus(true);
    DEBUG_PRINT("System Status: ");
    DEBUG_PRINTLN(String(ToByte(SystemStatus)));
    break;
  case CMD_MODULE_RESET:
    DEBUG_PRINT("Manual System Reset in: ");
    CountDownPrint();
    systemReset();
    break;
  case '\0':
    break;
  default:
    DEBUG_PRINTLN("NACK");
    break;
  }
}

void CountDownPrint(){
    DEBUG_PRINT("3");
    delay(333);
    DEBUG_PRINT(".");
    delay(333);
    DEBUG_PRINT(". ");
    delay(333);
    DEBUG_PRINT("2");
    delay(333);
    DEBUG_PRINT(".");
    delay(333);
    DEBUG_PRINT(". ");
    delay(333);
    DEBUG_PRINT("1");
    delay(333);
    DEBUG_PRINT(".");
    delay(333);
    DEBUG_PRINTLN(". ");
    delay(333);
}