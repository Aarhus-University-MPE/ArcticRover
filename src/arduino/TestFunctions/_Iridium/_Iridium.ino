/*
  GeoRover Iridium communication protocols for long range communication
  using: https://github.com/sparkfun/SparkFun_IridiumSBD_I2C_Arduino_Library

  Mads Rosenhøj Jeppesen
  
  Aarhus University
  2021
*/

#include <IridiumSBD.h>
#define DIAGNOSTICS        false  // Change this to see diagnostics
#define PO_POWER_IRIDIUM   46
#define COM_SERIAL_IRID    Serial2
#define IRID_ATT_TIMEOUT   2
#define IRID_BAUDRATE      19200
#define IRID_START_TIMEOUT 2

IridiumSBD modem(COM_SERIAL_IRID);

uint8_t buffer[200];
int signalQuality = -1;

void setup() {
  Serial.begin(115200);
  pinMode(PO_POWER_IRIDIUM, OUTPUT);
  digitalWrite(PO_POWER_IRIDIUM, HIGH);

  InitializeIridium();
}

void loop() {
}

void InitializeIridium() {
  int signalQuality = -1;
  int err;
  
  // Start the serial port connected to the satellite modem
  COM_SERIAL_IRID.begin(IRID_BAUDRATE);

  // Begin satellite modem operation
  Serial.println(F("Starting modem..."));
  err = modem.begin();
  if (err != ISBD_SUCCESS) {
    Serial.print(F("Begin failed: error "));
    Serial.println(err);
    if (err == ISBD_NO_MODEM_DETECTED)
      Serial.println(F("No modem detected: check wiring."));
    return;
  }

  // Get the IMEI
  char IMEI[16];
  err = modem.getIMEI(IMEI, sizeof(IMEI));
  if (err != ISBD_SUCCESS) {
    Serial.print(F("getIMEI failed: error "));
    Serial.println(err);
    return;
  }
  Serial.print(F("IMEI is "));
  Serial.print(IMEI);
  Serial.println(F("."));
}

#if DIAGNOSTICS
void ISBDConsoleCallback(IridiumSBD *device, char c) {
  Serial.write(c);
}

void ISBDDiagsCallback(IridiumSBD *device, char c) {
  Serial.write(c);
}
#endif
