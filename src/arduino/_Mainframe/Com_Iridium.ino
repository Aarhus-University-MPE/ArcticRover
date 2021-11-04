/*
  GeoRover Iridium communication protocols for long range communication

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021
*/

#include <IridiumSBD.h>

IridiumSBD modem(COM_SERIAL_IRID);

uint8_t buffer[200];
int signalQuality = -1;

bool InitializeIridium(){
  COM_SERIAL_IRID.begin(IRID_BAUDRATE);
  
  modem.adjustStartupTimeout(IRID_START_TIMEOUT);
  modem.adjustATTimeout(IRID_ATT_TIMEOUT);

  bool status = !(modem.begin() != ISBD_SUCCESS);
  if(status){
    int err = modem.getSignalQuality(signalQuality);  
    if(err != 0){
      status = false;
    }
  }

  return status;
}

void TerminateIridium(){
  COM_SERIAL_IRID.end();
}

bool IridiumStatus(){
  return modem.isConnected();
}

bool IridiumTest(){
  bool status = true;

  int err = modem.getSignalQuality(signalQuality);  
  if(err != 0){
      status = false;
  }

  DEBUG_PRINT("Iridium: ");
  if(status){
    DEBUG_PRINT("Signal Quality: ");
    DEBUG_PRINTLN(signalQuality);
  }
  else{
    DEBUG_PRINTLN("ERROR");
  }
  return status;
}