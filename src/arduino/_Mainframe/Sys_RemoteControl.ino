// Initialize Remote Control Variables
void RemoteControlInitialize() {
  remoteActive = false;
  remoteStart  = false;
}

// Primary Remote Controll Process Loop
void RemoteControlProcess() {
  if (!remoteActive) {
    SystemDisable();
    return;
  }

  if (remoteStart) {
    if (!RemoteControlStart()) return;
  }

  if (!SystemCheckMode(MODE_REMOTECONTROL)) {
    RemoteControlHalt();
    return;
  }

  // Read RF signal
  if (!SBusProcess()) {
    // RemoteControlHalt();
    // return;
  }
  // SBusPrint();

  // Transmit via CAN
  CanBusProcess();
  // if (!CanBusProcess()) {
  //   RemoteControlHalt();
  //   return;
  // }
  CanBusPrint();
}

// Start Remote Control Process, by enabling modules
bool RemoteControlStart() {
  remoteStart = false;
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN(F("Starting Remote Control"));
  DEBUG_PRINTLINE();
  if (!SystemEnableMode()) {
    DEBUG_PRINTLINE();
    DEBUG_PRINTLN(F("Module Start Error"));
    DEBUG_PRINTLINE();
    RemoteControlHalt();
    return false;
  }
  return true;
}

void RemoteControlHalt() {
  remoteActive = false;
  SystemDisable();
  StatusHaltLed(LED_SIGNAL_ERROR);
}
