/*
  GeoRover CanBUS communication protocols for openCAN communication (Motors)
  https://github.com/autowp/arduino-mcp2515

  Mads Rosenhøj Jeppesen
  Aarhus University
  2022
*/

#include <mcp2515.h>

struct can_frame canMsg;
struct can_frame canTestMsg;

long CanBusTxLast;

int canTestState        = 0;
long millisCanTestStart = 0;
long millisLastCanPrint = 0;

MCP2515 mcp2515(PO_SPISS_CANBUS);

bool InitializeCanBus() {
  int err;
  SPI.begin();  // Begins SPI communication
  mcp2515.reset();

  err = mcp2515.setBitrate(CANBBUS_SPEED, MCP_8MHZ);
  if (err != MCP2515::ERROR_OK) return false;

  err = mcp2515.setNormalMode();
  if (err != MCP2515::ERROR_OK) return false;

  motorRight.ResetCanStatus();
  motorLeft.SetCanTxStatus();

  bool status = mcp2515.checkReceive();
  status      = true;

  return status;
}

void TerminateCanBus() {
  SPI.end();
}

/*
  TX Period:
    MotorL TX -> RX
    ->
    MotorR TX -> RX
    ->
    Repeat
*/

void CanBusPrint() {
  if (millis() - millisLastCanPrint < 1000) {
    return;
  }
  millisLastCanPrint = millis();

  bool motorLeftStatus  = motorLeft.PrintStatus();
  bool motorRightStatus = motorRight.PrintStatus();

  // status = motorLeftStatus || motorRightStatus;

  // printChannels();
  Serial.println(F("-------------------------------"));
}

void CanBusProcess() {
  if (millis() - CanBusTxLast < CANBUS_TX_PERIOD) {
    return;
  }
  int err;

  if (motorLeft.GetCanTxStatus()) {
    err = mcp2515.sendMessage(motorLeft.GetCanMsg());
    if (err == MCP2515::ERROR_OK) {
    } else {
      // Serial.print("Motor Left Error - ");
      // Serial.println(err);
    }
  } else if (motorRight.GetCanTxStatus()) {
    err = mcp2515.sendMessage(motorRight.GetCanMsg());
    if (err == MCP2515::ERROR_OK) {
    } else {
      // Serial.print("Motor Right Error - ");
      // Serial.println(err);
    }
  }

  if (motorLeft.GetCanRxStatus() || motorRight.GetCanRxStatus()) {
    if (motorLeft.CheckCanRxTimeout() == GemMotor::CAN_ERROR_TIMEOUT) motorRight.SetCanTxStatus();
    if (motorRight.CheckCanRxTimeout() == GemMotor::CAN_ERROR_TIMEOUT) motorLeft.SetCanTxStatus();

    if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
      ParseData();
    }
  }
}

bool CanBusStatus() {
  return GetStatus(MODULE_CANBUS);
}

bool ParseData() {
  char buffer[100];
  bool status = false;

  // Check ID against left motor
  if (canMsg.can_id == CANBUS_RX_MOTOR_LEFT || canMsg.can_id == CANBUS_RX_MOTOR_LEFT + 1 || canMsg.can_id == CANBUS_RX_MOTOR_LEFT + 2 || canMsg.can_id == CANBUS_RX_MOTOR_LEFT + 3) {
    status = motorLeft.ParseCanMsg(canMsg);
    motorRight.SetCanTxStatus();  // Indicate motor Right to send next msg
  }

  // Check ID against right motor
  else if (canMsg.can_id == CANBUS_RX_MOTOR_RIGHT || canMsg.can_id == CANBUS_RX_MOTOR_RIGHT + 1 || canMsg.can_id == CANBUS_RX_MOTOR_RIGHT + 2 || canMsg.can_id == CANBUS_RX_MOTOR_RIGHT + 3) {
    status = motorRight.ParseCanMsg(canMsg);
    motorLeft.SetCanTxStatus();  // Indicate motor Left to send next msg
  }

  CanBusTxLast = millis();
  return status;
}

