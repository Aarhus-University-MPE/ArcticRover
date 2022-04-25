/*
  GeoRover Blackbox protocols

  Mads Rosenhøj Jeppesen
  Aarhus University
  2021
*/

bool newLine = true;

// Initialize system Blackbox
bool InitBlackBox() {
  if (!SDReaderStatus()) {
    return false;
  }

  File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
  if (blackBoxFile) {
    SetStatus(MODULE_BLACKBOX, true);
  } else {
    SetStatus(MODULE_BLACKBOX, false);
  }
  blackBoxFile.close();

  BlackBoxAppend("GeoRover Blackbox ");
  BlackBoxAppend("System Version: ");
  BlackBoxAppendln(SystemVersion);

  return BlackBoxStatus;
}

/*
  Prints Contents of BlackBox File
  TODO: Limit to only print X lines
*/
void BlackBoxPrint() {
  if (SDReaderStatus()) {
    DEBUG_PRINTLN(F("Printing Blackbox: "));
    File file = SD.open("Blackbox.csv");
    if (file) {
      int idx;
      while (file.available() && idx < MAX_BLACKBOX_ROW_PRINT) {
        DEBUG_WRITE(file.read());
        idx++;
      }
      file.close();
      if (idx < MAX_BLACKBOX_ROW_PRINT) {
        DEBUG_PRINTLN(F("End of File"));
      } else {
        DEBUG_PRINTLN(F("Printed 100 Rows"));
      }
    } else {
      DEBUG_PRINTLN(F("File not found!"));
    }
  }
}

void BlackBoxEmpty() {
  if (SD.exists("Blackbox.csv")) {
    DEBUG_PRINTLN(F("BLACKBOX CLEARED"));
    SD.remove("Blackbox.csv");
    InitBlackBox();
  } else {
    DEBUG_PRINTLN(F("BLACKBOX ERROR"));
  }
}

bool BlackBoxStatus() {
  return GetStatus(MODULE_BLACKBOX);
}

void BlackBoxAppend(String blackBoxInput) {
  if (BlackBoxStatus()) {
    File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
    if (blackBoxFile) {
      if (newLine) {
        blackBoxFile.print((String)millis());
        blackBoxFile.print(F(";"));
        newLine = false;
      }
      blackBoxFile.print(blackBoxInput);
      blackBoxFile.close();
    } else {
      DEBUG_PRINTLN(F("Blackbox Error"));

      SetStatus(MODULE_BLACKBOX, false);
    }
  }
}

void BlackBoxAppendln() {
  BlackBoxAppend("\r\n");
  newLine = true;
}

void BlackBoxAppendln(String blackBoxInput) {
  BlackBoxAppend(blackBoxInput);
  BlackBoxAppendln();
}

void BlackBoxAppend(int blackBoxInput) {
  BlackBoxAppend((String)blackBoxInput);
}

void BlackBoxAppendln(int blackBoxInput) {
  BlackBoxAppend((String)blackBoxInput);
  BlackBoxAppendln();
}

void BlackBoxAppend(long int blackBoxInput, int type) {
  if (BlackBoxStatus()) {
    File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
    if (blackBoxFile) {
      if (newLine) {
        blackBoxFile.print(millis());
        blackBoxFile.print(F(";"));
        newLine = false;
      }
      blackBoxFile.print(blackBoxInput, type);
      blackBoxFile.close();
    } else {
      DEBUG_PRINTLN(F("Blackbox Error"));

      SetStatus(MODULE_BLACKBOX, false);
    }
  }
}

void BlackBoxAppendln(long int blackBoxInput, int type) {
  BlackBoxAppend(blackBoxInput, type);
  BlackBoxAppendln();
}

void BlackBoxAppendln(float blackBoxInput) {
  BlackBoxAppend((String)blackBoxInput);
  BlackBoxAppendln();
}

void BlackBoxAppend(float blackBoxInput) {
  BlackBoxAppend((String)blackBoxInput);
}

void BlackBoxAppend(long int blackBoxInput) {
  BlackBoxAppend((String)blackBoxInput);
}

void BlackBoxAppendln(long int blackBoxInput) {
  BlackBoxAppend((String)blackBoxInput);
  BlackBoxAppendln();
}

void BlackBoxAppendln(unsigned long blackBoxInput) {
  BlackBoxAppend((String)blackBoxInput);
  BlackBoxAppendln();
}

void BlackBoxAppend(unsigned long blackBoxInput) {
  BlackBoxAppend((String)blackBoxInput);
}
