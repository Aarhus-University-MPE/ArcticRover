/*
  GeoRover local storage protocols

  Mads Rosenhøj Jeppesen
  Aarhus University
  2021
*/
#include <SD.h>

File activeWriteFile;

// Initialize SD card reader module.
bool InitializeSDReader() {
  bool status = false;
  if (!SDReaderStatus()) {
    status = SD.begin(PO_SPISS_SDCARD);

    SetStatus(MODULE_SD, status);
  }
  return status;
}

void TerminateSDReader() {
  SD.end();
  SetStatus(MODULE_SD, false);
}
// Checks status of SD reader
bool SDReaderStatus() {
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
void printFiles(File dir) {
  DEBUG_PRINTLN(F(("Files in system:")));
  while (true) {
    File entry = dir.openNextFile();
    if (!entry) {
      File entry = dir.openNextFile();
      if (!entry)
        break;
    }

    if (!entry.isDirectory()) {
      DEBUG_PRINT(entry.name());
      DEBUG_PRINT(F(("\t\t")));
      DEBUG_PRINTLN2(entry.size(), DEC);
    }
    entry.close();
  }
  DEBUG_PRINTLN(F(("End of storage")));
  DEBUG_PRINTLINE();
}

// Print size of file on SD card
void SDSize(char fileName[]) {
  if (SDReaderStatus()) {
    appendCsv(fileName);
    DEBUG_PRINT(F("Opening file: "));
    DEBUG_PRINTLN(fileName);
    File file = SD.open(fileName);
    if (file) {
      DEBUG_PRINTLN("File size: " + (String)file.size() + " bytes");
      file.close();
    } else {
      DEBUG_PRINTLN(F("File not found!"));
    }
  }
}

// Print all data from datafile to serial port
void SDDownload(char fileName[]) {
  if (SDReaderStatus()) {
    appendCsv(fileName);
    DEBUG_PRINTLN(F("Downloading file: "));
    DEBUG_PRINTLN(fileName);
    File file = SD.open(fileName);
    if (file) {
      while (file.available()) {
        DEBUG_WRITE(file.read());
      }
      file.close();
      DEBUG_PRINTLINE();
      DEBUG_PRINTLN(F("End of File"));
    } else {
      DEBUG_PRINTLN(F("File not found!"));
    }
  }
}

// Delete file on SD card
void SDDelete(char fileName[]) {
  if (SDReaderStatus()) {
    appendCsv(fileName);
    if (SD.exists(fileName)) {
      DEBUG_PRINT(F("Deleting file: "));
      DEBUG_PRINT(fileName);
      SD.remove(fileName);
      DEBUG_PRINTLN(F(" - File Removed"));
    } else {
      DEBUG_PRINTLN(F(" - File not found"));
    }
  }
}

// Create empty file on SD card
void SDCreate(char fileName[]) {
  if (SDReaderStatus()) {
    appendCsv(fileName);
    if (SD.exists(fileName)) {
      DEBUG_PRINTLN(F("File already exist"));
    } else {
      DEBUG_PRINT(F("Creating file: "));
      DEBUG_PRINT(fileName);
      File file = SD.open(fileName, FILE_WRITE);
      if (file) {
        DEBUG_PRINTLN(F(" - Success!"));
        file.close();
      } else {
        DEBUG_PRINTLN(F(" - Error!"));
      }
    }
  }
}

/*
  Initializes file write stream
  First run opens file with name, consecutive runs appends data to opened file
  When complete run SDQuit() to close write stream
*/
bool SDWriteStream(char fileNameOrData[]) {
  if (!activeWriteFile) {
    return SDOpenWriteStream(fileNameOrData);
  }

  DEBUG_PRINT(F("Wrinting to file: "));
  DEBUG_PRINTLN(fileNameOrData);

  activeWriteFile.write(fileNameOrData);
  return true;
}

/*
  Initializes file write stream
  First run opens file with name, consecutive runs appends data to opened file
  When complete run SDQuit() to close write stream
*/
bool SDWriteStreamNewLine() {
  if (!activeWriteFile) {
    DEBUG_PRINTLN(F("No active file!"));
  }

  DEBUG_PRINTLN(F("Wrinting newline char to file"));

  activeWriteFile.write('\n');
  return true;
}

bool SDOpenWriteStream(char fileName[]) {
  appendCsv(fileName);
  if (!SD.exists(fileName)) {
    DEBUG_PRINT(F("Creating file: "));
    DEBUG_PRINT(fileName);
  } else {
    DEBUG_PRINT(F("Opening file: "));
    DEBUG_PRINT(fileName);
  }
  activeWriteFile = SD.open(fileName, FILE_WRITE);

  if (!activeWriteFile) {
    DEBUG_PRINTLN(F(" - Error!"));
    return false;
  }

  DEBUG_PRINTLN(F(" - Success"));
  return true;
}

// Closes current active file write stream
void SDQuit() {
  DEBUG_PRINTLN(F("Closing active file"));
  activeWriteFile.close();
}
