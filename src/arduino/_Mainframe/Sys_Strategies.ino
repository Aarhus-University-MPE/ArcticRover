
// Set pointers for strategies methods
void InitStrategyMethods()
{
  strategyMethods[0][MODE_EMERGENCY] = StartStrategyEmergency;
  strategyMethods[1][MODE_EMERGENCY] = RunStrategyEmergency;
  strategyMethods[2][MODE_EMERGENCY] = FinishStrategyEmergency;

  strategyMethods[0][MODE_IDLE] = StartStrategyIdle;
  strategyMethods[1][MODE_IDLE] = RunStrategyIdle;
  strategyMethods[2][MODE_IDLE] = FinishStrategyIdle;

  strategyMethods[0][MODE_SYSTEMTEST] = StartStrategySystemTest;
  strategyMethods[1][MODE_SYSTEMTEST] = RunStrategySystemTest;
  strategyMethods[2][MODE_SYSTEMTEST] = FinishStrategySystemTest;

  strategyMethods[0][MODE_REMOTECONTROL] = StartStrategyRemote;
  strategyMethods[1][MODE_REMOTECONTROL] = RunStrategyRemote;
  strategyMethods[2][MODE_REMOTECONTROL] = FinishStrategyRemote;

  strategyMethods[0][MODE_AUTONOMOUS] = StartStrategyAutonomous;
  strategyMethods[1][MODE_AUTONOMOUS] = RunStrategyAutonomous;
  strategyMethods[2][MODE_AUTONOMOUS] = FinishStrategyAutonomous;
}

// Set last mode from EEPROM
void InitMode()
{
  mode = EEPROM.read(MEMADDR_LASTMODE);
  strategyMethods[0][mode]();
}

// Checks if mode is updated and finish exit operations before changing
void ModeUpdater()
{
  while (isModeUpdated)
  {
    isModeUpdated = false;
    // Skip finish operation when going to emergency
    if (mode != MODE_EMERGENCY)
    {
      strategyMethods[2][prevMode](); // finish any operations for prevMode here
    }

    strategyMethods[0][mode](); // init new strategy according to the new mode value
  }
}

// Tries set the mode and isModeUpdated flag
boolean SetMode(byte newMode)
{
  if (newMode < MODES_MAX)
  {
    prevMode = mode;
    mode = newMode;
    isModeUpdated = true;
    EEPROM.write(MEMADDR_LASTMODE, mode);
    DEBUG_PRINT("Mode is set to: ");
    DEBUG_PRINTLN(mode);
    return true;
  }
  return false;
}
