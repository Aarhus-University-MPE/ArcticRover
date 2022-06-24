/*
  GeoRover Iridium communication protocols for long range communication
  - Sensor:
  - Library Dependency: https://github.com/sparkfun/SparkFun_IridiumSBD_I2C_Arduino_Library

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

uint8_t receiveBuffer[200];
uint8_t sendBuffer[2]        = {0x12, 0x34};
size_t bufferSize            = 200;
size_t bufferSizeSend        = 2;
unsigned long lastMillisSend = 0;

bool msgSent      = false;
int signalQuality = -1;

// Arduino Setup
void setup() {
  Serial.begin(115200);
  pinMode(PO_POWER_IRIDIUM, OUTPUT);
  digitalWrite(PO_POWER_IRIDIUM, HIGH);

  InitializeIridium();
}

// Primary Loop
void loop() {
  if (millis() - lastMillisSend < 1500) return;

  lastMillisSend = millis();

  // Await sufficient signal quality before processing
  if (!IridiumSignalQuality()) {
    Serial.println(F("Waiting for signal..."));
    return;
  }

  // Iridium Process
  IridiumReceive();

  IridiumSend();
}

// Send test message
void IridiumSend() {
  if (msgSent) return;
  int err;

  Serial.print("Sending test message... ");

  // Send message
  err = modem.sendSBDBinary(sendBuffer, bufferSizeSend);
  if (err == ISBD_SUCCESS) {
    msgSent = true;
    Serial.println("Success!");
  } else {
    Serial.print("Error: ");
    Serial.println(err);
  }
}

// Check for new messages
void IridiumReceive() {
  Serial.println(F("Checking Messages"));

  // Check message count
  if (!modem.getWaitingMessageCount() > 0) {
    Serial.println(F("No New messages"));
    return;
  }
  Serial.println(F("New message available"));

  int err;

  // Download new message
  err = modem.sendReceiveSBDText(NULL, receiveBuffer, bufferSize);
  if (err != ISBD_SUCCESS) {
    Serial.print(F("Receive msg failed, Error: "));
    Serial.println(err);
  } else {
    // Print Iridium Buffer
    Serial.print(F("Received message (HEX): \""));
    for (uint8_t i = 0; i < bufferSize; i++) {
      Serial.print(receiveBuffer[i], HEX);
    }
    Serial.println(F("\""));
  }

  // Clear the Mobile Originated message buffer to avoid re-sending the message during subsequent loops
  Serial.println(F("Clearing the MO buffer."));
  err = modem.clearBuffers(ISBD_CLEAR_MO);  // Clear MO buffer
  if (err != ISBD_SUCCESS) {
    Serial.print(F("clearBuffers failed, Error: "));
    Serial.println(err);
  }
}

// Check signal quality
bool IridiumSignalQuality() {
  int err;

  err = modem.getSignalQuality(signalQuality);
  if (err != ISBD_SUCCESS) {
    Serial.print(F("SignalQuality failed: error "));
    Serial.println(err);
  }

  Serial.print(F("On a scale of 0 to 5, signal quality is currently "));
  Serial.print(signalQuality);
  Serial.println(F("."));

  return signalQuality >= 0;
}

// Configure Iridium
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

  // Check signal quality
  IridiumSignalQuality();

  // Configure timeout
  modem.adjustSendReceiveTimeout(1);
}

#if DIAGNOSTICS
void ISBDConsoleCallback(IridiumSBD *device, char c) {
  Serial.write(c);
}

void ISBDDiagsCallback(IridiumSBD *device, char c) {
  Serial.write(c);
}
#endif
