/*
  GeoRover Backup CPU
  Detects and sends heartbeat to mainframe of GeoRover.
  Resets system if no heartbeat detected within limit.

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
  DEBUG_PRINT("Debug mode. Entered Setup... ");

  // System initialization
  InitAllPins();

  // Setup finished
  DEBUG_PRINTLN("Setup complete.");

  delay(1000);
}

// ------------------------------------------------------------ //
//                          MAIN LOOP                           //
// ------------------------------------------------------------ //
void loop()
{
  HeartBeat();

  recvWithStartEndMarkers();

  delay(100);
}
