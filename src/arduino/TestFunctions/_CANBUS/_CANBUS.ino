#include <SPI.h>
#include <mcp2515.h>

#include "GemMotor.h"
#include "_constants.h"

#define CANBUS_TX_MOTOR_LEFT 0x12
#define CANBUS_RX_MOTOR_LEFT 0x64
#define PO_SPISS_CANBUS      26
#define PO_MOTOR_EN          34

struct can_frame canMsg;
MCP2515 mcp2515(PO_SPISS_CANBUS);

const byte numChars = 32;
char receivedCMD[numChars];
bool active;

GemMotor motorLeft = GemMotor(CANBUS_TX_MOTOR_LEFT, CANBUS_RX_MOTOR_LEFT);

void setup() {
  pinMode(PO_MOTOR_EN, OUTPUT);
  digitalWrite(PO_MOTOR_EN, true);

  Serial.begin(115200);
  SPI.begin();
  mcp2515.reset();
  Serial.print("setBitrate: ");
  Serial.println(mcp2515.setBitrate(CAN_125KBPS, MCP_8MHZ));

  Serial.print("setNormalMode: ");
  Serial.println(mcp2515.setNormalMode());

  motorLeft.Update(0);
  
  motorLeft.SetCanTxStatus();

  Serial.println("------- CAN Read ----------");
  Serial.println("ID  DLC   DATA");
}

void loop() {
  if(active) CanBusProcess();

  recvWithStartEndMarkers();
}

unsigned long CanBusTxLast;
void CanBusProcess() {
  if (millis() - CanBusTxLast < CANBUS_TX_PERIOD) {
    return;
  }
  int err;

  if (motorLeft.GetCanTxStatus()) {
    err = mcp2515.sendMessage(motorLeft.GetCanMsg());
    if (err != MCP2515::ERROR_OK) {
      // Serial.print("Motor Left Error - ");
      // Serial.println(err);
      motorLeft.SetCanTxStatus();
    }
  }

  if (motorLeft.GetCanRxStatus()) {
    if (motorLeft.CheckCanRxTimeout() == GemMotor::CAN_ERROR_TIMEOUT){
      Serial.println("Motor Left - Timeout");
      motorLeft.SetCanTxStatus();
      }

    if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
      ParseData();
      CanBusTxLast = millis();
    }
  }
}

// Stream all data in raw HEX
void StreamData() {
  Serial.print(canMsg.can_id, HEX);  // print ID
  Serial.print(" ");
  Serial.print(canMsg.can_dlc, HEX);  // print DLC
  Serial.print(" ");

  for (int i = 0; i < canMsg.can_dlc; i++) {  // print the data
    Serial.print(canMsg.data[i], HEX);
    Serial.print(" ");
  }

  Serial.println();
}

void ParseData() {
  char buffer[100];
  bool status = false;

  // Check ID against left motor
  if (canMsg.can_id == CANBUS_RX_MOTOR_LEFT || canMsg.can_id == CANBUS_RX_MOTOR_LEFT + 1 || canMsg.can_id == CANBUS_RX_MOTOR_LEFT + 2 || canMsg.can_id == CANBUS_RX_MOTOR_LEFT + 3) {
    status = motorLeft.ParseCanMsg(canMsg, true);
    motorLeft.SetCanTxStatus();  // Indicate motor Right to send next msg
  }
}


// Receive Commands
void recvWithStartEndMarkers() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;

  while (Serial.available() > 0) {
    rc = Serial.read();

    if (recvInProgress == true) {
      if (rc != endMarker) {
        receivedCMD[ndx] = rc;
        ndx++;
        if (ndx >= numChars) {
          ndx = numChars - 1;
        }
      } else {
        receivedCMD[ndx] = '\0';  // terminate the string
        recvInProgress = false;
        ndx = 0;
        parseCommand();
      }
    }

    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
}

// Parse read Command
void parseCommand() {
  Serial.print("Received command: ");
  Serial.println(receivedCMD[0]);
  
  switch (receivedCMD[0]) {
    case 'S':
      active = false;
      break;
    case 'H':
      motorLeft.Update(0);
      active = true;
      break;
    case 'F':
      motorLeft.Update(5);
      active = true;
      break;
    case 'B':
      motorLeft.Update(-5);
      active = true;
      break;
    case '\0':
      break;
    default:
      Serial.println("NACK");
      break;
  }
}