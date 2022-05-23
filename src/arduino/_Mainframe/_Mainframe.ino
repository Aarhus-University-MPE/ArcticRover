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

  SetStatus(false);

  SystemEnablePrimary();

  // Strategy initialization
  InitStrategyMethods();
  InitMode();
}

// ------------------------------------------------------------ //
//                          MAIN LOOP                           //
// ------------------------------------------------------------ //
void loop() {
  ModeUpdater();

  // Primary mode function
  strategyMethods[1][mode]();

  HeartBeat();

  IridiumProcess();

  DBG_ONLY(recvWithStartEndMarkers());
  DBG_ONLY(PerformCommand());
}
