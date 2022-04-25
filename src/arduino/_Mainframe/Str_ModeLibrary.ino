/*  Mode Library mode

    Cycles through modes, activates mode with Select Button.
*/

// Start sequence of strategy
void StartStrategyModeLibrary() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (Mode Library): Starting"));

  lastMillisMode  = millis();
  lastMillisEstop = millis();

  // Assign mode button interrupt
  attachInterrupt(PI_INT_BUTTON_MODE, ModeFunctionModeLibrary, FALLING);

  AttachSelectButton();

  DEBUG_PRINTLN(F("Strategy (Mode Library): Initialized"));
  DEBUG_PRINTLINE();
  DEBUG_PRINT(F("Mode Cycle: "));
  DEBUG_PRINTLN(ModeToString(modeCycle));
}

// Main sequence of strategy
void RunStrategyModeLibrary() {
  StrategyRunLed(modeCycle);
}

// End sequence of strategy
void FinishStrategyModeLibrary() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (Mode Library): Ending"));
  DEBUG_PRINTLN(F("Strategy (Mode Library): Finished"));

  DetachModeButton();
  DetachSelectButton();

  // Assign mode button interrupt
  AttachModeButton();
}

// Select button function
void SelectFunctionModeLibrary() {
  if (!SelectButtonDebounce()) {
    return;
  }

  // Select Mode
  DEBUG_PRINTLN(F("Mode Selected."));
  SetMode(modeCycle);
}

// Mode button function
void ModeFunctionModeLibrary() {
  if (!ModeButtonDebounce()) {
    return;
  }
  lastMillistModeBlink = 0;

  // Cycle Mode
  if (modeCycle + 1 < MODES_MIN_BROWSABLE || modeCycle + 1 >= MODES_MAX) {
    modeCycle = MODES_MIN_BROWSABLE;
  } else {
    modeCycle = modeCycle + 1;
  }

  DEBUG_PRINT(F("Mode Cycle: "));
  DEBUG_PRINTLN(ModeToString(modeCycle));

  ResetLed();
}