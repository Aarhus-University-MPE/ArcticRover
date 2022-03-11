/*  Mode Library mode

    Cycles through modes, activates mode with Select Button.
*/

// Start sequence of strategy
void StartStrategyModeLibrary() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Strategy (Mode Library): Starting"));

  lastMillisMode = millis();
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

  // Assign mode button interrupt
  attachInterrupt(PI_INT_BUTTON_MODE, ModeButtonInterruptHandler, FALLING);
}

// Select button function
void SelectFunctionModeLibrary() {
  if (millis() - lastMillisSelect > BTN_DEBOUNCE_TIME) {
    lastMillisSelect = millis();

    delay(1);
    if (digitalRead(PI_BUTTON_SELECT)) {
      return;
    }

    // Select Mode
    DEBUG_PRINTLN(F("Mode Selected."));
    SetMode(modeCycle);
  }
}

// Mode button function
void ModeFunctionModeLibrary() {
  if (millis() - lastMillisMode > BTN_DEBOUNCE_TIME) {
    lastMillisMode = millis();

    lastMillistModeBlink = 0;

    delay(1);
    if (digitalRead(PI_BUTTON_MODE)) {
      return;
    }

    // Cycle Mode
    if (modeCycle + 1 < MODES_MIN_BROWSABLE || modeCycle + 1 >= MODES_MAX) {
      modeCycle = MODES_MIN_BROWSABLE;
    } else {
      modeCycle = modeCycle + 1;
    }

    DEBUG_PRINT(F("Mode Cycle: "));
    DEBUG_PRINTLN(ModeToString(modeCycle));
  }
}