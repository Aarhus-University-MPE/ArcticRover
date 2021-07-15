// Initialize SD card reader module.
void InitSDReader() {
  DEBUG_PRINT("Initializing SD card...");

  if (!SD.begin(PO_SPISS_SDCARD)) {
    SetStatus(MODULE_SD, false);
    DEBUG_PRINTLN("initialization failed!");
  }
  else {
    SetStatus(MODULE_SD, true);
    DEBUG_PRINTLN("initialization done.");
  }
}

// Initialize system Blackbox
void InitBlackBox(){
  if(SDReaderStatus()){
    if(!SD.exists("Blackbox.csv")){
      DEBUG_PRINTLN("Creating Blackbox");
      SDCreate("Blackbox");
    } 
    File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
    if(file){
      DEBUG_PRINTLN("Initializing Blackbox");
      blackBoxFile.println("GeoRover Blackbox");
      blackBoxFile.print("System Version:;");
      blackBoxFile.println(SystemVersion);
      blackBoxFile.println();      
      blackBoxFile.close();

      SetStatus(MODULE_BLACKBOX, true);
    }
    else SetStatus(MODULE_BLACKBOX, false);
  }
  else {
    DEBUG_PRINTLN("SD connection error!");
    SetStatus(MODULE_BLACKBOX, false);
  }
}


bool BlackBoxStatus(){
  if (SDReaderStatus()){
    if (!GetStatus(MODULE_BLACKBOX)) {
      DEBUG_PRINTLN("Blackbox not initialized!");
      InitBlackBox();
    }
  }
  else SetStatus(MODULE_BLACKBOX, false);
  return GetStatus(MODULE_BLACKBOX);
}



void BlackBoxAppend(String blackBoxInput){

}


// Checks status of SD reader, will try to initialize if not currently active
bool SDReaderStatus() {
  if (!GetStatus(MODULE_SD)) {
    DEBUG_PRINTLN("SD card not active!");
    InitSDReader();
  }

  return GetStatus(MODULE_SD);
}

// Query all files on SD card and print to serial prompt
void SDQuery() {
  if (SDReaderStatus()) {
    File file = SD.open("/");
    file.rewindDirectory();
    DBG_ONLY(printFiles(file));
    file.rewindDirectory();
    file.close();
    delay(10);
  }
}


// Print all files to serial port
void printFiles(File dir)
{
  DEBUG_PRINTLN("Files in system:");
  while (true)
  {
    File entry =  dir.openNextFile();
    if (! entry) {
      File entry =  dir.openNextFile();
      if (! entry) break;
    }

    if (!entry.isDirectory()) {
      DEBUG_PRINT(entry.name());
      DEBUG_PRINT("\t\t");
      DEBUG_PRINTLN2(entry.size(), DEC);
    }
    entry.close();
  }
  DEBUG_PRINTLN("End of storage");
}

// Print size of file on SD card
void SDSize(char fileName[]) {
  if (SDReaderStatus()) {
    appendCsv(fileName);
    DEBUG_PRINT("Opening file: ");
    DEBUG_PRINTLN(fileName);
    File file = SD.open(fileName);
    if (file) {
      DEBUG_PRINTLN("File size: " + (String)file.size() + " bytes");
      file.close();
    }
    else DEBUG_PRINTLN("File not found!");
  }
}

// Print all data from datafile to serial port
void SDDownload(char fileName[]) {
  if (SDReaderStatus()) {
    appendCsv(fileName);
    DEBUG_PRINTLN("Downloading file: ");
    DEBUG_PRINTLN(fileName);
    File file = SD.open(fileName);
    if (file) {
      while (file.available()) {
        DEBUG_WRITE(file.read());
      }
      file.close();
      DEBUG_PRINTLN("End of File");
    }
    else DEBUG_PRINTLN("File not found!");
  }
}

// Delete file on SD card
void SDDelete(char fileName[]) {
  if (SDReaderStatus()) {
    appendCsv(fileName);
    if (SD.exists(fileName)) {
      DEBUG_PRINTLN("Deleting file: ");
      DEBUG_PRINTLN(fileName);
      SD.remove(fileName);
      DEBUG_PRINTLN("File Removed");
    }
    else DEBUG_PRINTLN("File not found");
  }
}

// Create empty file on SD card
void SDCreate(char fileName[]) {
  if (SDReaderStatus()) {
    appendCsv(fileName);
    if (SD.exists(fileName)) {
      DEBUG_PRINTLN("File already exist");
    }
    else {
      DEBUG_PRINTLN("Creating file: ");
      DEBUG_PRINTLN(fileName);
      File file = SD.open(fileName, FILE_WRITE);
      file.close();
    }
  }
}
