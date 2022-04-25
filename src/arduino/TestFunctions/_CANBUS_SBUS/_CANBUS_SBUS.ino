#include <SBUS.h>
#include <SPI.h>
#include <mcp2515.h>

#include "src/GemMotor.h"
#include "src/_constants.h"
#include "src/_pinout.h"

const int maxVelocity = 15;

struct can_frame canMsg;
MCP2515 mcp2515(PO_SPISS_CANBUS);
SBUS sbus(Serial3);

const byte numChars = 32;
char receivedCMD[numChars];
unsigned long lastMillisPost;
bool active;
bool status;

GemMotor motorLeft  = GemMotor(CANBUS_TX_MOTOR_LEFT, CANBUS_RX_MOTOR_LEFT);
GemMotor motorRight = GemMotor(CANBUS_TX_MOTOR_RIGHT, CANBUS_RX_MOTOR_RIGHT);

void setup() {
  pinMode(PO_MOTOR_EN, OUTPUT);
  pinMode(PO_MOTOR_EN_POWER, OUTPUT);
  digitalWrite(PO_MOTOR_EN, true);
  digitalWrite(PO_MOTOR_EN_POWER, true);

  pinMode(PO_POWER_MOTOR, OUTPUT);
  digitalWrite(PO_POWER_MOTOR, LOW);
  pinMode(PO_POWER_RF, OUTPUT);

  // Status LED/Sound
  pinMode(PO_LED_STATUS_RED, OUTPUT);
  pinMode(PO_LED_STATUS_YEL, OUTPUT);
  pinMode(PO_LED_STATUS_GRN, OUTPUT);

  digitalWrite(PO_LED_STATUS_RED, LOW);
  digitalWrite(PO_LED_STATUS_YEL, LOW);
  digitalWrite(PO_LED_STATUS_GRN, LOW);

  Serial.begin(115200);
  SPI.begin();

  InitializeSBUS();

  mcp2515.reset();
  Serial.print("setBitrate: ");
  Serial.println(mcp2515.setBitrate(CAN_125KBPS, MCP_8MHZ));

  Serial.print("setNormalMode: ");
  Serial.println(mcp2515.setNormalMode());

  motorLeft.Update(0);

  motorLeft.SetCanTxStatus();
  motorRight.ResetCanStatus();
}

void loop() {
  recvWithStartEndMarkers();

  if (!active) {
    LedSet(BINARY_CODE_LED_YEL);
    digitalWrite(PO_POWER_MOTOR, LOW);
    return;
  }

  digitalWrite(PO_POWER_MOTOR, HIGH);

  CanBusProcess();
  sbus.process();

  ProcessSbus();

  if (millis() - lastMillisPost > 1000) {
    lastMillisPost = millis();

    // bool motorLeftStatus  = motorLeft.PrintStatus();
    bool motorRightStatus = motorRight.PrintStatus();

    // status = motorLeftStatus || motorRightStatus;

    // printChannels();
    Serial.println("-------------------------------");
  }

  if (status)
    LedSet(BINARY_CODE_LED_GRN);
  else
    LedSet(BINARY_CODE_LED_RED);
}

void ProcessSbus() {
  float throttle1 = getChannelFloatFull(1);
  float throttle2 = getChannelFloat(3);

  int dir;
  float speed;

  if (getChannel(6) < 200) {
    motorLeft.Update(0);
    motorRight.Update(0);
    return;
  }

  int gear = getChannel(5);
  if (throttle1 > CONTROLLER_DEADZONE_FLOAT) {
    if (gear < 50) {
      dir = -1;
    } else if (gear > 200) {
      dir = 1;
    } else {
      dir = 0;
    }
    speed = throttle1 * maxVelocity * dir;
  } else if (abs(throttle2) > CONTROLLER_DEADZONE_FLOAT && gear < REMOTE_CHANNEL_HIGH && gear > REMOTE_CHANNEL_LOW) {
    speed = throttle2 * maxVelocity / 2;
  } else {
    speed = 0;
  }

  motorLeft.Update(speed);
  motorRight.Update(speed);
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
    }
  } else if (motorRight.GetCanTxStatus()) {
    err = mcp2515.sendMessage(motorRight.GetCanMsg());
    if (err != MCP2515::ERROR_OK) {
    }
  }

  if (motorLeft.GetCanRxStatus() || motorRight.GetCanRxStatus()) {
    if (motorLeft.CheckCanRxTimeout() == GemMotor::CAN_ERROR_TIMEOUT) {
      motorRight.SetCanTxStatus();
    }
    if (motorRight.CheckCanRxTimeout() == GemMotor::CAN_ERROR_TIMEOUT) {
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
    status = motorLeft.ParseCanMsg(canMsg);
    motorRight.SetCanTxStatus();  // Indicate motor Right to send next msg
  } else if (canMsg.can_id == CANBUS_RX_MOTOR_RIGHT || canMsg.can_id == CANBUS_RX_MOTOR_RIGHT + 1 || canMsg.can_id == CANBUS_RX_MOTOR_RIGHT + 2 || canMsg.can_id == CANBUS_RX_MOTOR_RIGHT + 3) {
    status = motorRight.ParseCanMsg(canMsg);
    motorLeft.SetCanTxStatus();  // Indicate motor Right to send next msg
  }
}

