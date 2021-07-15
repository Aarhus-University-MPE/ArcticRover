/*
  GeoRover mainframe - Manages primary systems as the Rover backbone.

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021s
*/

#include "_constants.h"
#include "_shared.h"
#include "_pinout.h"


// ------------------------------------------------------------ //
//                            SETUP                             //
// ------------------------------------------------------------ //
void setup()
{
  // Debug
  DBG_ONLY(initializeDebugComm());
  DEBUG_PRINT("Debug mode. Entered setup...");

  // System initialization
  InitAllPins();
  InitButtons();
  InitStatusLed();

  // Strategy initialization
  InitMode();
  InitStrategyMethods();

  // Setup finished
  LedBlinkDoubleShort(BINARY_CODE_LED_GRN);
  DEBUG_PRINTLN("Setup complete.");
}

// ------------------------------------------------------------ //
//                          MAIN LOOP                           //
// ------------------------------------------------------------ //
void loop()
{
  ModeUpdater();

  strategyMethods[1][mode]();

  HeartBeat();

  recvWithStartEndMarkers();
}
