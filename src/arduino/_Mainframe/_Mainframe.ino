/*
  GeoRover mainframe - Manages primary systems as the Rover backbone.

  Mads Rosenhøj Jeppesen
  Aarhus University
  2022
*/

#include "_constants.h"
#include "_pinout.h"
#include "_shared.h"

// ------------------------------------------------------------ //
//                            SETUP                             //
// ------------------------------------------------------------ //
void setup() {
  // System initialization
  InitAllPins();
  InitButtons();
  InitStatusLed();

  // Debug
  DBG_ONLY(initializeDebugComm());

  // Clear System status flags
  SetStatus(false);

  // Enable all primary systems
  SystemEnablePrimary();

  // Strategy initialization
  InitStrategyMethods();
  InitMode();
}

// ------------------------------------------------------------ //
//                          MAIN LOOP                           //
// ------------------------------------------------------------ //
void loop() {
  // Update new mode based on input
  ModeUpdater();

  // Primary mode function
  strategyMethods[1][mode]();

  // System Heartbeat (Backup-CPU & Blackbox)
  HeartBeat();

  // Send/Read Iridium messages
  IridiumProcess();

  // Control Heating Element
  // HeatingProcess(); // TODO: Verify heating process works before enabling this

  // Receive Serial Commands (DEBUG)
  DBG_ONLY(recvWithStartEndMarkers());
  DBG_ONLY(PerformCommand());
}
