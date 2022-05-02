/*
  GeoRover CanBUS communication protocols for openCAN communication (Motors)
  - Library Dependency: https://github.com/autowp/arduino-mcp2515

  Mads Rosenhøj Jeppesen
  Aarhus University
  2022
*/

#include <mcp2515.h>

struct can_frame canMsg;
struct can_frame canTestMsg;

long CanBusTxLast;

int canTestState                   = 0;
unsigned long millisCanTestStart   = 0;
unsigned long millisLastCanPrint   = 0;
unsigned long maximumtimeSinceLast = 0;

MCP2515 mcp2515(PO_SPISS_CANBUS);

bool InitializeCanBus() {
  int err;

  // digitalWrite(PO_MOTOR_EN, true);
  // delay(5);

  SPI.begin();  // Begins SPI communication
  mcp2515.reset();

  err = mcp2515.setBitrate(CANBBUS_SPEED, MCP_8MHZ);
  if (err != MCP2515::ERROR_OK) return false;

  err = mcp2515.setNormalMode();
  if (err != MCP2515::ERROR_OK) return false;

  motorRight.ResetCanStatus();
  motorLeft.SetCanTxStatus();

  if (!MotorCycle()) {
    SystemDisable(MODULE_MOTORS);
    return false;
  }

  return true;
}

void TerminateCanBus() {
  SPI.end();
  digitalWrite(PO_MOTOR_EN, false);
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
  if (millis() - millisLastCanPrint < CANBUS_PRINT_PERIOD) {
    return;
  }
  millisLastCanPrint = millis();

  bool motorLeftStatus  = motorLeft.PrintStatus() == GemMotor::ERROR_OK;
  bool motorRightStatus = motorRight.PrintStatus() == GemMotor::ERROR_OK;

  // DEBUG_PRINT("Maximum canbus Process period: ");
  // DEBUG_PRINTLN(maximumtimeSinceLast);
  maximumtimeSinceLast = 0;

  // if (!(motorLeftStatus && motorRightStatus)) {
  // SystemDisable(MODULE_MOTORS);
  // SystemEnable(MODULE_MOTORS);
  // }
  // printChannels();
  // Serial.println(F("-------------------------------"));
}

unsigned long timeSinceLastCanProcess = 0;
// Process CANbus commands, returns false if MCP2515 throws error
bool CanBusProcess() {
  timeSinceLastCanProcess = millis() - CanBusTxLast;
  while (timeSinceLastCanProcess > CANBUS_TX_PERIOD) {
    maximumtimeSinceLast = max(timeSinceLastCanProcess, maximumtimeSinceLast);

    // Check motor status
    if (!MotorStatusLeft() && MotorStatusRight() && SBusStatus()) {
      MotorUpdate(0, 0);  // Avoid running single motor if other motor reports error
      // return false;
    }

    int err;
    if (motorLeft.GetCanTxStatus()) {
      err = mcp2515.sendMessage(motorLeft.GetCanMsg());
      // DEBUG_PRINTLN(F("Send - Left"));
      if (err != MCP2515::ERROR_OK) {
        DEBUG_PRINT(F("Can Error Left: "));
        DEBUG_PRINTLN(err);
        return false;
      }
    } else if (motorRight.GetCanTxStatus()) {
      err = mcp2515.sendMessage(motorRight.GetCanMsg());
      // DEBUG_PRINTLN(F("Send - Right"));
      if (err != MCP2515::ERROR_OK) {
        DEBUG_PRINT(F("Can Error Right: "));
        DEBUG_PRINTLN(err);
        return false;
      }
    }

    if (motorLeft.GetCanRxStatus() || motorRight.GetCanRxStatus()) {
      if (motorLeft.CheckCanRxTimeout() == GemMotor::CAN_ERROR_TIMEOUT) {
        motorRight.SetCanTxStatus();
        // DEBUG_PRINTLN(F("Timeout - Left"));
      }
      if (motorRight.CheckCanRxTimeout() == GemMotor::CAN_ERROR_TIMEOUT) {
        motorLeft.SetCanTxStatus();
        // DEBUG_PRINTLN(F("Timeout - Right"));
      }
      if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
        ParseData();
      }
    }
    return true;
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
    // DEBUG_PRINTLN(F("Read - Left"));
    status = motorLeft.ParseCanMsg(canMsg);
    motorRight.SetCanTxStatus();  // Indicate motor Right to send next msg
  }

  // Check ID against right motor
  else if (canMsg.can_id == CANBUS_RX_MOTOR_RIGHT || canMsg.can_id == CANBUS_RX_MOTOR_RIGHT + 1 || canMsg.can_id == CANBUS_RX_MOTOR_RIGHT + 2 || canMsg.can_id == CANBUS_RX_MOTOR_RIGHT + 3) {
    // DEBUG_PRINTLN(F("Read - Right"));
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
      SetStatus(MODULE_CANBUS, CanBusStatus());  // TODO: Update to reflect Motor CAN Errors
    default:
      break;
  }

  return testDone;
}

