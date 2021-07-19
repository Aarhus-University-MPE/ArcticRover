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
  InitBlackBox();
  SetStatus(false);

  // Debug
  DBG_ONLY(initializeDebugComm());
  DEBUG_PRINTLN("## - Debug mode - ##");

  // System initialization
  InitAllPins();
  InitButtons();
  InitStatusLed();

  // Strategy initialization
  InitStrategyMethods();
  InitMode();

  // Setup finished
  LedBlinkDoubleShort(BINARY_CODE_LED_GRN);
}

// ------------------------------------------------------------ //
//                          MAIN LOOP                           //
// ------------------------------------------------------------ //
void loop()
{
  ModeUpdater();

  strategyMethods[1][mode]();

  HeartBeat();

  DBG_ONLY(recvWithStartEndMarkers());
}
