/*
  GeoRover Blackbox protocols

  Mads Rosenhøj Jepepsen
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
    blackBoxFile.println("GeoRover Blackbox");
    blackBoxFile.print("System Version:;");
    blackBoxFile.println(SystemVersion);
    blackBoxFile.println();
    blackBoxFile.close();

    SetStatus(MODULE_BLACKBOX, true);
  } else {
    SetStatus(MODULE_BLACKBOX, false);
  }

  return BlackBoxStatus;
}

bool BlackBoxStatus() {
  return GetStatus(MODULE_BLACKBOX);
}

void BlackBoxAppendln() {
  if (BlackBoxStatus()) {
    File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
    if (blackBoxFile) {
      if (newLine) {
        blackBoxFile.print(millis());
        blackBoxFile.print(";");
      }
      blackBoxFile.println();
      newLine = true;
      blackBoxFile.close();
    } else {
      DEBUG_PRINTLN("Blackbox Error");

      SetStatus(MODULE_BLACKBOX, false);
    }
  }
}

void BlackBoxAppendln(String blackBoxInput) {
  if (BlackBoxStatus()) {
    File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
    if (blackBoxFile) {
      if (newLine) {
        blackBoxFile.print(millis());
        blackBoxFile.print(";");
      }
      blackBoxFile.println(blackBoxInput);
      newLine = true;
      blackBoxFile.close();
    } else {
      DEBUG_PRINTLN("Blackbox Error");

      SetStatus(MODULE_BLACKBOX, false);
    }
  }
}

void BlackBoxAppendln(float blackBoxInput) {
  if (BlackBoxStatus()) {
    File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
    if (blackBoxFile) {
      if (newLine) {
        blackBoxFile.print(millis());
        blackBoxFile.print(";");
      }
      blackBoxFile.println(blackBoxInput);
      newLine = true;
      blackBoxFile.close();
    } else {
      DEBUG_PRINTLN("Blackbox Error");

      SetStatus(MODULE_BLACKBOX, false);
    }
  }
}

void BlackBoxAppendln(byte blackBoxInput) {
  if (BlackBoxStatus()) {
    File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
    if (blackBoxFile) {
      if (newLine) {
        blackBoxFile.print(millis());
        blackBoxFile.print(";");
      }
      blackBoxFile.println(blackBoxInput);
      newLine = true;
      blackBoxFile.close();
    } else {
      DEBUG_PRINTLN("Blackbox Error");

      SetStatus(MODULE_BLACKBOX, false);
    }
  }
}
void BlackBoxAppendln(char blackBoxInput) {
  if (BlackBoxStatus()) {
    File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
    if (blackBoxFile) {
      if (newLine) {
        blackBoxFile.print(millis());
        blackBoxFile.print(";");
      }
      blackBoxFile.println(blackBoxInput);
      newLine = true;
      blackBoxFile.close();
    } else {
      DEBUG_PRINTLN("Blackbox Error");

      SetStatus(MODULE_BLACKBOX, false);
    }
  }
}

void BlackBoxAppendln(int blackBoxInput) {
  if (BlackBoxStatus()) {
    File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
    if (blackBoxFile) {
      if (newLine) {
        blackBoxFile.print(millis());
        blackBoxFile.print(";");
      }
      blackBoxFile.println(blackBoxInput);
      newLine = true;
      blackBoxFile.close();
    } else {
      DEBUG_PRINTLN("Blackbox Error");

      SetStatus(MODULE_BLACKBOX, false);
    }
  }
}

void BlackBoxAppendln(bool blackBoxInput) {
  if (BlackBoxStatus()) {
    File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
    if (blackBoxFile) {
      if (newLine) {
        blackBoxFile.print(millis());
        blackBoxFile.print(";");
      }
      blackBoxFile.println(blackBoxInput);
      newLine = true;
      blackBoxFile.close();
    } else {
      DEBUG_PRINTLN("Blackbox Error");

      SetStatus(MODULE_BLACKBOX, false);
    }
  }
}

void BlackBoxAppendln(long int blackBoxInput) {
  if (BlackBoxStatus()) {
    File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
    if (blackBoxFile) {
      if (newLine) {
        blackBoxFile.print(millis());
        blackBoxFile.print(";");
      }
      blackBoxFile.println(blackBoxInput);
      newLine = true;
      blackBoxFile.close();
    } else {
      DEBUG_PRINTLN("Blackbox Error");

      SetStatus(MODULE_BLACKBOX, false);
    }
  }
}

void BlackBoxAppendln(unsigned long blackBoxInput) {
  if (BlackBoxStatus()) {
    File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
    if (blackBoxFile) {
      if (newLine) {
        blackBoxFile.print(millis());
        blackBoxFile.print(";");
      }
      blackBoxFile.println(blackBoxInput);
      newLine = true;
      blackBoxFile.close();
    } else {
      DEBUG_PRINTLN("Blackbox Error");

      SetStatus(MODULE_BLACKBOX, false);
    }
  }
}

void BlackBoxAppendln(long int blackBoxInput, int Type) {
  if (BlackBoxStatus()) {
    File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
    if (blackBoxFile) {
      if (newLine) {
        blackBoxFile.print(millis());
        blackBoxFile.print(";");
      }
      blackBoxFile.print(blackBoxInput, Type);
      blackBoxFile.println();
      newLine = true;
      blackBoxFile.close();
    } else {
      DEBUG_PRINTLN("Blackbox Error");

      SetStatus(MODULE_BLACKBOX, false);
    }
  }
}

void BlackBoxAppend(String blackBoxInput) {
  if (BlackBoxStatus()) {
    File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
    if (blackBoxFile) {
      if (newLine) {
        blackBoxFile.print(millis());
        blackBoxFile.print(";");
      }
      newLine = false;
      blackBoxFile.print(blackBoxInput);
      blackBoxFile.close();
    } else {
      DEBUG_PRINTLN("Blackbox Error");

      SetStatus(MODULE_BLACKBOX, false);
    }
  }
}

void BlackBoxAppend(float blackBoxInput) {
  if (BlackBoxStatus()) {
    File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
    if (blackBoxFile) {
      if (newLine) {
        blackBoxFile.print(millis());
        blackBoxFile.print(";");
      }
      newLine = false;
      blackBoxFile.print(blackBoxInput);
      blackBoxFile.close();
    } else {
      DEBUG_PRINTLN("Blackbox Error");

      SetStatus(MODULE_BLACKBOX, false);
    }
  }
}

void BlackBoxAppend(byte blackBoxInput) {
  if (BlackBoxStatus()) {
    File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
    if (blackBoxFile) {
      if (newLine) {
        blackBoxFile.print(millis());
        blackBoxFile.print(";");
      }
      newLine = false;
      blackBoxFile.print(blackBoxInput);
      blackBoxFile.close();
    } else {
      DEBUG_PRINTLN("Blackbox Error");

      SetStatus(MODULE_BLACKBOX, false);
    }
  }
}

void BlackBoxAppend(char blackBoxInput) {
  if (BlackBoxStatus()) {
    File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
    if (blackBoxFile) {
      if (newLine) {
        blackBoxFile.print(millis());
        blackBoxFile.print(";");
      }
      newLine = false;
      blackBoxFile.print(blackBoxInput);
      blackBoxFile.close();
    } else {
      DEBUG_PRINTLN("Blackbox Error");

      SetStatus(MODULE_BLACKBOX, false);
    }
  }
}

void BlackBoxAppend(int blackBoxInput) {
  if (BlackBoxStatus()) {
    File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
    if (blackBoxFile) {
      if (newLine) {
        blackBoxFile.print(millis());
        blackBoxFile.print(";");
      }
      newLine = false;
      blackBoxFile.print(blackBoxInput);
      blackBoxFile.close();
    } else {
      DEBUG_PRINTLN("Blackbox Error");

      SetStatus(MODULE_BLACKBOX, false);
    }
  }
}

void BlackBoxAppend(bool blackBoxInput) {
  if (BlackBoxStatus()) {
    File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
    if (blackBoxFile) {
      if (newLine) {
        blackBoxFile.print(millis());
        blackBoxFile.print(";");
      }
      newLine = false;
      blackBoxFile.print(blackBoxInput);
      blackBoxFile.close();
    } else {
      DEBUG_PRINTLN("Blackbox Error");

      SetStatus(MODULE_BLACKBOX, false);
    }
  }
}

void BlackBoxAppend(long int blackBoxInput) {
  if (BlackBoxStatus()) {
    File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
    if (blackBoxFile) {
      if (newLine) {
        blackBoxFile.print(millis());
        blackBoxFile.print(";");
      }
      newLine = false;
      blackBoxFile.print(blackBoxInput);
      blackBoxFile.close();
    } else {
      DEBUG_PRINTLN("Blackbox Error");

      SetStatus(MODULE_BLACKBOX, false);
    }
  }
}

void BlackBoxAppend(unsigned long blackBoxInput) {
  if (BlackBoxStatus()) {
    File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
    if (blackBoxFile) {
      if (newLine) {
        blackBoxFile.print(millis());
        blackBoxFile.print(";");
      }
      newLine = false;
      blackBoxFile.print(blackBoxInput);
      blackBoxFile.close();
    } else {
      DEBUG_PRINTLN("Blackbox Error");

      SetStatus(MODULE_BLACKBOX, false);
    }
  }
}

void BlackBoxAppend(long int blackBoxInput, int Type) {
  if (BlackBoxStatus()) {
    File blackBoxFile = SD.open("Blackbox.csv", FILE_WRITE);
    if (blackBoxFile) {
      if (newLine) {
        blackBoxFile.print(millis());
        blackBoxFile.print(";");
      }
      newLine = false;
      blackBoxFile.print(blackBoxInput, Type);
      blackBoxFile.close();
    } else {
      DEBUG_PRINTLN("Blackbox Error");

      SetStatus(MODULE_BLACKBOX, false);
    }
  }
}
