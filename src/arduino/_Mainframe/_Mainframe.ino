/*
  GeoRover mainframe - Manages primary systems as the Rover backbone.

  Mads Rosenhøj Jepepsen
  Aarhus University
  2022
*/

#include "_constants.h"
#include "_shared.h"
#include "_pinout.h"

// ------------------------------------------------------------ //
//                            SETUP                             //
// ------------------------------------------------------------ //
void setup() {
  // System initialization
  InitAllPins();
  InitButtons();
  InitStatusLed();

  SetStatus(false);

  // Debug
  DBG_ONLY(initializeDebugComm());
  
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

  strategyMethods[1][mode]();

  HeartBeat();

  DBG_ONLY(recvWithStartEndMarkers());
  DBG_ONLY(PerformCommand());
}
