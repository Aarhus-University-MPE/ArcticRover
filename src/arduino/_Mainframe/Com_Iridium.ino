/*
  GeoRover Iridium communication protocols for long range communication
  using: https://github.com/sparkfun/SparkFun_IridiumSBD_I2C_Arduino_Library

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021
*/

#include <IridiumSBD.h>
#define DIAGNOSTICS false // Change this to see diagnostics

IridiumSBD modem(COM_SERIAL_IRID);

uint8_t buffer[200];
int signalQuality = -1;

bool InitializeIridium() {
  COM_SERIAL_IRID.begin(IRID_BAUDRATE);

  bool status = (modem.begin() == ISBD_SUCCESS);

  return status;
}

void TerminateIridium() {
  COM_SERIAL_IRID.end();
}

bool IridiumStatus() {
  return modem.isConnected();
}

bool IridiumTest() {
  bool status = true;

  int err = modem.getSignalQuality(signalQuality);
  if (err != ISBD_SUCCESS) {
    status = false;
  }

  DEBUG_PRINT(F("Iridium: "));
  if (status) {
    DEBUG_PRINT(F("Signal Quality: "));
    DEBUG_PRINTLN(signalQuality);
  } else {
    DEBUG_PRINTLN(F("ERROR"));
  }

   // Example: Print the firmware revision
  char version[12];
  err = modem.getFirmwareVersion(version, sizeof(version));
  if (err != ISBD_SUCCESS)
  {
     DEBUG_PRINT(F("Firmware Version failed: error "));
     DEBUG_PRINTLN(err);
     status = false;
  }
  DEBUG_PRINT(F("Firmware Version is "));
  DEBUG_PRINT(version);
  DEBUG_PRINTLN(F("."));

  // Example: Print the IMEI
  char IMEI[16];
  err = modem.getIMEI(IMEI, sizeof(IMEI));
  if (err != ISBD_SUCCESS)
  {
     DEBUG_PRINT(F("getIMEI failed: error "));
     DEBUG_PRINTLN(err);
     status = false;
  }
  DEBUG_PRINT(F("IMEI is "));
  DEBUG_PRINT(IMEI);
  DEBUG_PRINTLN(F("."));

  // Example: Test the signal quality.
  // This returns a number between 0 and 5.
  // 2 or better is preferred.
  err = modem.getSignalQuality(signalQuality);
  if (err != ISBD_SUCCESS)
  {
    DEBUG_PRINT(F("SignalQuality failed: error "));
    DEBUG_PRINTLN(err);
     status = false;
  }

  DEBUG_PRINT(F("On a scale of 0 to 5, signal quality is currently "));
  DEBUG_PRINT(signalQuality);
  DEBUG_PRINTLN(F("."));

  DEBUG_PRINTLINE();

  return status;
}


#if DIAGNOSTICS
void ISBDConsoleCallback(IridiumSBD *device, char c)
{
  Serial.write(c);
}

void ISBDDiagsCallback(IridiumSBD *device, char c)
{
  Serial.write(c);
}
#endif
