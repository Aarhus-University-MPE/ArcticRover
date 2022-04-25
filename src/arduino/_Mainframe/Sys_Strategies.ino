/*
  GeoRover Strategy Control functions

  Mads Rosenhøj Jeppesen
  Aarhus University
  2021
*/

// Set pointers for strategies methods
void InitStrategyMethods() {
  strategyMethods[0][MODE_EMERGENCY] = StartStrategyEmergency;
  strategyMethods[1][MODE_EMERGENCY] = RunStrategyEmergency;
  strategyMethods[2][MODE_EMERGENCY] = FinishStrategyEmergency;
  strategyMethods[3][MODE_EMERGENCY] = SelectFunctionEmergency;

  strategyMethods[0][MODE_MODELIBRARY] = StartStrategyModeLibrary;
  strategyMethods[1][MODE_MODELIBRARY] = RunStrategyModeLibrary;
  strategyMethods[2][MODE_MODELIBRARY] = FinishStrategyModeLibrary;
  strategyMethods[3][MODE_MODELIBRARY] = SelectFunctionModeLibrary;

  strategyMethods[0][MODE_IDLE] = StartStrategyIdle;
  strategyMethods[1][MODE_IDLE] = RunStrategyIdle;
  strategyMethods[2][MODE_IDLE] = FinishStrategyIdle;
  strategyMethods[3][MODE_IDLE] = SelectFunctionIdle;

  strategyMethods[0][MODE_SYSTEMTEST] = StartStrategySystemTest;
  strategyMethods[1][MODE_SYSTEMTEST] = RunStrategySystemTest;
  strategyMethods[2][MODE_SYSTEMTEST] = FinishStrategySystemTest;
  strategyMethods[3][MODE_SYSTEMTEST] = SelectFunctionSystemTest;

  strategyMethods[0][MODE_REMOTECONTROL] = StartStrategyRemote;
  strategyMethods[1][MODE_REMOTECONTROL] = RunStrategyRemote;
  strategyMethods[2][MODE_REMOTECONTROL] = FinishStrategyRemote;
  strategyMethods[3][MODE_REMOTECONTROL] = SelectFunctionRemote;

  strategyMethods[0][MODE_AUTONOMOUS] = StartStrategyAutonomous;
  strategyMethods[1][MODE_AUTONOMOUS] = RunStrategyAutonomous;
  strategyMethods[2][MODE_AUTONOMOUS] = FinishStrategyAutonomous;
  strategyMethods[3][MODE_AUTONOMOUS] = SelectFunctionAutonomous;
}

// Set last mode from EEPROM
void InitMode() {
  mode = EEPROM.read(MEMADDR_LASTMODE);
  if (!digitalRead(PI_BUTTON_ESTOP)) {
    mode = MODE_EMERGENCY;
  }
  if (mode == MODE_MODELIBRARY) mode = MODE_IDLE;

  SetMode(mode);

  strategyMethods[0][mode]();
}

// Checks if mode is updated and finish exit operations before changing
void ModeUpdater() {
  if (!isModeUpdated) {
    return;
  }

  isModeUpdated = false;
  // Skip finish operation when going to emergency
  if (mode != MODE_EMERGENCY) {
    strategyMethods[2][prevMode]();  // finish any operations for prevMode here
  }

  strategyMethods[0][mode]();  // init new strategy according to the new mode value
}

// Attach current mode's Select Button function to the Interrupt of the Select Button Input Pin
void AttachSelectButton() {
  lastMillisSelect = millis();
  attachInterrupt(PI_INT_BUTTON_SELECT, strategyMethods[3][mode], FALLING);
}

// Attach Default Mode function to the Interrupt of the Mode Button Input Pin
void AttachModeButton() {
  attachInterrupt(PI_INT_BUTTON_MODE, ModeButtonInterruptHandler, FALLING);
}

// Detach current Select Button's function
void DetachSelectButton() {
  detachInterrupt(PI_INT_BUTTON_SELECT);
}

// Detach current Mode Button's function
void DetachModeButton() {
  detachInterrupt(PI_INT_BUTTON_MODE);
}

// Tries set the mode and isModeUpdated flag
boolean SetMode(byte newMode) {
  if (newMode < MODES_MAX) {
    prevMode = mode;
    mode     = newMode;
    IncrementModeCycle();
    isModeUpdated = true;
    EEPROM.write(MEMADDR_LASTMODE, mode);
    DEBUG_PRINT(F("Mode Set: "));
    DEBUG_PRINTLN(ModeToString(mode));
    return true;
  }
  return false;
}

// Cycle Mode
void IncrementModeCycle() {
  if (mode == MODE_MODELIBRARY) {
    return;
  }
  modeCycle = mode;
  if (modeCycle + 1 < MODES_MIN_BROWSABLE || modeCycle + 1 >= MODES_MAX) {
    modeCycle = MODES_MIN_BROWSABLE;
  } else {
    modeCycle = modeCycle + 1;
  }
}