// Receive Commands
void recvWithStartEndMarkers() {
  static boolean recvInProgress = false;
  static byte ndx               = 0;
  char startMarker              = '<';
  char endMarker                = '>';
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
        recvInProgress   = false;
        ndx              = 0;
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
      motorRight.Update(0);
      active = true;
      break;
    case 'F':
      motorLeft.Update(5);
      motorRight.Update(5);
      active = true;
      break;
    case 'B':
      motorLeft.Update(-5);
      motorRight.Update(-5);
      active = true;
      break;
    case 'A':
      active = true;
      break;
    case 'M':
      parseCommandMove();
      break;
    case '\0':
      break;
    default:
      Serial.println("NACK");
      break;
  }
}

void parseCommandMove() {
  char *velocityPtr          = receivedCMD + 1;
  char veloctyChar[numChars] = {0};

  strcpy(veloctyChar, velocityPtr);
  int velocity = atoi(veloctyChar);

  active = (velocity != 0);

  Serial.print(F("Setting velocity to: "));
  Serial.println(velocity);

  motorRight.Update(velocity);
  motorLeft.Update(velocity);
}

// Initialize RF Communication
bool InitializeSBUS() {
  digitalWrite(PO_POWER_RF, HIGH);
  Serial.println("Initializing Short Range Communication... ");
  sbus.begin(false);
  bool status = Serial3;
  if (status) {
    Serial.println("Initialized.");
  } else {
    Serial.println("Failed.");
  }
  return status;
}

// Scale SBUS channel value from range [0, 256] to [-1, 1]
float getChannelFloat(int channel) {
  int value = getChannel(channel);

  float valueFloat = value / 128.0 - 1;

  return valueFloat;
}

// Scale SBUS channel value from range [0, 256] to [-1, 1]
float getChannelFloatFull(int channel) {
  int value = getChannel(channel);

  float valueFloat = value / 256.0;

  return valueFloat;
}

const int minChannel = -83;
const int maxChannel = 83;
// Scale the S.BUS channel values into the range [0, 256]
int getChannel(int channel) {
  int value = sbus.getNormalizedChannel(channel);
  if (value < minChannel) {
    return 256 / 2;
  }

  float result = value;

  result -= minChannel;
  result /= (maxChannel - minChannel);
  result *= 256;

  return (int)result;
}

void printChannels() {
  for (int i = 1; i < 8; i++) {
    Serial.print("CH ");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(getChannelFloat(i));
    Serial.print("\t");
  }
  Serial.println();

  Serial.print("Good frames: ");
  Serial.print(sbus.getGoodFrames());

  Serial.print(" \tData loss on connection: ");
  Serial.print(sbus.getFrameLoss());
  Serial.print("% \t");

  Serial.print("Time diff: ");
  Serial.println(millis() - (unsigned long)sbus.getLastTime());
}

byte currentLedStatus = B000;
// Sets current LED Status from byte
// [100] [RED - -]
// [010] [- YEL -]
// [001] [- - GRN]
void LedSet(byte color) {
  // Do nothing if arguments are the same
  if (color == currentLedStatus)
    return;

  digitalWrite(PO_LED_STATUS_GRN, (BINARY_CODE_LED_GRN & color) > 0);
  digitalWrite(PO_LED_STATUS_YEL, (BINARY_CODE_LED_YEL & color) > 0);
  digitalWrite(PO_LED_STATUS_RED, (BINARY_CODE_LED_RED & color) > 0);

  currentLedStatus = color;
}