bool CanBusTest() {
  bool testDone = false;

  switch (canTestState) {
    case 0:
      DEBUG_PRINT(F("GNSS feed starting for: "));
      DEBUG_PRINT(SYS_TEST_DURATION_LONG);
      DEBUG_PRINTLN(F(" ms"));
      DEBUG_PRINTLINE();
      DEBUG_PRINTLN(F("------- CAN Read ----------"));
      DEBUG_PRINTLN(F("ID  DLC   DATA"));
      canTestMsg.can_id  = 0x12;
      canTestMsg.can_dlc = 3;
      canTestMsg.data[0] = 0;
      canTestMsg.data[1] = 0;
      canTestMsg.data[2] = 0;
      millisCanTestStart = millis();
      canTestState++;
      break;
    case 1:
      if (millis() - millisLastCanPrint > CANBUS_TX_PERIOD) {
        millisLastCanPrint = millis();
        if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
          StreamData();
          ParseData();
          DEBUG_PRINTLN();
        }
        mcp2515.sendMessage(&canTestMsg);
      }

      if (millis() - millisCanTestStart > SYS_TEST_DURATION_LONG) canTestState++;
      break;
    case 2:
      canTestState = 0;
      testDone     = true;
      SetStatus(MODULE_CANBUS, CanBusStatus());  //<---- Update to reflect Motor CAN Errors
    default:
      break;
  }

  return testDone;
}

// Stream all data in raw HEX
void StreamData() {
  Serial.print(canMsg.can_id, HEX);  // print ID
  Serial.print(F(" "));
  Serial.print(canMsg.can_dlc, HEX);  // print DLC
  Serial.print(F(" "));

  for (int i = 0; i < canMsg.can_dlc; i++) {  // print the data
    Serial.print(canMsg.data[i], HEX);
    Serial.print(F(" "));
  }

  Serial.println();
}

void ParseCanMsg(bool motor) {
  if (canMsg.can_id == 0x64) {
    int control_value = (int)((canMsg.data[1] << 8) | canMsg.data[0]);
    int motor_state   = (int)(canMsg.data[3] >> 6);
    int rpm           = (int)((canMsg.data[6] << 8) | canMsg.data[5]) / 10.0f;
    int temperature   = (int)canMsg.data[7];

    Serial.print(F("Control value: "));
    Serial.print(control_value);
    Serial.print(F("\t Motor State: "));
    Serial.print(motor_state);
    Serial.print(F("\t rpm: "));
    Serial.print(rpm);
    Serial.print(F("\t Temperature: "));
    Serial.println(temperature);
  }

  if (canMsg.can_id == 0x65) {
    // inv peak current
    // Motor power
  }

  // Warning
  if (canMsg.can_id == 0x66) {
    bool warning[64];

    for (size_t i = 0; i < 8; i++) {
      for (size_t j = 0; j < 8; j++) {
        warning[i * 8 + j] = canMsg.data[i] >> j & 1;
      }
    }
    Serial.print(F("Warning: "));
    for (size_t i = 0; i < 64; i++) {
      if (warning[i] == 1) {
        Serial.print(i);
        Serial.print(F("\t"));
      }
    }

    // for (int i = 0; i<canMsg.can_dlc; i++)  {  // print the data
    //   Serial.print(canMsg.data[i],HEX);
    //   Serial.print(" ");
    // }
    Serial.println();
  }

  // Error
  if (canMsg.can_id == 0x67) {
    bool error[64];

    for (size_t i = 0; i < 8; i++) {
      for (size_t j = 0; j < 8; j++) {
        error[i * 8 + j] = canMsg.data[i] >> j & 1;
      }
    }
    Serial.print(F("Error: "));
    for (size_t i = 0; i < 64; i++) {
      if (error[i] == 1) {
        Serial.print(i);
        Serial.print(F("\t"));
      }
    }

    // for (int i = 0; i<canMsg.can_dlc; i++)  {  // print the data
    //   Serial.print(canMsg.data[i],HEX);
    //   Serial.print(" ");
    // }

    Serial.println();
  }
}
