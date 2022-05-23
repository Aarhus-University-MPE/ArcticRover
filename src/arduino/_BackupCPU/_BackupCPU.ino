
/*
  GeoRover Backup CPU
  Detects and sends heartbeat to mainframe of GeoRover.
  Resets system if no heartbeat detected within limit.

  Mads Rosenhøj Jeppesen
  Aarhus University
  2021s
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

  // Debug
  DBG_ONLY(initializeDebugComm());
  DEBUG_PRINT("Debug mode. Entered Setup... ");

  // Setup finished
  DEBUG_PRINTLN("Setup complete.");
}

// ------------------------------------------------------------ //
//                          MAIN LOOP                           //
// ------------------------------------------------------------ //
void loop() {
  HeartBeat();

  DBG_ONLY(recvWithStartEndMarkers());
}
