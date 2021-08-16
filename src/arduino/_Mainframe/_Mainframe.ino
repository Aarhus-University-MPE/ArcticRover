/*
  GeoRover mainframe - Manages primary systems as the Rover backbone.

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021s
*/

#include "_constants.h"
#include "_pinout.h"
#include "_shared.h"


// ------------------------------------------------------------ //
//                            SETUP                             //
// ------------------------------------------------------------ //
void setup()
{
  InitBlackBox();
  SetStatus(false);

  // Debug
  DBG_ONLY(initializeDebugComm());

  // System initialization
  InitAllPins();
  delay(50);
  InitButtons();
  delay(50);
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