// Stream all data in raw HEX
void StreamData() {
  DEBUG_PRINT2(canMsg.can_id, HEX);  // print ID
  DEBUG_PRINT(F(" "));
  DEBUG_PRINT2(canMsg.can_dlc, HEX);  // print DLC
  DEBUG_PRINT(F(" "));

  for (int i = 0; i < canMsg.can_dlc; i++) {  // print the data
    DEBUG_PRINT2(canMsg.data[i], HEX);
    DEBUG_PRINT(F(" "));
  }

  DEBUG_PRINTLN();
}

// void ParseCanMsg(bool motor) {
//   if (canMsg.can_id == 0x64) {
//     int control_value = (int)((canMsg.data[1] << 8) | canMsg.data[0]);
//     int motor_state   = (int)(canMsg.data[3] >> 6);
//     int rpm           = (int)((canMsg.data[6] << 8) | canMsg.data[5]) / 10.0f;
//     int temperature   = (int)canMsg.data[7];

//     Serial.print(F("Control value: "));
//     Serial.print(control_value);
//     Serial.print(F("\t Motor State: "));
//     Serial.print(motor_state);
//     Serial.print(F("\t rpm: "));
//     Serial.print(rpm);
//     Serial.print(F("\t Temperature: "));
//     Serial.println(temperature);
//   }

//   if (canMsg.can_id == 0x65) {
//     // inv peak current
//     // Motor power
//   }

//   // Warning
//   if (canMsg.can_id == 0x66) {
//     bool warning[64];

//     for (size_t i = 0; i < 8; i++) {
//       for (size_t j = 0; j < 8; j++) {
//         warning[i * 8 + j] = canMsg.data[i] >> j & 1;
//       }
//     }
//     Serial.print(F("Warning: "));
//     for (size_t i = 0; i < 64; i++) {
//       if (warning[i] == 1) {
//         Serial.print(i);
//         Serial.print(F("\t"));
//       }
//     }

//     // for (int i = 0; i<canMsg.can_dlc; i++)  {  // print the data
//     //   Serial.print(canMsg.data[i],HEX);
//     //   Serial.print(" ");
//     // }
//     Serial.println();
//   }

//   // Error
//   if (canMsg.can_id == 0x67) {
//     bool error[64];

//     for (size_t i = 0; i < 8; i++) {
//       for (size_t j = 0; j < 8; j++) {
//         error[i * 8 + j] = canMsg.data[i] >> j & 1;
//       }
//     }
//     Serial.print(F("Error: "));
//     for (size_t i = 0; i < 64; i++) {
//       if (error[i] == 1) {
//         Serial.print(i);
//         Serial.print(F("\t"));
//       }
//     }

//     // for (int i = 0; i<canMsg.can_dlc; i++)  {  // print the data
//     //   Serial.print(canMsg.data[i],HEX);
//     //   Serial.print(" ");
//     // }

//     Serial.println();
//   }
// }
