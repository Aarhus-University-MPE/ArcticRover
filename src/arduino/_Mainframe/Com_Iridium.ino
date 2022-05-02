/*
  GeoRover Iridium communication protocols for long range communication
  - Library Dependency: https://github.com/sparkfun/SparkFun_IridiumSBD_I2C_Arduino_Library

  Mads Rosenhøj Jeppesen
  Aarhus University
  2021
*/

#include <IridiumSBD.h>
#define DIAGNOSTICS false  // Change this to see diagnostics

IridiumSBD modem(COM_SERIAL_IRID);

char sendBuffer[200];
uint8_t receiveBuffer[200];
size_t sendBuferSize;
size_t bufferSize = 200;
int signalQuality = -1;
bool iridiumTxStatus;
unsigned long millisLastIridiumProcess;

bool InitializeIridium() {
  bool status = true;
  COM_SERIAL_IRID.begin(IRID_BAUDRATE);

  modem.adjustATTimeout(IRID_ATT_TIMEOUT);
  modem.adjustStartupTimeout(IRID_START_TIMEOUT);
  digitalWrite(PO_POWER_IRIDIUM, HIGH);

  // modem.setPowerProfile(IridiumSBD::USB_POWER_PROFILE);

  status = (modem.begin() == ISBD_SUCCESS);
  return status;
}

void TerminateIridium() {
  digitalWrite(PO_POWER_IRIDIUM, LOW);
  COM_SERIAL_IRID.end();
}

bool IridiumStatus() {
  return modem.isConnected();
}

void IridiumProcess() {
  if (!GetStatus(MODULE_IRIDIUM)) {
    return;
  }

  if (signalQuality < 1) {
    return;
  }

  if (!(millis() - millisLastIridiumProcess > IRID_PROCESS_PERIOD)) {
    return;
  }

  millisLastIridiumProcess = millis();
  IridiumReceive();
  IridiumSend();
}

// Check modem for waiting messages
void IridiumReceive() {
  if (!modem.getWaitingMessageCount() > 0) {
    return;
  }

  int err;

  err = modem.sendReceiveSBDText(NULL, receiveBuffer, bufferSize);
  if (err != ISBD_SUCCESS) {
    DEBUG_PRINT(F("Receive msg failed, Error: "));
    DEBUG_PRINTLN(err);
  } else {
    for (int i = 0; i < bufferSize; i++) {
      Serial.print(receiveBuffer[i], HEX);
      if (isprint(receiveBuffer[i])) {
        Serial.print(F("("));
        Serial.write(receiveBuffer[i]);
        Serial.print(F(")"));
      }
      Serial.print(F(" "));
    }
    Serial.println();
  }
  // Clear the Mobile Originated message buffer to avoid re-sending the message during subsequent loops
  Serial.println(F("Clearing the MO buffer."));
  err = modem.clearBuffers(ISBD_CLEAR_MO);  // Clear MO buffer
  if (err != ISBD_SUCCESS) {
    Serial.print(F("clearBuffers failed, Error: "));
    Serial.println(err);
  }
}

// Send iridium message awaiting to be sent
void IridiumSend() {
  if (iridiumTxStatus) {
    DEBUG_PRINT("Sending test message... ");
    int err = modem.sendReceiveSBDText(sendBuffer, receiveBuffer, bufferSize);
    if (err == ISBD_SUCCESS) {
      iridiumTxStatus = false;
      DEBUG_PRINTLN("Success!");
    } else {
      DEBUG_PRINT("Error: ");
      DEBUG_PRINTLN(err);
    }
  }
}

// TODO: Fill send buffer with specified data
void PopulateSendBuffer() {
  // fill send buffer
  sendBuffer[0] = 'H';
  sendBuffer[1] = 'e';
  sendBuffer[2] = 'l';
  sendBuffer[3] = 'l';
  sendBuffer[4] = 'o';
  sendBuffer[5] = '!';
  sendBuffer[6] = '\r';
  sendBuffer[7] = '\n';
  sendBuferSize = 8;
}

// Full Iridium Test
bool IridiumTest() {
  bool status = true;

  int err;
  // err = modem.getSignalQuality(signalQuality);
  // if (err != ISBD_SUCCESS) {
  //   status = false;
  // }

  // DEBUG_PRINT(F("Iridium: "));
  // if (status) {
  //   DEBUG_PRINT(F("Signal Quality: "));
  //   DEBUG_PRINTLN(signalQuality);
  // } else {
  //   DEBUG_PRINTLN(F("ERROR"));
  // }

  // Example: Print the firmware revision
  // char version[12];
  // err = modem.getFirmwareVersion(version, sizeof(version));
  // if (err != ISBD_SUCCESS) {
  //   DEBUG_PRINT(F("Firmware Version failed: Error "));
  //   DEBUG_PRINTLN(err);
  //   status = false;
  // }
  // DEBUG_PRINT(F("Firmware Version is "));
  // DEBUG_PRINT(version);
  // DEBUG_PRINTLN(F("."));

  // // Example: Print the IMEI
  // char IMEI[16];
  // err = modem.getIMEI(IMEI, sizeof(IMEI));
  // if (err != ISBD_SUCCESS) {
  //   DEBUG_PRINT(F("getIMEI failed: Error "));
  //   DEBUG_PRINTLN(err);
  //   status = false;
  // }
  // DEBUG_PRINT(F("IMEI is "));
  // DEBUG_PRINT(IMEI);
  // DEBUG_PRINTLN(F("."));

  // Example: Test the signal quality.
  // This returns a number between 0 and 5.
  // 2 or better is preferred.
  err = modem.getSignalQuality(signalQuality);
  if (err != ISBD_SUCCESS) {
    DEBUG_PRINT(F("SignalQuality failed: Error "));
    DEBUG_PRINTLN(err);
    status = false;
  }

  DEBUG_PRINT(F("On a scale of 0 to 5, signal quality is currently "));
  DEBUG_PRINT(signalQuality);
  DEBUG_PRINTLN(F("."));

  DEBUG_PRINTLINE();
  if (signalQuality <= 0) {
    return status;
  }
  sendBuffer[0]   = (uint8_t)BatteryLevel();
  iridiumTxStatus = true;
  IridiumSend();

  return status;
}

#if DIAGNOSTICS
void ISBDConsoleCallback(IridiumSBD *device, char c) {
  Serial.write(c);
}

void ISBDDiagsCallback(IridiumSBD *device, char c) {
  Serial.write(c);
}
#endif
