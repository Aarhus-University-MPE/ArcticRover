# 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\_Mainframe.ino"
/*

  GeoRover mainframe - Manages primary systems as the Rover backbone.



  Mads Rosenhøj Jeppesen

  Aarhus University

  2022

*/
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\_Mainframe.ino"
# 10 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\_Mainframe.ino" 2
# 11 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\_Mainframe.ino" 2
# 12 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\_Mainframe.ino" 2

// ------------------------------------------------------------ //
//                            SETUP                             //
// ------------------------------------------------------------ //
void setup() {
  // System initialization
  InitAllPins();
  InitButtons();
  InitStatusLed();

  // Debug
  initializeDebugComm();

  SetStatus(false);

  SystemEnablePrimary();

  // Strategy initialization
  InitStrategyMethods();
  InitMode();
}

// ------------------------------------------------------------ //
//                          MAIN LOOP                           //
// ------------------------------------------------------------ //
void loop() {
  ModeUpdater();

  strategyMethods[1][mode]();

  HeartBeat();

  // IridiumProcess();

  recvWithStartEndMarkers();
  PerformCommand();
}
# 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
/*

  GeoRover CanBUS communication protocols for openCAN communication (Motors)

  https://github.com/autowp/arduino-mcp2515



  Mads Rosenhøj Jeppesen

  Aarhus University

  2022

*/
# 12 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
struct can_frame canMsg;
struct can_frame canTestMsg;

long CanBusTxLast;

int canTestState = 0;
long millisCanTestStart = 0;
long millisLastCanPrint = 0;

MCP2515 mcp2515(26 /* 50 (MISO), 51 (MOSI), 52 (SCK)*/);

bool InitializeCanBus() {
  int err;

  digitalWrite(34 /* Enables CAN power, Motor HW Enable and Motor Driver Power*/, true);
  delay(5);

  SPI.begin(); // Begins SPI communication
  mcp2515.reset();

  err = mcp2515.setBitrate(CAN_125KBPS, MCP_8MHZ);
  if (err != MCP2515::ERROR_OK) return false;

  err = mcp2515.setNormalMode();
  if (err != MCP2515::ERROR_OK) return false;

  motorRight.ResetCanStatus();
  motorLeft.SetCanTxStatus();

  bool status = true;

  return status;
}

void TerminateCanBus() {
  SPI.end();
  digitalWrite(34 /* Enables CAN power, Motor HW Enable and Motor Driver Power*/, false);
}

/*

  TX Period:

    MotorL TX -> RX

    ->

    MotorR TX -> RX

    ->

    Repeat

*/
# 60 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
void CanBusPrint() {
  if (millis() - millisLastCanPrint < 1000) {
    return;
  }
  millisLastCanPrint = millis();

  bool motorLeftStatus = motorLeft.PrintStatus();
  bool motorRightStatus = motorRight.PrintStatus();

  // status = motorLeftStatus || motorRightStatus;

  // printChannels();
  Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 72 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
                (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 72 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
                "-------------------------------"
# 72 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
                ); &__c[0];}))
# 72 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
                )));
}

// Process CANbus commands, returns false if MCP2515 throws error
bool CanBusProcess() {
  if (millis() - CanBusTxLast < 10) {
    return true;
  }

  int err;
  if (motorLeft.GetCanTxStatus()) {
    err = mcp2515.sendMessage(motorLeft.GetCanMsg());
    if (err != MCP2515::ERROR_OK) {
      BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 85 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 85 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
     "Can Error Left: "
# 85 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
     ); &__c[0];}))
# 85 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
     ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 85 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 85 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
     "Can Error Left: "
# 85 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
     ); &__c[0];}))
# 85 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
     )));
      BlackBoxAppendln(err); Serial.println(err);
      return false;
    }
  } else if (motorRight.GetCanTxStatus()) {
    err = mcp2515.sendMessage(motorRight.GetCanMsg());
    if (err != MCP2515::ERROR_OK) {
      BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 92 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 92 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
     "Can Error Right: "
# 92 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
     ); &__c[0];}))
# 92 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
     ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 92 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 92 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
     "Can Error Right: "
# 92 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
     ); &__c[0];}))
# 92 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
     )));
      BlackBoxAppendln(err); Serial.println(err);
      return false;
    }
  }

  if (motorLeft.GetCanRxStatus() || motorRight.GetCanRxStatus()) {
    if (motorLeft.CheckCanRxTimeout() == GemMotor::CAN_ERROR_TIMEOUT) motorRight.SetCanTxStatus();
    if (motorRight.CheckCanRxTimeout() == GemMotor::CAN_ERROR_TIMEOUT) motorLeft.SetCanTxStatus();

    if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
      ParseData();
    }
  }
  return true;
}

bool CanBusStatus() {
  return GetStatus(9);
}

bool ParseData() {
  char buffer[100];
  bool status = false;

  // Check ID against left motor
  if (canMsg.can_id == 0x64 || canMsg.can_id == 0x64 + 1 || canMsg.can_id == 0x64 + 2 || canMsg.can_id == 0x64 + 3) {
    status = motorLeft.ParseCanMsg(canMsg);
    motorRight.SetCanTxStatus(); // Indicate motor Right to send next msg
  }

  // Check ID against right motor
  else if (canMsg.can_id == 0x74 || canMsg.can_id == 0x74 + 1 || canMsg.can_id == 0x74 + 2 || canMsg.can_id == 0x74 + 3) {
    status = motorRight.ParseCanMsg(canMsg);
    motorLeft.SetCanTxStatus(); // Indicate motor Left to send next msg
  }

  CanBusTxLast = millis();
  return status;
}

bool CanBusTest() {
  bool testDone = false;

  switch (canTestState) {
    case 0:
      BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 138 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 138 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
     "GNSS feed starting for: "
# 138 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
     ); &__c[0];}))
# 138 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
     ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 138 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 138 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
     "GNSS feed starting for: "
# 138 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
     ); &__c[0];}))
# 138 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
     )));
      BlackBoxAppend(30000); Serial.print(30000);
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 140 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 140 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
     " ms"
# 140 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
     ); &__c[0];}))
# 140 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 140 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 140 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
     " ms"
# 140 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
     ); &__c[0];}))
# 140 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
     )));
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 141 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 141 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
     "------------------------------------"
# 141 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
     ); &__c[0];}))
# 141 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 141 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 141 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
     "------------------------------------"
# 141 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
     ); &__c[0];}))
# 141 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
     )));
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 142 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 142 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
     "------- CAN Read ----------"
# 142 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
     ); &__c[0];}))
# 142 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 142 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 142 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
     "------- CAN Read ----------"
# 142 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
     ); &__c[0];}))
# 142 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
     )));
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 143 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 143 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
     "ID  DLC   DATA"
# 143 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
     ); &__c[0];}))
# 143 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 143 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 143 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
     "ID  DLC   DATA"
# 143 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
     ); &__c[0];}))
# 143 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
     )));
      canTestMsg.can_id = 0x12;
      canTestMsg.can_dlc = 3;
      canTestMsg.data[0] = 0;
      canTestMsg.data[1] = 0;
      canTestMsg.data[2] = 0;
      millisCanTestStart = millis();
      canTestState++;
      break;
    case 1:
      if (millis() - millisLastCanPrint > 10) {
        millisLastCanPrint = millis();
        if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
          StreamData();
          ParseData();
          BlackBoxAppendln(); Serial.println();
        }
        mcp2515.sendMessage(&canTestMsg);
      }

      if (millis() - millisCanTestStart > 30000) canTestState++;
      break;
    case 2:
      canTestState = 0;
      testDone = true;
      SetStatus(9, CanBusStatus()); //<---- Update to reflect Motor CAN Errors
    default:
      break;
  }

  return testDone;
}

// Stream all data in raw HEX
void StreamData() {
  Serial.print(canMsg.can_id, 16); // print ID
  Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 179 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 179 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
              " "
# 179 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
              ); &__c[0];}))
# 179 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
              )));
  Serial.print(canMsg.can_dlc, 16); // print DLC
  Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 181 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 181 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
              " "
# 181 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
              ); &__c[0];}))
# 181 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
              )));

  for (int i = 0; i < canMsg.can_dlc; i++) { // print the data
    Serial.print(canMsg.data[i], 16);
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 185 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
                (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 185 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
                " "
# 185 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
                ); &__c[0];}))
# 185 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
                )));
  }

  Serial.println();
}

void ParseCanMsg(bool motor) {
  if (canMsg.can_id == 0x64) {
    int control_value = (int)((canMsg.data[1] << 8) | canMsg.data[0]);
    int motor_state = (int)(canMsg.data[3] >> 6);
    int rpm = (int)((canMsg.data[6] << 8) | canMsg.data[5]) / 10.0f;
    int temperature = (int)canMsg.data[7];

    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 198 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
                (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 198 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
                "Control value: "
# 198 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
                ); &__c[0];}))
# 198 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
                )));
    Serial.print(control_value);
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 200 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
                (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 200 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
                "\t Motor State: "
# 200 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
                ); &__c[0];}))
# 200 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
                )));
    Serial.print(motor_state);
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 202 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
                (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 202 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
                "\t rpm: "
# 202 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
                ); &__c[0];}))
# 202 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
                )));
    Serial.print(rpm);
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 204 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
                (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 204 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
                "\t Temperature: "
# 204 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
                ); &__c[0];}))
# 204 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
                )));
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
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 222 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
                (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 222 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
                "Warning: "
# 222 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
                ); &__c[0];}))
# 222 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
                )));
    for (size_t i = 0; i < 64; i++) {
      if (warning[i] == 1) {
        Serial.print(i);
        Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 226 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
                    (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 226 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
                    "\t"
# 226 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
                    ); &__c[0];}))
# 226 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
                    )));
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
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 246 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
                (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 246 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
                "Error: "
# 246 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
                ); &__c[0];}))
# 246 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
                )));
    for (size_t i = 0; i < 64; i++) {
      if (error[i] == 1) {
        Serial.print(i);
        Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 250 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
                    (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 250 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
                    "\t"
# 250 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino" 3
                    ); &__c[0];}))
# 250 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_CanBus.ino"
                    )));
      }
    }

    // for (int i = 0; i<canMsg.can_dlc; i++)  {  // print the data
    //   Serial.print(canMsg.data[i],HEX);
    //   Serial.print(" ");
    // }

    Serial.println();
  }
}
# 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
/*

  GeoRover Iridium communication protocols for long range communication

  using: https://github.com/sparkfun/SparkFun_IridiumSBD_I2C_Arduino_Library



  Mads Rosenhøj Jeppesen

  Aarhus University

  2021

*/
# 10 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
# 11 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 2


IridiumSBD modem(Serial2 /* 16 (TX), 17 (RX)*/);

uint8_t buffer[200];
int signalQuality = -1;
bool iridiumTxStatus;
unsigned long millisLastIridiumProcess;

bool InitializeIridium() {
  bool status = true;
  Serial2 /* 16 (TX), 17 (RX)*/.begin(19200);

  // modem.setPowerProfile(IridiumSBD::USB_POWER_PROFILE);

  //bool status = (modem.begin() == ISBD_SUCCESS);
  return status;
}

void TerminateIridium() {
  Serial2 /* 16 (TX), 17 (RX)*/.end();
}

bool IridiumStatus() {
  return modem.isConnected();
}

void IridiumProcess() {
  if (!GetStatus(11)) {
    return;
  }

  if (signalQuality < 1) {
    return;
  }

  if (!(millis() - millisLastIridiumProcess > 900000 /* 15 min*/)) {
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
  size_t bufferSize = sizeof(buffer);

  err = modem.sendReceiveSBDText(
# 65 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3 4
                                __null
# 65 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
                                    , buffer, bufferSize);
  if (err != 0) {
    BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 67 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 67 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
   "Receive msg failed, error: "
# 67 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
   ); &__c[0];}))
# 67 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
   ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 67 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 67 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
   "Receive msg failed, error: "
# 67 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
   ); &__c[0];}))
# 67 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
   )));
    BlackBoxAppendln(err); Serial.println(err);
  } else {
    for (int i = 0; i < bufferSize; i++) {
      Serial.print(buffer[i], 16);
      if (isprint(buffer[i])) {
        Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 73 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
                    (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 73 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
                    "("
# 73 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
                    ); &__c[0];}))
# 73 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
                    )));
        Serial.write(buffer[i]);
        Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 75 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
                    (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 75 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
                    ")"
# 75 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
                    ); &__c[0];}))
# 75 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
                    )));
      }
      Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 77 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
                  (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 77 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
                  " "
# 77 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
                  ); &__c[0];}))
# 77 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
                  )));
    }
    Serial.println();
  }
  // Clear the Mobile Originated message buffer to avoid re-sending the message during subsequent loops
  Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 82 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
                (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 82 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
                "Clearing the MO buffer."
# 82 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
                ); &__c[0];}))
# 82 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
                )));
  err = modem.clearBuffers(0); // Clear MO buffer
  if (err != 0) {
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 85 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
                (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 85 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
                "clearBuffers failed, error: "
# 85 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
                ); &__c[0];}))
# 85 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
                )));
    Serial.println(err);
  }
}

// Send iridium message awaiting to be sent
void IridiumSend() {
}

// Full Iridium Test
bool IridiumTest() {
  bool status = true;

  int err = modem.getSignalQuality(signalQuality);
  if (err != 0) {
    status = false;
  }

  BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 103 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 103 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
 "Iridium: "
# 103 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
 ); &__c[0];}))
# 103 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
 ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 103 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 103 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
 "Iridium: "
# 103 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
 ); &__c[0];}))
# 103 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
 )));
  if (status) {
    BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 105 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 105 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
   "Signal Quality: "
# 105 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
   ); &__c[0];}))
# 105 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
   ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 105 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 105 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
   "Signal Quality: "
# 105 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
   ); &__c[0];}))
# 105 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
   )));
    BlackBoxAppendln(signalQuality); Serial.println(signalQuality);
  } else {
    BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 108 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 108 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
   "ERROR"
# 108 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
   ); &__c[0];}))
# 108 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
   ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 108 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 108 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
   "ERROR"
# 108 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
   ); &__c[0];}))
# 108 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
   )));
  }

  // Example: Print the firmware revision
  char version[12];
  err = modem.getFirmwareVersion(version, sizeof(version));
  if (err != 0) {
    BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 115 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 115 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
   "Firmware Version failed: error "
# 115 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
   ); &__c[0];}))
# 115 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
   ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 115 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 115 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
   "Firmware Version failed: error "
# 115 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
   ); &__c[0];}))
# 115 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
   )));
    BlackBoxAppendln(err); Serial.println(err);
    status = false;
  }
  BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 119 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 119 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
 "Firmware Version is "
# 119 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
 ); &__c[0];}))
# 119 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
 ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 119 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 119 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
 "Firmware Version is "
# 119 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
 ); &__c[0];}))
# 119 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
 )));
  BlackBoxAppend(version); Serial.print(version);
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 121 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 121 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
 "."
# 121 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
 ); &__c[0];}))
# 121 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 121 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 121 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
 "."
# 121 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
 ); &__c[0];}))
# 121 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
 )));

  // Example: Print the IMEI
  char IMEI[16];
  err = modem.getIMEI(IMEI, sizeof(IMEI));
  if (err != 0) {
    BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 127 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 127 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
   "getIMEI failed: error "
# 127 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
   ); &__c[0];}))
# 127 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
   ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 127 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 127 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
   "getIMEI failed: error "
# 127 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
   ); &__c[0];}))
# 127 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
   )));
    BlackBoxAppendln(err); Serial.println(err);
    status = false;
  }
  BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 131 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 131 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
 "IMEI is "
# 131 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
 ); &__c[0];}))
# 131 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
 ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 131 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 131 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
 "IMEI is "
# 131 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
 ); &__c[0];}))
# 131 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
 )));
  BlackBoxAppend(IMEI); Serial.print(IMEI);
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 133 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 133 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
 "."
# 133 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
 ); &__c[0];}))
# 133 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 133 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 133 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
 "."
# 133 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
 ); &__c[0];}))
# 133 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
 )));

  // Example: Test the signal quality.
  // This returns a number between 0 and 5.
  // 2 or better is preferred.
  err = modem.getSignalQuality(signalQuality);
  if (err != 0) {
    BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 140 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 140 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
   "SignalQuality failed: error "
# 140 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
   ); &__c[0];}))
# 140 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
   ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 140 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 140 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
   "SignalQuality failed: error "
# 140 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
   ); &__c[0];}))
# 140 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
   )));
    BlackBoxAppendln(err); Serial.println(err);
    status = false;
  }

  BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 145 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 145 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
 "On a scale of 0 to 5, signal quality is currently "
# 145 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
 ); &__c[0];}))
# 145 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
 ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 145 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 145 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
 "On a scale of 0 to 5, signal quality is currently "
# 145 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
 ); &__c[0];}))
# 145 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
 )));
  BlackBoxAppend(signalQuality); Serial.print(signalQuality);
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 147 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 147 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
 "."
# 147 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
 ); &__c[0];}))
# 147 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 147 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 147 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
 "."
# 147 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
 ); &__c[0];}))
# 147 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
 )));

  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 149 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 149 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
 "------------------------------------"
# 149 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
 ); &__c[0];}))
# 149 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 149 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 149 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
 "------------------------------------"
# 149 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino" 3
 ); &__c[0];}))
# 149 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_Iridium.ino"
 )));

  return status;
}
# 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_SBus.ino"
/*

  GeoRover SBUS communication protocols for RF communication

  using: https://github.com/zendes/SBUS



  Mads Rosenhøj Jeppesen

  Aarhus University

  2021

*/
# 10 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_SBus.ino"
# 11 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_SBus.ino" 2

SBUS sbus(Serial3 /* 15 (RX)*/);
bool sBusStatus;
int sbusTestState;
long millisSbusTestStart;
long millisLastSbusPrint;
long millisLastSBusUpdate;

// Initialize RF Communication
bool InitializeSBUS() {
  digitalWrite(44, 0x1);
  sBusStatus = true;

  sbus.begin(false);

  return Serial3 /* 15 (RX)*/;
}

void TerminateSBUS() {
  Serial3 /* 15 (RX)*/.end();
}

bool SBusStatus() {
  return GetStatus(10);
}

bool SBusTest() {
  bool testDone = false;

  switch (sbusTestState) {
    case 0:
      BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 42 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_SBus.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 42 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_SBus.ino"
     "SBUS feed starting for: "
# 42 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_SBus.ino" 3
     ); &__c[0];}))
# 42 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_SBus.ino"
     ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 42 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_SBus.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 42 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_SBus.ino"
     "SBUS feed starting for: "
# 42 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_SBus.ino" 3
     ); &__c[0];}))
# 42 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_SBus.ino"
     )));
      BlackBoxAppend(10000); Serial.print(10000);
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 44 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_SBus.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 44 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_SBus.ino"
     " ms"
# 44 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_SBus.ino" 3
     ); &__c[0];}))
# 44 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_SBus.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 44 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_SBus.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 44 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_SBus.ino"
     " ms"
# 44 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_SBus.ino" 3
     ); &__c[0];}))
# 44 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_SBus.ino"
     )));
      millisSbusTestStart = millis();
      sbusTestState++;
      break;
    case 1:
      sbus.process();

      if (millis() - millisLastSbusPrint > 100) {
        millisLastSbusPrint = millis();
        printChannels();
      }

      if (millis() - millisSbusTestStart > 10000) sbusTestState++;
      break;
    case 2:
      sbusTestState = 0;
      testDone = true;
      SetStatus(10, sbus.getGoodFrames() > 0);
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 62 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_SBus.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 62 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_SBus.ino"
     "------------------------------------"
# 62 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_SBus.ino" 3
     ); &__c[0];}))
# 62 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_SBus.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 62 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_SBus.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 62 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_SBus.ino"
     "------------------------------------"
# 62 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_SBus.ino" 3
     ); &__c[0];}))
# 62 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_SBus.ino"
     )));
    default:
      break;
  }

  return testDone;
}

// Scale SBUS channel value from range [0, 256] to [-1, 1]
float getChannelFloat(int channel) {
  int value = getChannel(channel);

  float valueFloat = value / 128.0 - 1;

  return valueFloat;
}

const int minChannel = -83;
const int maxChannel = 83;
// Scale the S.BUS channel values into the range [0, 256]
int getChannel(int channel) {
  int value = sbus.getNormalizedChannel(channel);

  // if (value < minChannel) {
  //   minChannel = value;
  // }
  // if (value > maxChannel) {
  //   maxChannel = value;
  // }

  float result = value;

  result -= minChannel;
  result /= (maxChannel - minChannel);
  result *= 256;

  return (int)result;
}

// Scale SBUS channel value from range [0, 256] to [-1, 1]
float getChannelFloatFull(int channel) {
  int value = getChannel(channel);

  float valueFloat = value / 256.0;

  return valueFloat;
}

// Read RF signal and update motors accordingly
bool SBusProcess() {
  sbus.process();

  if (millis() - millisLastSBusUpdate < 100) {
    return sBusStatus;
  }
  millisLastSBusUpdate = millis();

  if (getChannel(6) < 200) { // Enable (SF)
    MotorUpdate(0, 0);
    return sBusStatus;
  }

  SBusController();

  // First timeout error returns true (catches large first read getLastTime)
  if (millis() - (unsigned long)sbus.getLastTime() < 2000 && sBusStatus) {
    sBusStatus = false;
    return true;
  } else {
    sBusStatus = true;
  }

  return sBusStatus;
}

// Reads controller input and navigates motors
void SBusController(){
  float throttle1 = getChannelFloatFull(1); // Left stick Vertical
  float dir = getChannelFloat(2); // Right stick Horisontal
  float throttle2 = getChannelFloat(3); // Right stick Vertical
  int gear = getChannel(5); // Gear select (SA)

  int forwardDir;
  float speed;

  // Primary input (Left Stick)
  if (throttle1 > 0.05) {
    if (gear < 50) {
      speed = throttle1 * -1.0f;
    } else if (gear > 200) {
      speed = throttle1;
    } else {
      speed = 0;
    }
    MotorUpdate(dir, speed);
  }
  // Secondary input (Right Stick)
  else if (((throttle2)>0?(throttle2):-(throttle2)) > 0.05 && gear < 200 && gear > 50) {
    if (throttle2 > 0) {
      speed = throttle2 / 2.0f;
    } else {
      speed = throttle2 / 2.0f;
    }
    MotorUpdateTorque(dir, speed);

  } else {
    MotorUpdate(0, 0);
  }
}

void SBusPrint() {
  if (millis() - millisLastSbusPrint < 1000) {
    return;
  }
  millisLastSbusPrint = millis();
  printChannels();
  Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 178 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_SBus.ino" 3
                (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 178 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_SBus.ino"
                "-------------------------------"
# 178 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_SBus.ino" 3
                ); &__c[0];}))
# 178 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_SBus.ino"
                )));
}

void printChannels() {
  // for (int i = 1; i < 8; i++) {
  //   Serial.print("CH ");
  //   Serial.print(i);
  //   Serial.print(": ");
  //   Serial.print(getChannelFloat(i));
  //   Serial.print("\t");
  // }
  // Serial.println();

  Serial.print("Good frames: ");
  Serial.print(sbus.getGoodFrames());

  Serial.print(" \tData loss on connection: ");
  Serial.print(sbus.getFrameLoss());
  Serial.print("% \t");

  Serial.print("Time diff: ");
  Serial.println(millis() - (unsigned long)sbus.getLastTime());
}

// Timer2 triggers ever 1ms and processes the incoming SBUS datastream
// ISR(TIMER2_COMPA_vect)
// {
//   if(GetStatus(MODULE_RF)) sbus.process();
// }
# 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
/*

    USB communication, handles reading serial and parsing commands.



    By

    Mads Rosenhoej Jeppesen - Aarhus 2021

    mrj@mpe.au.dk



    Query data from Drill Logger SD card

*/
# 11 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
const byte numChars = 32;
char receivedCMD[numChars];
int moduleSlct;

void initializeDebugComm() {
  Serial.begin(115200);
  if (Serial /* USB*/)
    SetStatus(17, true);
  else
    SetStatus(17, false);

  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 22 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 22 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 "#---------------------------#"
# 22 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 ); &__c[0];}))
# 22 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 22 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 22 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 "#---------------------------#"
# 22 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 ); &__c[0];}))
# 22 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 )));
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 23 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 23 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 "#     GeoRover Firmware     #"
# 23 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 ); &__c[0];}))
# 23 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 23 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 23 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 "#     GeoRover Firmware     #"
# 23 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 ); &__c[0];}))
# 23 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 )));
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 24 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 24 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 "#       #DEBUG MODE#        #"
# 24 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 ); &__c[0];}))
# 24 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 24 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 24 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 "#       #DEBUG MODE#        #"
# 24 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 ); &__c[0];}))
# 24 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 )));
  BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 25 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 25 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 "#  System Version: "
# 25 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 ); &__c[0];}))
# 25 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 25 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 25 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 "#  System Version: "
# 25 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 ); &__c[0];}))
# 25 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 )));
  BlackBoxAppend("0.91.126"); Serial.print("0.91.126");
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 27 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 27 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 " #"
# 27 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 ); &__c[0];}))
# 27 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 27 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 27 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 " #"
# 27 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 ); &__c[0];}))
# 27 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 )));
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 28 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 28 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 "#---------------------------#"
# 28 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 ); &__c[0];}))
# 28 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 28 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 28 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 "#---------------------------#"
# 28 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 ); &__c[0];}))
# 28 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 )));
  BlackBoxAppendln(); Serial.println();
}

// Receive Commands
void recvWithStartEndMarkers() {
  if (Serial.available() <= 0) {
    return;
  }

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
        receivedCMD[ndx] = '\0'; // terminate the string
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

bool activeCommand = false;
void PerformCommand() {
  if (!activeCommand) {
    return;
  }

  SystemTestModule(moduleSlct);
}

bool DebugCommStatus() {
  return Serial /* USB*/;
}

// Parse read Command
void parseCommand() {
  BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 83 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 83 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 "Received command (DBG_PRT): \""
# 83 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 ); &__c[0];}))
# 83 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 83 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 83 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 "Received command (DBG_PRT): \""
# 83 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 ); &__c[0];}))
# 83 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 )));
  BlackBoxAppend(receivedCMD); Serial.print(receivedCMD);
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 85 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 85 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 "\""
# 85 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 ); &__c[0];}))
# 85 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 85 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 85 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 "\""
# 85 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 ); &__c[0];}))
# 85 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 )));

  switch (receivedCMD[0]) {
    case 'F':
      parseCommandFiles();
      break;
    case 'S':
      parseCommandStrategy();
      break;
    case 'B':
      parseCommandBackup();
      break;
    case 'M':
      parseCommandModule();
      break;
    case 'R':
      parseCommandRoute();
      break;
    case '\0':
      break;
    default:
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 106 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 106 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     "NACK"
# 106 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 106 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 106 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 106 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     "NACK"
# 106 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 106 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     )));
      break;
  }
}

void parseCommandFiles() {
  char *fileNamePtr = receivedCMD + 2;
  char fileName[numChars] = {0};
  strcpy(fileName, fileNamePtr);

  switch (receivedCMD[1]) {
    case 'L':
      SDQuery();
      break;
    case 'S':
      SDSize(fileName);
      break;
    case 'D':
      SDDownload(fileName);
      break;
    case 'R':
      SDDelete(fileName);
      break;
    case 'B':
      BlackBoxPrint();
      break;
    case 'C':
      BlackBoxClear();
      break;
    case '\0':
      break;
    default:
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 138 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 138 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     "NACK"
# 138 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 138 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 138 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 138 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     "NACK"
# 138 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 138 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     )));
      break;
  }
}

void parseCommandStrategy() {
  switch (receivedCMD[1]) {
    case 'S':
      BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 146 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 146 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     "Manual Strategy Set to: "
# 146 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 146 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 146 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 146 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     "Manual Strategy Set to: "
# 146 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 146 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     )));
      BlackBoxAppendln((int)(receivedCMD[2] - '0')); Serial.println((int)(receivedCMD[2] - '0'));
      if (!SetMode((int)(receivedCMD[2] - '0')))
        BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 149 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
       (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 149 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
       "Mode not found!"
# 149 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
       ); &__c[0];}))
# 149 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
       ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 149 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
       (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 149 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
       "Mode not found!"
# 149 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
       ); &__c[0];}))
# 149 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
       )));
      break;
    case 'F':
      strategyMethods[3][mode]();
      break;
    case 'O':
      // Override
      break;
    case '\0':
      break;
    default:
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 160 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 160 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     "NACK"
# 160 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 160 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 160 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 160 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     "NACK"
# 160 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 160 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     )));
      break;
  }
}

void parseCommandBackup() {
  switch (receivedCMD[1]) {
    case 'R':
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 168 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 168 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     "Manual Reset of Backup System."
# 168 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 168 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 168 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 168 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     "Manual Reset of Backup System."
# 168 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 168 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     )));
      ResetBackupCPU();
      break;
    case 'S':
      BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 172 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 172 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     "Backup System Status: "
# 172 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 172 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 172 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 172 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     "Backup System Status: "
# 172 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 172 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     )));
      BlackBoxAppendln(GetStatus(21)); Serial.println(GetStatus(21));
      break;
    case 'H':
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 176 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 176 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     "Virtual Heartbeat"
# 176 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 176 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 176 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 176 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     "Virtual Heartbeat"
# 176 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 176 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     )));
      HeartBeatInInterrupt();
      break;
    case 'F':
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 180 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 180 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     "Simulating System Halt for 60 sec"
# 180 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 180 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 180 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 180 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     "Simulating System Halt for 60 sec"
# 180 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 180 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     )));
      delay(60000);
      break;
    case '\0':
      break;
    default:
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 186 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 186 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     "NACK"
# 186 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 186 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 186 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 186 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     "NACK"
# 186 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 186 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     )));
      break;
  }
}

void parseCommandModule() {
  char *modulePtr = receivedCMD + 2;
  char moduleChar[numChars] = {0};
  strcpy(moduleChar, modulePtr);

  moduleSlct = atoi(moduleChar);

  switch (receivedCMD[1]) {
    case 'E':
      BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 200 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 200 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     "Manual Enable of Module: "
# 200 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 200 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 200 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 200 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     "Manual Enable of Module: "
# 200 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 200 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     )));
      switch (receivedCMD[2]) {
        case '\0':
          BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 203 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 203 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
         "ALL SYSTEMS"
# 203 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
         ); &__c[0];}))
# 203 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
         ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 203 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 203 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
         "ALL SYSTEMS"
# 203 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
         ); &__c[0];}))
# 203 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
         )));
          break;
        default:
          BlackBoxAppendln(moduleSlct); Serial.println(moduleSlct);
          SystemEnable(moduleSlct);
          break;
      }
      break;
    case 'D':
      BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 212 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 212 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     "Manual Disable of Module: "
# 212 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 212 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 212 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 212 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     "Manual Disable of Module: "
# 212 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 212 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     )));
      switch (receivedCMD[2]) {
        case '\0':
          BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 215 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 215 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
         "ALL SYSTEMS"
# 215 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
         ); &__c[0];}))
# 215 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
         ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 215 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 215 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
         "ALL SYSTEMS"
# 215 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
         ); &__c[0];}))
# 215 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
         )));
          SystemDisable();
          break;
        default:
          BlackBoxAppendln(moduleSlct); Serial.println(moduleSlct);
          SystemDisable(moduleSlct);
          break;
      }
      break;
    case 'O':
      BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 225 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 225 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     "Manual Override of Module: "
# 225 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 225 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 225 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 225 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     "Manual Override of Module: "
# 225 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 225 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     )));
      switch (receivedCMD[2]) {
        case '\0':
          BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 228 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 228 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
         "NACK"
# 228 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
         ); &__c[0];}))
# 228 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
         ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 228 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 228 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
         "NACK"
# 228 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
         ); &__c[0];}))
# 228 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
         )));
          // SystemDisable();
          break;
        default:
          BlackBoxAppend(moduleSlct); Serial.print(moduleSlct);
          BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 233 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 233 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
         "\t"
# 233 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
         ); &__c[0];}))
# 233 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
         ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 233 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 233 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
         "\t"
# 233 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
         ); &__c[0];}))
# 233 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
         )));
          BlackBoxAppendln(((!GetStatus(moduleSlct)) == 1 ? "T" : ((!GetStatus(moduleSlct)) == 0 ? "F" : "Unknown"))); Serial.println(((!GetStatus(moduleSlct)) == 1 ? "T" : ((!GetStatus(moduleSlct)) == 0 ? "F" : "Unknown")));
          SetStatus(moduleSlct, !GetStatus(moduleSlct));
          break;
      }
      break;
    case 'S':
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 240 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 240 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     "Manual System Status Check"
# 240 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 240 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 240 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 240 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     "Manual System Status Check"
# 240 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 240 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     )));
      // GetStatus(true);
      BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 242 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 242 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     "System Status: "
# 242 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 242 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 242 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 242 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     "System Status: "
# 242 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 242 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     )));
      BlackBoxAppendln(String(ToLong(SystemStatus))); Serial.println(String(ToLong(SystemStatus)));
      break;
    case 'R':
      BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 246 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 246 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     "Manual System Reset in: "
# 246 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 246 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 246 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 246 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     "Manual System Reset in: "
# 246 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 246 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     )));
      CountDownPrint();
      systemReset();
      break;
    case 'T':
      BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 251 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 251 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     "Testing Module: "
# 251 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 251 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 251 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 251 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     "Testing Module: "
# 251 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 251 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     )));
      BlackBoxAppendln(((moduleSlct) == 0 ? "Primary Power" : ((moduleSlct) == 1 ? "Secondary Power (5V)" : ((moduleSlct) == 2 ? "Secondary Power (12V)" : ((moduleSlct) == 3 ? "Secondary Power (24V)" : ((moduleSlct) == 4 ? "Secondary Power (Motors)" : ((moduleSlct) == 5 ? "Motors" : ((moduleSlct) == 6 ? "Motor Left" : ((moduleSlct) == 7 ? "Motor Right" : ((moduleSlct) == 8 ? "Motor Active" : ((moduleSlct) == 9 ? "CAN-bus" : ((moduleSlct) == 10 ? "Short Range Communication (RF)" : ((moduleSlct) == 11 ? "Long Range Communication (Iridium)" : ((moduleSlct) == 12 ? "Global Navigation Satellite System" : ((moduleSlct) == 13 ? "Accelerometer" : ((moduleSlct) == 14 ? "Local Storage" : ((moduleSlct) == 15 ? "Blackbox" : ((moduleSlct) == 16 ? "Waypoint Route" : ((moduleSlct) == 17 ? "Debug Comm." : ((moduleSlct) == 18 ? "System LED" : ((moduleSlct) == 19 ? "Heating" : ((moduleSlct) == 20 ? "Temperature" : ((moduleSlct) == 21 ? "Backup CPU" : ((moduleSlct) == 22 ? "Emergency Stop" : ((moduleSlct) == 23 ? "Reserved" : ("Unknown")))))))))))))))))))))))))); Serial.println(((moduleSlct) == 0 ? "Primary Power" : ((moduleSlct) == 1 ? "Secondary Power (5V)" : ((moduleSlct) == 2 ? "Secondary Power (12V)" : ((moduleSlct) == 3 ? "Secondary Power (24V)" : ((moduleSlct) == 4 ? "Secondary Power (Motors)" : ((moduleSlct) == 5 ? "Motors" : ((moduleSlct) == 6 ? "Motor Left" : ((moduleSlct) == 7 ? "Motor Right" : ((moduleSlct) == 8 ? "Motor Active" : ((moduleSlct) == 9 ? "CAN-bus" : ((moduleSlct) == 10 ? "Short Range Communication (RF)" : ((moduleSlct) == 11 ? "Long Range Communication (Iridium)" : ((moduleSlct) == 12 ? "Global Navigation Satellite System" : ((moduleSlct) == 13 ? "Accelerometer" : ((moduleSlct) == 14 ? "Local Storage" : ((moduleSlct) == 15 ? "Blackbox" : ((moduleSlct) == 16 ? "Waypoint Route" : ((moduleSlct) == 17 ? "Debug Comm." : ((moduleSlct) == 18 ? "System LED" : ((moduleSlct) == 19 ? "Heating" : ((moduleSlct) == 20 ? "Temperature" : ((moduleSlct) == 21 ? "Backup CPU" : ((moduleSlct) == 22 ? "Emergency Stop" : ((moduleSlct) == 23 ? "Reserved" : ("Unknown"))))))))))))))))))))))))));
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 253 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 253 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     "------------------------------------"
# 253 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 253 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 253 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 253 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     "------------------------------------"
# 253 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 253 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     )));
      activeCommand = true;
      break;
    case 'Q':
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 257 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 257 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     "------------------------------------"
# 257 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 257 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 257 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 257 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     "------------------------------------"
# 257 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 257 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     )));
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 258 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 258 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     "Manual System Test Stop"
# 258 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 258 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 258 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 258 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     "Manual System Test Stop"
# 258 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 258 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     )));
      SystemDisable(moduleSlct);
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 260 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 260 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     "------------------------------------"
# 260 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 260 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 260 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 260 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     "------------------------------------"
# 260 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 260 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     )));
      activeCommand = false;
      systemTestState = -1;
      break;
    case '\0':
      break;
    default:
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 267 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 267 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     "NACK"
# 267 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 267 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 267 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 267 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     "NACK"
# 267 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 267 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     )));
      break;
  }
}

void parseCommandRoute() {
  char tempChars[numChars];
  char *strtokIndx;
  strcpy(tempChars, receivedCMD);

  long latLong;
  long lonLong;

  strtokIndx = strtok(tempChars, ",");
  strtokIndx = strtok(
# 281 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3 4
                     __null
# 281 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
                         , ",");
  latLong = atol(strtokIndx);
  strtokIndx = strtok(
# 283 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3 4
                     __null
# 283 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
                         , ",");
  lonLong = atol(strtokIndx);

  switch (receivedCMD[1]) {
    case 'S':
      // write coordinate
      BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 289 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 289 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     "Lat: "
# 289 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 289 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 289 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 289 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     "Lat: "
# 289 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 289 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     )));
      BlackBoxAppend(latLong); Serial.print(latLong);
      BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 291 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 291 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     " Lon: "
# 291 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 291 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 291 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 291 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     " Lon: "
# 291 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 291 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     )));
      BlackBoxAppendln(lonLong); Serial.println(lonLong);

      EEPROM.put(1 + 8 + 2 + 2 + 2 + (long)(receivedCMD[2] - '0') * 8, latLong);
      EEPROM.put(1 + 8 + 2 + 2 + 2 + 4 + (long)(receivedCMD[2] - '0') * 8, lonLong);
      break;
    case 'P':
      EEPROM.get(1 + 8 + 2 + 2 + 2 + (long)(receivedCMD[2] - '0') * 8, latLong);
      EEPROM.get(1 + 8 + 2 + 2 + 2 + 4 + (long)(receivedCMD[2] - '0') * 8, lonLong);

      BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 301 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 301 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     "Lat: "
# 301 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 301 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 301 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 301 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     "Lat: "
# 301 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 301 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     )));
      BlackBoxAppend(latLong); Serial.print(latLong);
      BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 303 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 303 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     " Lon: "
# 303 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 303 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 303 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 303 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     " Lon: "
# 303 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
     ); &__c[0];}))
# 303 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
     )));
      BlackBoxAppendln(lonLong); Serial.println(lonLong);
      break;
    default:
      break;
  }
}

void parseData() { // split the data into its parts

  char *strtokIndx; // this is used by strtok() as an index
}

void CountDownPrint() {
  BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 317 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 317 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 "3"
# 317 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 ); &__c[0];}))
# 317 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 317 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 317 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 "3"
# 317 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 ); &__c[0];}))
# 317 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 )));
  delay(333);
  BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 319 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 319 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 "."
# 319 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 ); &__c[0];}))
# 319 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 319 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 319 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 "."
# 319 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 ); &__c[0];}))
# 319 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 )));
  delay(333);
  BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 321 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 321 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 ". "
# 321 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 ); &__c[0];}))
# 321 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 321 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 321 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 ". "
# 321 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 ); &__c[0];}))
# 321 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 )));
  delay(333);
  BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 323 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 323 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 "2"
# 323 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 ); &__c[0];}))
# 323 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 323 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 323 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 "2"
# 323 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 ); &__c[0];}))
# 323 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 )));
  delay(333);
  BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 325 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 325 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 "."
# 325 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 ); &__c[0];}))
# 325 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 325 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 325 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 "."
# 325 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 ); &__c[0];}))
# 325 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 )));
  delay(333);
  BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 327 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 327 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 ". "
# 327 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 ); &__c[0];}))
# 327 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 327 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 327 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 ". "
# 327 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 ); &__c[0];}))
# 327 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 )));
  delay(333);
  BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 329 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 329 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 "1"
# 329 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 ); &__c[0];}))
# 329 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 329 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 329 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 "1"
# 329 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 ); &__c[0];}))
# 329 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 )));
  delay(333);
  BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 331 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 331 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 "."
# 331 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 ); &__c[0];}))
# 331 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 331 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 331 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 "."
# 331 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 ); &__c[0];}))
# 331 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 )));
  delay(333);
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 333 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 333 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 ". "
# 333 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 ); &__c[0];}))
# 333 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 333 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 333 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 ". "
# 333 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino" 3
 ); &__c[0];}))
# 333 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Com_USB.ino"
 )));
  delay(333);
}
# 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino"
/*

  GeoRover Accelerometer functionalities, utilizing the XXXX

  (SparkFun Triple Axis Accelerometer Breakout - MMA8452Q https://www.sparkfun.com/products/12756)



  Primary library used:

    https://github.com/sparkfun/SparkFun_MMA8452Q_Arduino_Library/tree/V_1.1.0



  Mads Rosenhøj Jeppesen

  Aarhus University

  2021

*/
# 13 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino"
# 14 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino" 2

MMA8452Q accel;

int accTestState = 0;
long millisAccTestStart = 0;
long millisLastAccPrint = 0;

bool InitializeAccel() {
  bool status = accel.begin(); // scale +/- 2g and frequency of 1.56 Hz
  accel.setScale(SCALE_2G);
  accel.setDataRate(ODR_12);
  SetStatus(13, status);
  return status;
}

void TerminateAccel() {
  //
}

// accel.read() will update two sets of variables.
// * int's x, y, and z will store the signed 12-bit values
//   read out of the accelerometer.
// * floats cx, cy, and cz will store the calculated
//   acceleration from those 12-bit values. These variables
//   are in units of g's.
void ReadAccel() {
  if (accel.available()) {
    accel.read();
  }
}

bool AccelStatus() {
  if(!GetStatus(13)) return false;

  return accel.cz > 0.8f /* approx 35°*/;
}

bool AccelTest() {
  bool testDone = false;
  switch (accTestState) {
    case 0:
      BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 55 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 55 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino"
     "Accelerometer feed starting for: "
# 55 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino" 3
     ); &__c[0];}))
# 55 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino"
     ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 55 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 55 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino"
     "Accelerometer feed starting for: "
# 55 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino" 3
     ); &__c[0];}))
# 55 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino"
     )));
      BlackBoxAppend(10000); Serial.print(10000);
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 57 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 57 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino"
     " ms"
# 57 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino" 3
     ); &__c[0];}))
# 57 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 57 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 57 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino"
     " ms"
# 57 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino" 3
     ); &__c[0];}))
# 57 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino"
     )));
      millisAccTestStart = millis();
      accTestState++;
      break;
    case 1:
      if (millis() - millisLastAccPrint > 250) {
        millisLastAccPrint = millis();
        BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 64 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino" 3
       (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 64 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino"
       "Accelerometer: "
# 64 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino" 3
       ); &__c[0];}))
# 64 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino"
       ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 64 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino" 3
       (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 64 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino"
       "Accelerometer: "
# 64 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino" 3
       ); &__c[0];}))
# 64 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino"
       )));
        if (AccelStatus()) {
          ReadAccel();
          BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 67 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino" 3
         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 67 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino"
         "x: "
# 67 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino" 3
         ); &__c[0];}))
# 67 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino"
         ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 67 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino" 3
         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 67 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino"
         "x: "
# 67 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino" 3
         ); &__c[0];}))
# 67 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino"
         )));
          BlackBoxAppend((float)accel.cx); Serial.print((float)accel.cx);
          BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 69 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino" 3
         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 69 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino"
         "\t y: "
# 69 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino" 3
         ); &__c[0];}))
# 69 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino"
         ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 69 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino" 3
         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 69 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino"
         "\t y: "
# 69 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino" 3
         ); &__c[0];}))
# 69 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino"
         )));
          BlackBoxAppend((float)accel.cy); Serial.print((float)accel.cy);
          BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 71 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino" 3
         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 71 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino"
         "\t z: "
# 71 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino" 3
         ); &__c[0];}))
# 71 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino"
         ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 71 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino" 3
         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 71 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino"
         "\t z: "
# 71 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino" 3
         ); &__c[0];}))
# 71 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino"
         )));
          BlackBoxAppendln((float)accel.cz); Serial.println((float)accel.cz);
        } else {
          BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 74 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino" 3
         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 74 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino"
         "ERROR"
# 74 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino" 3
         ); &__c[0];}))
# 74 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino"
         ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 74 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino" 3
         (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 74 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino"
         "ERROR"
# 74 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino" 3
         ); &__c[0];}))
# 74 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino"
         )));
        }
      }

      if (millis() - millisAccTestStart > 10000) accTestState++;
      break;
    case 2:
      testDone = true;
      accTestState = 0;
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 83 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 83 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino"
     "------------------------------------"
# 83 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino" 3
     ); &__c[0];}))
# 83 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 83 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 83 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino"
     "------------------------------------"
# 83 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino" 3
     ); &__c[0];}))
# 83 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Accel.ino"
     )));
    default:
      break;
  }

  return testDone;
}
# 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
/*

  GeoRover GNSS functionalities, utilizing the ZED-F9P

  (SparkFun GPS-RTK-SMA Breakout https://www.sparkfun.com/products/16481)



  Primary library used:

    https://github.com/sparkfun/SparkFun_u-blox_GNSS_Arduino_Library



  Mads Rosenhøj Jeppesen

  Aarhus University

  2021

*/
# 13 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
# 14 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 2

SFE_UBLOX_GNSS gnss;

long lastTimeGNSS = 0; // Local timer, limits I2C traffic to u-blox module.

int gnssTestState = 0;
long millisGnssTestStart = 0;
long millisLastGnssPrint = 0;

bool InitializeGnss() {
  Wire.begin();
  bool status = gnss.begin();
  if (status) {
    gnss.setI2COutput(COM_TYPE_UBX); // Set the I2C port to output UBX only (turn off NMEA noise)
    gnss.setVal(UBLOX_CFG_RATE_MEAS, 1000 /* in milliseconds*/); // Set measurement rate to 1000ms (1Hz update rate)
  }
  return status;
}

bool GnssStatus() {
  return gnss.isConnected();
}

// Updates current position and Gnss status
bool GnssUpdate(){
  latCurrent = GnssGetLat();
  lonCurrent = GnssGetLong();
}

bool GnssTest() {
  bool testDone = false;
  switch (gnssTestState) {
    case 0:
      BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 47 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 47 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
     "GNSS feed starting for: "
# 47 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
     ); &__c[0];}))
# 47 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
     ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 47 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 47 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
     "GNSS feed starting for: "
# 47 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
     ); &__c[0];}))
# 47 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
     )));
      BlackBoxAppend(10000); Serial.print(10000);
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 49 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 49 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
     " ms"
# 49 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
     ); &__c[0];}))
# 49 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 49 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 49 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
     " ms"
# 49 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
     ); &__c[0];}))
# 49 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
     )));
      millisGnssTestStart = millis();
      gnssTestState++;
      break;
    case 1:
      if (millis() - millisLastGnssPrint > 500) {
        millisLastGnssPrint = millis();
        QueryGnss();
      }

      if (millis() - millisGnssTestStart > 10000) gnssTestState++;
      break;
    case 2:
      SetStatus(12, GnssTime(true));
      gnssTestState = 0;
      testDone = true;
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 65 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 65 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
     "------------------------------------"
# 65 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
     ); &__c[0];}))
# 65 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 65 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 65 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
     "------------------------------------"
# 65 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
     ); &__c[0];}))
# 65 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
     )));
    default:
      break;
  }

  return testDone;
}

bool GnssTime(bool print) {
  bool status;
  if (print) BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 75 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
            (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 75 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
            "Time and Date is: "
# 75 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
            ); &__c[0];}))
# 75 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
            ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 75 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
            (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 75 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
            "Time and Date is: "
# 75 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
            ); &__c[0];}))
# 75 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
            )));
  if (!gnss.getTimeValid() || !gnss.getDateValid()) {
    if (print) BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 77 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 77 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
              "not valid"
# 77 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
              ); &__c[0];}))
# 77 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
              ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 77 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 77 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
              "not valid"
# 77 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
              ); &__c[0];}))
# 77 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
              )));
    status = false;
  } else {
    if (print) {
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 81 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 81 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
     "valid"
# 81 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
     ); &__c[0];}))
# 81 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 81 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 81 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
     "valid"
# 81 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
     ); &__c[0];}))
# 81 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
     )));
      int year = gnss.getYear();
      int month = gnss.getMonth();
      int day = gnss.getDay();
      int hour = gnss.getHour();
      int minute = gnss.getMinute();

      BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 88 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 88 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
     "Current time: "
# 88 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
     ); &__c[0];}))
# 88 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
     ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 88 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 88 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
     "Current time: "
# 88 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
     ); &__c[0];}))
# 88 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
     )));
      BlackBoxAppend(year); Serial.print(year);
      BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 90 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 90 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
     "-"
# 90 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
     ); &__c[0];}))
# 90 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
     ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 90 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 90 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
     "-"
# 90 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
     ); &__c[0];}))
# 90 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
     )));
      BlackBoxAppend(month); Serial.print(month);
      BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 92 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 92 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
     "-"
# 92 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
     ); &__c[0];}))
# 92 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
     ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 92 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 92 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
     "-"
# 92 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
     ); &__c[0];}))
# 92 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
     )));
      BlackBoxAppend(day); Serial.print(day);
      BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 94 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 94 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
     "-"
# 94 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
     ); &__c[0];}))
# 94 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
     ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 94 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 94 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
     "-"
# 94 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
     ); &__c[0];}))
# 94 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
     )));
      BlackBoxAppend(hour); Serial.print(hour);
      BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 96 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 96 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
     ":"
# 96 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
     ); &__c[0];}))
# 96 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
     ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 96 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 96 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
     ":"
# 96 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
     ); &__c[0];}))
# 96 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
     )));
      BlackBoxAppendln(minute); Serial.println(minute);
    }
    status = true;
  }

  return status;
}

void TerminateGnss() {
  gnss.end();
}

// Gets positional data in Latitude in degrees * 10^-7
long GnssGetLat() {
  return gnss.getLatitude();
}

// Gets positional data in Longitude in degrees * 10^-7
long GnssGetLong() {
  return gnss.getLongitude();
}

// Returns heading in degrees * 10^-5
long GnssGetHeading(){
  return gnss.getHeading();
}

// Query module and prints Lat, Long, Alt, Acc
void QueryGnss() {
  BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 126 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 126 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
 "GNSS: "
# 126 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
 ); &__c[0];}))
# 126 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
 ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 126 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 126 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
 "GNSS: "
# 126 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
 ); &__c[0];}))
# 126 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
 )));

  if (GnssStatus()) {
    long latitude = gnss.getLatitude();
    BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 130 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 130 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
   "Lat: "
# 130 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
   ); &__c[0];}))
# 130 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
   ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 130 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 130 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
   "Lat: "
# 130 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
   ); &__c[0];}))
# 130 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
   )));
    BlackBoxAppend(latitude); Serial.print(latitude);
    BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 132 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 132 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
   " (deg * 10^-7)"
# 132 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
   ); &__c[0];}))
# 132 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
   ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 132 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 132 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
   " (deg * 10^-7)"
# 132 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
   ); &__c[0];}))
# 132 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
   )));

    long longitude = gnss.getLongitude();
    BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 135 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 135 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
   "\tLong: "
# 135 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
   ); &__c[0];}))
# 135 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
   ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 135 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 135 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
   "\tLong: "
# 135 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
   ); &__c[0];}))
# 135 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
   )));
    BlackBoxAppend(longitude); Serial.print(longitude);
    BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 137 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 137 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
   " (deg * 10^-7)"
# 137 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
   ); &__c[0];}))
# 137 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
   ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 137 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 137 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
   " (deg * 10^-7)"
# 137 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
   ); &__c[0];}))
# 137 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
   )));

    long altitude = gnss.getAltitude();
    BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 140 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 140 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
   "\tAlt: "
# 140 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
   ); &__c[0];}))
# 140 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
   ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 140 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 140 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
   "\tAlt: "
# 140 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
   ); &__c[0];}))
# 140 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
   )));
    BlackBoxAppend(altitude); Serial.print(altitude);
    BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 142 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 142 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
   " (mm)"
# 142 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
   ); &__c[0];}))
# 142 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
   ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 142 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 142 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
   " (mm)"
# 142 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
   ); &__c[0];}))
# 142 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
   )));

    long accuracy = gnss.getPositionAccuracy();
    BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 145 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 145 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
   "\t3D Positional Accuracy: "
# 145 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
   ); &__c[0];}))
# 145 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
   ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 145 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 145 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
   "\t3D Positional Accuracy: "
# 145 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
   ); &__c[0];}))
# 145 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
   )));
    BlackBoxAppend(accuracy); Serial.print(accuracy);
    BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 147 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 147 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
   " (mm)"
# 147 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
   ); &__c[0];}))
# 147 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
   ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 147 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 147 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
   " (mm)"
# 147 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino" 3
   ); &__c[0];}))
# 147 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Gnss.ino"
   )));
  } else {
    BlackBoxAppendln("ERROR"); Serial.println("ERROR");
  }
}

// Returns current signal status
bool GnssSignal(){
  if(!GetStatus(12)){
    return false;
  }

  return (gnss.getTimeValid() && gnss.getDateValid() && gnss.getGnssFixOk());
}

// returns distance in meters between two positions, both specified
// as signed decimal-degrees latitude and longitude. Uses great-circle
// distance computation for hypothetical sphere of radius 6372795 meters.
// Because Earth is no exact sphere, rounding errors may be up to 0.5%.
// Courtesy of Maarten Lamers
double DistanceBetween(double lat1, double long1, double lat2, double long2) {
  double delta = ((long1 - long2)*0.017453292519943295769236907684886);
  double sdlong = sin(delta);
  double cdlong = cos(delta);
  lat1 = ((lat1)*0.017453292519943295769236907684886);
  lat2 = ((lat2)*0.017453292519943295769236907684886);
  double slat1 = sin(lat1);
  double clat1 = cos(lat1);
  double slat2 = sin(lat2);
  double clat2 = cos(lat2);
  delta = (clat1 * slat2) - (slat1 * clat2 * cdlong);
  delta = ((delta)*(delta));
  delta += ((clat2 * sdlong)*(clat2 * sdlong));
  delta = sqrt(delta);
  double denom = (slat1 * slat2) + (clat1 * clat2 * cdlong);
  delta = atan2(delta, denom);
  return delta * 6372795;
}

// returns course in degrees (North=0, West=270) from position 1 to position 2,
// both specified as signed decimal-degrees latitude and longitude.
// Because Earth is no exact sphere, calculated course may be off by a tiny fraction.
// Courtesy of Maarten Lamers
double CourseTo(double lat1, double long1, double lat2, double long2) {
  double dlon = ((long2 - long1)*0.017453292519943295769236907684886);
  lat1 = ((lat1)*0.017453292519943295769236907684886);
  lat2 = ((lat2)*0.017453292519943295769236907684886);
  double a1 = sin(dlon) * cos(lat2);
  double a2 = sin(lat1) * cos(lat2) * cos(dlon);
  a2 = cos(lat1) * sin(lat2) - a2;
  a2 = atan2(a1, a2);
  if (a2 < 0.0) {
    a2 += 6.283185307179586476925286766559;
  }
  return ((a2)*57.295779513082320876798154814105);
}

// Returns Distance between two sets of coordinates
double DistanceBetweenLong(long lat1, long long1, long lat2, long long2){
  return DistanceBetween(CoordLong2Double(lat1),CoordLong2Double(long1),CoordLong2Double(lat2),CoordLong2Double(long2));
}

// Returns Distance between two sets of coordinates
double CourseToLong(long lat1, long long1, long lat2, long long2){
  return CourseTo(CoordLong2Double(lat1),CoordLong2Double(long1),CoordLong2Double(lat2),CoordLong2Double(long2));
}
# 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino"
/*

  GeoRover Temperature Sensor functionalities, utilizing a 10kOhm NTC thermistor

  (https://dk.rs-online.com/web/p/termistor-ic/7062759)



  Mads Rosenhøj Jeppesen

  Aarhus University

  2021

*/
# 10 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino"
int tempTestState = 0;
long millisTempTestStart = 0;
long millisLastTempPrint = 0;

int MeanThermTemp() {
  int tempCount, total;

  for (int i = 0; i < 3; i++) {
    int therm = ThermTemp(i);
    if (therm != 0) {
      total += therm;
      tempCount++;
    }
  }

  return total / tempCount;
}

int ThermTemp(int thermistor) {
  int temp;
  switch (thermistor) {
    case 0:
      temp = analogRead((55));
      break;
    case 1:
      temp = analogRead((56));
      break;
    case 2:
      temp = analogRead((57));
      break;
    default:
      break;
  }
  return temp;
}

bool HeatingStart() {
  if (millis() - lastMillistHeatingOff < 40000) {
    return false;
  }

  digitalWrite(48, 0x1);
  lastMillistHeatingOn = millis();

  return true;
}

bool HeatingStop() {
  digitalWrite(48, 0x0);
  lastMillistHeatingOff = millis();
}

bool HeatingStatus() {
  return GetStatus(19);
}

bool TemperatureStatus() {
  bool status = MeanThermTemp() > 10 /* ~28 V*/;

  SetStatus(20, status);
  return status;
}

// Runs temperature test for a duration, returns true once test is complete
bool TemperatureTest() {
  bool testDone = false;
  int meanTemp;
  switch (tempTestState) {
    case 0:
      BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 79 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 79 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino"
     "Thermistor feed starting for: "
# 79 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino" 3
     ); &__c[0];}))
# 79 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino"
     ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 79 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 79 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino"
     "Thermistor feed starting for: "
# 79 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino" 3
     ); &__c[0];}))
# 79 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino"
     )));
      BlackBoxAppend(10000); Serial.print(10000);
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 81 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 81 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino"
     " ms"
# 81 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino" 3
     ); &__c[0];}))
# 81 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 81 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 81 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino"
     " ms"
# 81 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino" 3
     ); &__c[0];}))
# 81 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino"
     )));
      millisTempTestStart = millis();
      tempTestState++;
      break;
    case 1:
      if (millis() - millisLastTempPrint > 500) {
        millisLastTempPrint = millis();
        meanTemp = MeanThermTemp();
        BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 89 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino" 3
       (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 89 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino"
       "Temperatures: "
# 89 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino" 3
       ); &__c[0];}))
# 89 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino"
       ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 89 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino" 3
       (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 89 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino"
       "Temperatures: "
# 89 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino" 3
       ); &__c[0];}))
# 89 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino"
       )));
        BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 90 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino" 3
       (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 90 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino"
       "\tT1: "
# 90 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino" 3
       ); &__c[0];}))
# 90 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino"
       ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 90 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino" 3
       (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 90 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino"
       "\tT1: "
# 90 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino" 3
       ); &__c[0];}))
# 90 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino"
       )));
        BlackBoxAppend(ThermTemp(0)); Serial.print(ThermTemp(0));
        BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 92 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino" 3
       (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 92 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino"
       "\tT2: "
# 92 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino" 3
       ); &__c[0];}))
# 92 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino"
       ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 92 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino" 3
       (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 92 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino"
       "\tT2: "
# 92 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino" 3
       ); &__c[0];}))
# 92 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino"
       )));
        BlackBoxAppend(ThermTemp(1)); Serial.print(ThermTemp(1));
        BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 94 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino" 3
       (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 94 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino"
       "\tT3: "
# 94 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino" 3
       ); &__c[0];}))
# 94 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino"
       ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 94 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino" 3
       (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 94 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino"
       "\tT3: "
# 94 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino" 3
       ); &__c[0];}))
# 94 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino"
       )));
        BlackBoxAppend(ThermTemp(2)); Serial.print(ThermTemp(2));
        BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 96 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino" 3
       (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 96 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino"
       "\tMean: "
# 96 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino" 3
       ); &__c[0];}))
# 96 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino"
       ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 96 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino" 3
       (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 96 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino"
       "\tMean: "
# 96 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino" 3
       ); &__c[0];}))
# 96 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sen_Temp.ino"
       )));
        BlackBoxAppendln(meanTemp); Serial.println(meanTemp);

        SetStatus(20, meanTemp > 10 /* ~28 V*/);
      }

      if (millis() - millisTempTestStart > 10000) tempTestState++;
      break;
    case 2:
      testDone = true;
      tempTestState = 0;
      break;
    default:
      break;
  }

  return testDone;
}

// If temp < minimum temp runs heating period
void HeatingProcess() {
  if (TemperatureStatus()) {
    SystemDisable(19);
    return;
  }

  HeatingCheck();
}

// If currently heating checks for maximum heating duration
// If not currently heating checks for timeout to reactivate heating
void HeatingCheck() {
  if (GetStatus(19)) {
    if (millis() - lastMillistHeatingOn > 20000) {
      SystemDisable(19);
    }
  } else {
    if (millis() - lastMillistHeatingOff > 40000) {
      SystemEnable(19);
    }
  }
}
# 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino"
/*  Strategy - Autonomous



    Primary Strategy for Autonomous Navigation

*/
# 7 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino"
// Start sequence of strategy
void StartStrategyAutonomous() {
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino"
 "------------------------------------"
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino" 3
 ); &__c[0];}))
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino"
 "------------------------------------"
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino" 3
 ); &__c[0];}))
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino"
 )));
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 10 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 10 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino"
 "Strategy (Autonomous): Starting."
# 10 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino" 3
 ); &__c[0];}))
# 10 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 10 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 10 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino"
 "Strategy (Autonomous): Starting."
# 10 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino" 3
 ); &__c[0];}))
# 10 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino"
 )));

  SystemDisable();

  // TODO: Will reset autonomy upon reboot
  // - Add bool in EEPROM autonomous active, if true skip reset. Unset bool upon in FinishStrategyAutonomous
  AutonomyReset();

  AttachSelectButton();

  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 20 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 20 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino"
 "Strategy (Autonomous): Initialized."
# 20 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino" 3
 ); &__c[0];}))
# 20 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 20 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 20 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino"
 "Strategy (Autonomous): Initialized."
# 20 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino" 3
 ); &__c[0];}))
# 20 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino"
 )));
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 21 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 21 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino"
 "------------------------------------"
# 21 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino" 3
 ); &__c[0];}))
# 21 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 21 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 21 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino"
 "------------------------------------"
# 21 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino" 3
 ); &__c[0];}))
# 21 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino"
 )));

  StrategyStartLed();
}

// Main sequence of strategy
void RunStrategyAutonomous() {

  AutonomousProcess();

  StrategyRunLed();

}

// End sequence of strategy
void FinishStrategyAutonomous() {
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 37 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 37 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino"
 "------------------------------------"
# 37 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino" 3
 ); &__c[0];}))
# 37 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 37 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 37 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino"
 "------------------------------------"
# 37 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino" 3
 ); &__c[0];}))
# 37 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino"
 )));
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 38 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 38 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino"
 "Strategy (Autonomous): Ending."
# 38 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino" 3
 ); &__c[0];}))
# 38 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 38 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 38 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino"
 "Strategy (Autonomous): Ending."
# 38 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino" 3
 ); &__c[0];}))
# 38 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino"
 )));

  DetachSelectButton();

  // TODO: Add unset autonomy bool in EEPROM once implemented

  SystemDisable();

  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 46 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 46 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino"
 "Strategy (Autonomous): Finished."
# 46 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino" 3
 ); &__c[0];}))
# 46 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 46 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 46 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino"
 "Strategy (Autonomous): Finished."
# 46 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino" 3
 ); &__c[0];}))
# 46 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino"
 )));
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 47 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 47 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino"
 "------------------------------------"
# 47 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino" 3
 ); &__c[0];}))
# 47 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 47 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 47 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino"
 "------------------------------------"
# 47 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino" 3
 ); &__c[0];}))
# 47 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Autonomous.ino"
 )));
}

// Select button function, starts autonomous driving
void SelectFunctionAutonomous() {
  if (millis() - lastMillisSelect > 500) {
    lastMillisSelect = millis();
    AutonomyToggle();
  }
}
# 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino"
/*  Strategy - Emergency mode



    Disable all outputs and activate light.

    Detatch interrupts from input buttons, to not exit mode unless emergency reset.

    No actions until mode end.

*/
# 8 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino"
// Start sequence of strategy
void StartStrategyEmergency() {
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 10 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 10 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino"
 "------------------------------------"
# 10 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino" 3
 ); &__c[0];}))
# 10 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 10 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 10 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino"
 "------------------------------------"
# 10 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino" 3
 ); &__c[0];}))
# 10 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino"
 )));
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 11 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 11 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino"
 "Strategy (Emergency): Starting"
# 11 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino" 3
 ); &__c[0];}))
# 11 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 11 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 11 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino"
 "Strategy (Emergency): Starting"
# 11 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino" 3
 ); &__c[0];}))
# 11 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino"
 )));

  // Disable all outputs
  SystemDisable();
  SetStatus(22, false);

  // Disable input buttons
  detachInterrupt(19);
  detachInterrupt(18);
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 20 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 20 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino"
 "Strategy (Emergency): Systems Disabled"
# 20 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino" 3
 ); &__c[0];}))
# 20 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 20 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 20 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino"
 "Strategy (Emergency): Systems Disabled"
# 20 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino" 3
 ); &__c[0];}))
# 20 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino"
 )));

  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 22 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 22 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino"
 "Strategy (Emergency): Initialized"
# 22 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino" 3
 ); &__c[0];}))
# 22 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 22 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 22 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino"
 "Strategy (Emergency): Initialized"
# 22 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino" 3
 ); &__c[0];}))
# 22 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino"
 )));
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 23 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 23 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino"
 "------------------------------------"
# 23 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino" 3
 ); &__c[0];}))
# 23 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 23 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 23 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino"
 "------------------------------------"
# 23 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino" 3
 ); &__c[0];}))
# 23 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino"
 )));
}

// Main sequence of strategy
void RunStrategyEmergency() {
  StrategyRunLed(0 /* Emergency Stop Mode*/);

  // Exit strategy, if emergency not set -> Idle strategy
  if (EmergencyStopStatus()) {
    LedBlink(1 /* 001*/, 1000 / 2, 1000 / 2);
    if (EmergencyStopStatus()) {
      LedBlinkDoubleShort(1 /* 001*/);
      SetMode(2 /* Standby mode*/);
    }
  }
}

// End sequence of strategy
void FinishStrategyEmergency() {
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 42 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 42 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino"
 "------------------------------------"
# 42 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino" 3
 ); &__c[0];}))
# 42 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 42 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 42 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino"
 "------------------------------------"
# 42 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino" 3
 ); &__c[0];}))
# 42 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino"
 )));
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 43 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 43 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino"
 "Strategy (Emergency): Ending"
# 43 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino" 3
 ); &__c[0];}))
# 43 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 43 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 43 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino"
 "Strategy (Emergency): Ending"
# 43 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino" 3
 ); &__c[0];}))
# 43 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino"
 )));

  lastMillisMode = millis();
  lastMillisEstop = millis();

  // Enable input buttons
  attachInterrupt(19, ModeButtonInterruptHandler, 2);

  // Assign Emergency Stop button interrupt
  attachInterrupt(((2) == 2 ? 0 : ((2) == 3 ? 1 : ((2) >= 18 && (2) <= 21 ? 23 - (2) : -1))), EstopButtonInterruptHandler, 2);
  SetStatus(22, true);

  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 55 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 55 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino"
 "Strategy (Emergency): Finished"
# 55 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino" 3
 ); &__c[0];}))
# 55 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 55 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 55 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino"
 "Strategy (Emergency): Finished"
# 55 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino" 3
 ); &__c[0];}))
# 55 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Emergency.ino"
 )));

  // Disable light
}

void SelectFunctionEmergency() {
}

// Returns TRUE if Emergency Button is NOT pressed
// Returns FALSE if Emergency Button IS pressed
bool EmergencyStopStatus() {
  return digitalRead(2);
}
# 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino"
/*  Strategy - Example



    Description...

*/
# 6 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino"
// Start sequence of strategy
void StartStrategyExample() {
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 8 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 8 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino"
 "------------------------------------"
# 8 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino" 3
 ); &__c[0];}))
# 8 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 8 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 8 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino"
 "------------------------------------"
# 8 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino" 3
 ); &__c[0];}))
# 8 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino"
 )));
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino"
 "Strategy (Example): Starting"
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino" 3
 ); &__c[0];}))
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino"
 "Strategy (Example): Starting"
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino" 3
 ); &__c[0];}))
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino"
 )));
  SystemDisable();

  StrategyStartLed();

  SystemEnableMode();

  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 16 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 16 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino"
 "Strategy (Example): Initialized"
# 16 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino" 3
 ); &__c[0];}))
# 16 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 16 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 16 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino"
 "Strategy (Example): Initialized"
# 16 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino" 3
 ); &__c[0];}))
# 16 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino"
 )));
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 17 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 17 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino"
 "------------------------------------"
# 17 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino" 3
 ); &__c[0];}))
# 17 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 17 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 17 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino"
 "------------------------------------"
# 17 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino" 3
 ); &__c[0];}))
# 17 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino"
 )));
}

// Main sequence of strategy
void RunStrategyExample() {
  StrategyRunLed(); // Non-blocking LED blink

  // Add code which runs every loop for current strategy

}

// End sequence of strategy
void FinishStrategyExample() {
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 30 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 30 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino"
 "------------------------------------"
# 30 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino" 3
 ); &__c[0];}))
# 30 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 30 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 30 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino"
 "------------------------------------"
# 30 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino" 3
 ); &__c[0];}))
# 30 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino"
 )));
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 31 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 31 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino"
 "Strategy (Example): Ending"
# 31 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino" 3
 ); &__c[0];}))
# 31 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 31 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 31 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino"
 "Strategy (Example): Ending"
# 31 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino" 3
 ); &__c[0];}))
# 31 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino"
 )));

  DetachSelectButton();

  SystemDisable();

  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 37 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 37 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino"
 "Strategy (Example): Finished"
# 37 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino" 3
 ); &__c[0];}))
# 37 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 37 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 37 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino"
 "Strategy (Example): Finished"
# 37 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino" 3
 ); &__c[0];}))
# 37 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Example.ino"
 )));
}

// Select button function for current strategy
void SelectFunctionExample() {
  if (millis() - lastMillisSelect > 500) {
    lastMillisSelect = millis();
    // Add functionality here
  }
}
# 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino"
/*  Idle mode



    Description...

*/
# 6 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino"
// Start sequence of strategy
void StartStrategyIdle() {
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 8 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 8 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino"
 "------------------------------------"
# 8 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino" 3
 ); &__c[0];}))
# 8 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 8 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 8 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino"
 "------------------------------------"
# 8 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino" 3
 ); &__c[0];}))
# 8 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino"
 )));
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino"
 "Strategy (Idle): Starting"
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino" 3
 ); &__c[0];}))
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino"
 "Strategy (Idle): Starting"
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino" 3
 ); &__c[0];}))
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino"
 )));

  SystemDisable();

  AttachSelectButton();

  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 15 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 15 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino"
 "Strategy (Idle): Initialized"
# 15 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino" 3
 ); &__c[0];}))
# 15 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 15 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 15 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino"
 "Strategy (Idle): Initialized"
# 15 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino" 3
 ); &__c[0];}))
# 15 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino"
 )));
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 16 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 16 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino"
 "------------------------------------"
# 16 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino" 3
 ); &__c[0];}))
# 16 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 16 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 16 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino"
 "------------------------------------"
# 16 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino" 3
 ); &__c[0];}))
# 16 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino"
 )));

  StrategyStartLed();
}

// Main sequence of strategy
void RunStrategyIdle() {
  StrategyRunLed();
}

// End sequence of strategy
void FinishStrategyIdle() {
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 28 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 28 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino"
 "------------------------------------"
# 28 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino" 3
 ); &__c[0];}))
# 28 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 28 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 28 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino"
 "------------------------------------"
# 28 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino" 3
 ); &__c[0];}))
# 28 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino"
 )));
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 29 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 29 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino"
 "Strategy (Idle): Ending"
# 29 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino" 3
 ); &__c[0];}))
# 29 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 29 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 29 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino"
 "Strategy (Idle): Ending"
# 29 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino" 3
 ); &__c[0];}))
# 29 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino"
 )));
  SystemDisable();

  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 32 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 32 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino"
 "Strategy (Idle): Finished"
# 32 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino" 3
 ); &__c[0];}))
# 32 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 32 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 32 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino"
 "Strategy (Idle): Finished"
# 32 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino" 3
 ); &__c[0];}))
# 32 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Idle.ino"
 )));
}

// Selecet button function
void SelectFunctionIdle() {
}
# 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino"
/*  Mode Library mode



    Cycles through modes, activates mode with Select Button.

*/
# 6 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino"
// Start sequence of strategy
void StartStrategyModeLibrary() {
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 8 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 8 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino"
 "------------------------------------"
# 8 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino" 3
 ); &__c[0];}))
# 8 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 8 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 8 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino"
 "------------------------------------"
# 8 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino" 3
 ); &__c[0];}))
# 8 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino"
 )));
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino"
 "Strategy (Mode Library): Starting"
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino" 3
 ); &__c[0];}))
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino"
 "Strategy (Mode Library): Starting"
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino" 3
 ); &__c[0];}))
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino"
 )));

  lastMillisMode = millis();
  lastMillisEstop = millis();

  // Assign mode button interrupt
  attachInterrupt(((19) == 2 ? 0 : ((19) == 3 ? 1 : ((19) >= 18 && (19) <= 21 ? 23 - (19) : -1))), ModeFunctionModeLibrary, 2);

  AttachSelectButton();

  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 19 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 19 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino"
 "Strategy (Mode Library): Initialized"
# 19 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino" 3
 ); &__c[0];}))
# 19 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 19 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 19 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino"
 "Strategy (Mode Library): Initialized"
# 19 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino" 3
 ); &__c[0];}))
# 19 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino"
 )));
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 20 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 20 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino"
 "------------------------------------"
# 20 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino" 3
 ); &__c[0];}))
# 20 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 20 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 20 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino"
 "------------------------------------"
# 20 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino" 3
 ); &__c[0];}))
# 20 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino"
 )));
  BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 21 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 21 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino"
 "Mode Cycle: "
# 21 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino" 3
 ); &__c[0];}))
# 21 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino"
 ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 21 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 21 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino"
 "Mode Cycle: "
# 21 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino" 3
 ); &__c[0];}))
# 21 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino"
 )));
  BlackBoxAppendln(((modeCycle) == 0 ? "Emergency" : ((modeCycle) == 1 ? "Mode Library" : ((modeCycle) == 2 ? "Idle" : ((modeCycle) == 3 ? "System Test" : ((modeCycle) == 4 ? "Remote Control" : ((modeCycle) == 5 ? "Autonomous" : "Unknown"))))))); Serial.println(((modeCycle) == 0 ? "Emergency" : ((modeCycle) == 1 ? "Mode Library" : ((modeCycle) == 2 ? "Idle" : ((modeCycle) == 3 ? "System Test" : ((modeCycle) == 4 ? "Remote Control" : ((modeCycle) == 5 ? "Autonomous" : "Unknown")))))));
}

// Main sequence of strategy
void RunStrategyModeLibrary() {
  StrategyRunLed(modeCycle);
}

// End sequence of strategy
void FinishStrategyModeLibrary() {
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 32 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 32 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino"
 "------------------------------------"
# 32 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino" 3
 ); &__c[0];}))
# 32 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 32 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 32 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino"
 "------------------------------------"
# 32 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino" 3
 ); &__c[0];}))
# 32 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino"
 )));
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 33 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 33 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino"
 "Strategy (Mode Library): Ending"
# 33 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino" 3
 ); &__c[0];}))
# 33 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 33 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 33 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino"
 "Strategy (Mode Library): Ending"
# 33 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino" 3
 ); &__c[0];}))
# 33 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino"
 )));
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 34 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 34 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino"
 "Strategy (Mode Library): Finished"
# 34 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino" 3
 ); &__c[0];}))
# 34 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 34 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 34 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino"
 "Strategy (Mode Library): Finished"
# 34 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino" 3
 ); &__c[0];}))
# 34 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino"
 )));

  DetachModeButton();
  DetachSelectButton();

  // Assign mode button interrupt
  AttachModeButton();
}

// Select button function
void SelectFunctionModeLibrary() {
  if (millis() - lastMillisSelect > 500) {
    lastMillisSelect = millis();

    delay(1);
    if (digitalRead(18)) {
      return;
    }

    // Select Mode
    BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 54 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 54 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino"
   "Mode Selected."
# 54 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino" 3
   ); &__c[0];}))
# 54 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino"
   ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 54 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 54 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino"
   "Mode Selected."
# 54 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino" 3
   ); &__c[0];}))
# 54 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino"
   )));
    SetMode(modeCycle);
  }
}

// Mode button function
void ModeFunctionModeLibrary() {
  if (millis() - lastMillisMode > 500) {
    lastMillisMode = millis();

    lastMillistModeBlink = 0;

    delay(1);
    if (digitalRead(19)) {
      return;
    }


    // Cycle Mode
    if (modeCycle + 1 < 2 /* Minimum mode index, that could be set via mode button.*/ || modeCycle + 1 >= 6 /* Total number of modes*/) {
      modeCycle = 2 /* Minimum mode index, that could be set via mode button.*/;
    } else {
      modeCycle = modeCycle + 1;
    }

    BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 79 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 79 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino"
   "Mode Cycle: "
# 79 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino" 3
   ); &__c[0];}))
# 79 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino"
   ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 79 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 79 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino"
   "Mode Cycle: "
# 79 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino" 3
   ); &__c[0];}))
# 79 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_ModeLibrary.ino"
   )));
    BlackBoxAppendln(((modeCycle) == 0 ? "Emergency" : ((modeCycle) == 1 ? "Mode Library" : ((modeCycle) == 2 ? "Idle" : ((modeCycle) == 3 ? "System Test" : ((modeCycle) == 4 ? "Remote Control" : ((modeCycle) == 5 ? "Autonomous" : "Unknown"))))))); Serial.println(((modeCycle) == 0 ? "Emergency" : ((modeCycle) == 1 ? "Mode Library" : ((modeCycle) == 2 ? "Idle" : ((modeCycle) == 3 ? "System Test" : ((modeCycle) == 4 ? "Remote Control" : ((modeCycle) == 5 ? "Autonomous" : "Unknown")))))));

    ResetLed();
  }
}
# 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
/*  Strategy - Remote control



    Allows manual control of motors

*/
# 5 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
bool remoteActive, remoteStart;

// Start sequence of strategy
void StartStrategyRemote() {
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
 "------------------------------------"
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
 ); &__c[0];}))
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
 "------------------------------------"
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
 ); &__c[0];}))
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
 )));
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 10 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 10 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
 "Strategy (Remote): Starting"
# 10 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
 ); &__c[0];}))
# 10 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 10 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 10 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
 "Strategy (Remote): Starting"
# 10 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
 ); &__c[0];}))
# 10 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
 )));
  SystemDisable();

  remoteActive = false;
  remoteStart = false;

  AttachSelectButton();

  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 18 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 18 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
 "Strategy (Remote): Initialized"
# 18 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
 ); &__c[0];}))
# 18 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 18 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 18 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
 "Strategy (Remote): Initialized"
# 18 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
 ); &__c[0];}))
# 18 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
 )));
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 19 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 19 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
 "------------------------------------"
# 19 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
 ); &__c[0];}))
# 19 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 19 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 19 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
 "------------------------------------"
# 19 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
 ); &__c[0];}))
# 19 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
 )));

  StrategyStartLed();
}

// Main sequence of strategy
void RunStrategyRemote() {
  if (!remoteActive) {
    SystemDisable();
    StrategyRunLed(2 /* Standby mode*/);
    return;
  }

  if (remoteStart) {
    remoteStart = false;
    BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 34 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 34 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
   "------------------------------------"
# 34 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
   ); &__c[0];}))
# 34 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
   ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 34 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 34 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
   "------------------------------------"
# 34 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
   ); &__c[0];}))
# 34 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
   )));
    BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 35 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 35 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
   "Starting Remote Control"
# 35 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
   ); &__c[0];}))
# 35 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
   ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 35 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 35 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
   "Starting Remote Control"
# 35 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
   ); &__c[0];}))
# 35 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
   )));
    BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 36 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 36 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
   "------------------------------------"
# 36 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
   ); &__c[0];}))
# 36 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
   ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 36 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 36 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
   "------------------------------------"
# 36 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
   ); &__c[0];}))
# 36 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
   )));
    if(!SystemEnableMode()){
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 38 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 38 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
     "------------------------------------"
# 38 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
     ); &__c[0];}))
# 38 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 38 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 38 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
     "------------------------------------"
# 38 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
     ); &__c[0];}))
# 38 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
     )));
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 39 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 39 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
     "Module Start Error"
# 39 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
     ); &__c[0];}))
# 39 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 39 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 39 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
     "Module Start Error"
# 39 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
     ); &__c[0];}))
# 39 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
     )));
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 40 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 40 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
     "------------------------------------"
# 40 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
     ); &__c[0];}))
# 40 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 40 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 40 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
     "------------------------------------"
# 40 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
     ); &__c[0];}))
# 40 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
     )));
      RemoteHalt();
      return;
    }
  }

  if (!SystemCheckMode(4 /* System remotely controllable*/)) {
    RemoteHalt();
    return;
  }

  StrategyRunLed(5 /* Autonomous driving mode*/);

  // Read RF signal
  SBusProcess();
  if (!SBusProcess()) {
    RemoteHalt();
    return;
  }
  SBusPrint();

  // Transmit via CAN
  CanBusProcess();
  if (!CanBusProcess()) {
    RemoteHalt();
    return;
  }
  CanBusPrint();
}

// End sequence of strategy
void FinishStrategyRemote() {
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 72 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 72 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
 "------------------------------------"
# 72 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
 ); &__c[0];}))
# 72 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 72 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 72 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
 "------------------------------------"
# 72 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
 ); &__c[0];}))
# 72 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
 )));
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 73 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 73 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
 "Strategy (Remote): Ending"
# 73 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
 ); &__c[0];}))
# 73 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 73 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 73 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
 "Strategy (Remote): Ending"
# 73 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
 ); &__c[0];}))
# 73 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
 )));

  DetachSelectButton();

  SystemDisable();

  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 79 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 79 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
 "Strategy (Remote): Finished"
# 79 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
 ); &__c[0];}))
# 79 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 79 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 79 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
 "Strategy (Remote): Finished"
# 79 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino" 3
 ); &__c[0];}))
# 79 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_Remote.ino"
 )));
}

// Select button function
void SelectFunctionRemote() {
  if (millis() - lastMillisSelect > 500) {
    lastMillisSelect = millis();
    lastSystemCheck = millis() - 30000;
    remoteActive = !remoteActive;
    remoteStart = true;
  }
}

void RemoteHalt() {
  remoteActive = false;
  SystemDisable();
  StatusHaltLed(LED_SIGNAL_ERROR);
}
# 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino"
/*  Strategy - System Test mode



    Runs full system test ...

*/
# 5 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino"
bool runTest = false;

// Start sequence of strategy
void StartStrategySystemTest() {
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino"
 "------------------------------------"
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino" 3
 ); &__c[0];}))
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino"
 "------------------------------------"
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino" 3
 ); &__c[0];}))
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino"
 )));
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 10 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 10 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino"
 "Strategy (System Test): Starting"
# 10 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino" 3
 ); &__c[0];}))
# 10 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 10 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 10 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino"
 "Strategy (System Test): Starting"
# 10 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino" 3
 ); &__c[0];}))
# 10 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino"
 )));
  SystemDisable();

  AttachSelectButton();

  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 15 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 15 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino"
 "Strategy (System Test): Initialized"
# 15 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino" 3
 ); &__c[0];}))
# 15 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 15 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 15 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino"
 "Strategy (System Test): Initialized"
# 15 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino" 3
 ); &__c[0];}))
# 15 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino"
 )));
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 16 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 16 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino"
 "------------------------------------"
# 16 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino" 3
 ); &__c[0];}))
# 16 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 16 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 16 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino"
 "------------------------------------"
# 16 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino" 3
 ); &__c[0];}))
# 16 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino"
 )));
  StrategyStartLed();
}

// Main sequence of strategy
void RunStrategySystemTest() {
  // Blink light
  StrategyRunLed();

  if (runTest) {
    runTest = !SystemTest();
  }
}

// End sequence of strategy
void FinishStrategySystemTest() {
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 32 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 32 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino"
 "------------------------------------"
# 32 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino" 3
 ); &__c[0];}))
# 32 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 32 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 32 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino"
 "------------------------------------"
# 32 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino" 3
 ); &__c[0];}))
# 32 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino"
 )));
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 33 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 33 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino"
 "Strategy (System Test): Ending"
# 33 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino" 3
 ); &__c[0];}))
# 33 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 33 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 33 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino"
 "Strategy (System Test): Ending"
# 33 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino" 3
 ); &__c[0];}))
# 33 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino"
 )));

  DetachSelectButton();
  SystemDisable();

  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 38 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 38 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino"
 "Strategy (System Test): Finished"
# 38 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino" 3
 ); &__c[0];}))
# 38 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 38 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 38 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino"
 "Strategy (System Test): Finished"
# 38 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino" 3
 ); &__c[0];}))
# 38 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Str_SystemTest.ino"
 )));
}


// Selecet button function
void SelectFunctionSystemTest() {
  if (millis() - lastMillisSelect > 500) {
    systemTestState = 0;
    lastMillisSelect = millis();
    runTest = true;
    DetachSelectButton();
  }
}
# 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Autonomy.ino"
/*

  GeoRover Autonomy Stack



  Mads Rosenhøj Jeppesen

  Aarhus University

  2021

*/
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Autonomy.ino"
bool autonomyStart, autonomyRestart, autonomyActive, standbyMode;
unsigned long millisLastAutonomyCycle;

// Primary Autonomous Algorithm
void AutonomousProcess() {
  if (!AutonomyPreRun()) return;

  if (!AutonomyPowerCycle()) return;

  if (!StandbyRestart()) return;

  if (!CanBusProcess()) return; // TODO: Handle CAN Error

  if (!Navigate()) return; // TODO: Handle run-check Error
}

// Runs necessary autonomy pre-runs (Power, Communication, Navigation)
bool AutonomyPreRun() {
  if (autonomyActive) return true;

  if (!AutonomyPreCheck()) return false;

  // System ready to start, waiting idle
  if (!autonomyActive) {
    StatusRunLed(LED_SIGNAL_LOADING);
    return false;
  }

  // Select button pressed, start navigation
  if (autonomyStart) {
    autonomyStart = false;
    millisAutonomyStart = millis();
    BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 41 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Autonomy.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 41 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Autonomy.ino"
   "------------------------------------"
# 41 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Autonomy.ino" 3
   ); &__c[0];}))
# 41 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Autonomy.ino"
   ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 41 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Autonomy.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 41 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Autonomy.ino"
   "------------------------------------"
# 41 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Autonomy.ino" 3
   ); &__c[0];}))
# 41 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Autonomy.ino"
   )));
    BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 42 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Autonomy.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 42 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Autonomy.ino"
   "Starting Autonomous Driving"
# 42 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Autonomy.ino" 3
   ); &__c[0];}))
# 42 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Autonomy.ino"
   ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 42 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Autonomy.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 42 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Autonomy.ino"
   "Starting Autonomous Driving"
# 42 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Autonomy.ino" 3
   ); &__c[0];}))
# 42 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Autonomy.ino"
   )));
    BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 43 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Autonomy.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 43 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Autonomy.ino"
   "------------------------------------"
# 43 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Autonomy.ino" 3
   ); &__c[0];}))
# 43 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Autonomy.ino"
   ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 43 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Autonomy.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 43 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Autonomy.ino"
   "------------------------------------"
# 43 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Autonomy.ino" 3
   ); &__c[0];}))
# 43 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Autonomy.ino"
   )));
  }

  return true;
}

// Run power cycle, check if levels below thresholds or restart autonomy.
bool AutonomyPowerCycle() {
  if (millis() - millisLastAutonomyCycle < 10000) return standbyMode;

  millisLastAutonomyCycle = millis();

  // Check power levels
  if (!PowerCycle()) {
    if (standbyMode) return false;
    AutonomyStandby();
    return false;
  }

  if (!standbyMode) {
    return true;
  }

  // Power levels sufficient, restart systems if previously powered down
  if (!StandbyRestart()) return false;

  standbyMode = false;
  return true;
}

// Runs necessary system pre-checks (enables systems, run Navigation pre-check)
bool AutonomyPreCheck() {
  // Enables primary and secondary systems
  if (!SystemEnableMode()) {
    autonomyActive = false;
    StatusHaltLed(LED_SIGNAL_ERROR);
    return false;
  }

  // Runs navigation route checks
  if (!NavigationPreCheck()) {
    return false;
  }
  return true;
}


// Inverts current autonomy state
void AutonomyToggle() {
  autonomyActive = !autonomyActive;
  autonomyStart = autonomyActive;
  NavigationPreCheckReset();
}

// Full autonomy reset
void AutonomyReset() {
  NavigationPreCheckReset();
  ResetNavigation();
  ResetPowerCycle();
  autonomyActive = false;
  autonomyStart = false;
}

// Power down system
void AutonomyStandby() {
  standbyMode = true;
  autonomyRestart = true;
  MotorUpdate(0, 0);
  MotorCycle();
  SystemDisable();
}

// Power levels acceptable, if coming out of standby, reboot systems and await PreCheck flag
bool StandbyRestart() {
  if (autonomyRestart) {
    autonomyRestart = false;
    NavigationPreCheckReset();
  }

  if (!AutonomyPreCheck()) {
    return false;
  }
  return true;
}
# 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
/*

  GeoRover Blackbox protocols



  Mads Rosenhøj Jeppesen

  Aarhus University

  2021

*/
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
bool newLine = true;

// Initialize system Blackbox
bool InitBlackBox() {
  if (!SDReaderStatus()) {
    return false;
  }

  File blackBoxFile = SD.open("Blackbox.csv", (O_READ | O_WRITE | O_CREAT | O_APPEND));
  if (blackBoxFile) {
    SetStatus(15, true);
  } else {
    SetStatus(15, false);
  }
  blackBoxFile.close();

  BlackBoxAppend("GeoRover Blackbox ");
  BlackBoxAppend("System Version: ");
  BlackBoxAppendln("0.91.126");

  return BlackBoxStatus;
}

void BlackBoxPrint() {
  if (SDReaderStatus()) {
    BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 34 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 34 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
   "Printing Blackbox: "
# 34 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino" 3
   ); &__c[0];}))
# 34 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
   ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 34 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 34 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
   "Printing Blackbox: "
# 34 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino" 3
   ); &__c[0];}))
# 34 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
   )));
    File file = SD.open("Blackbox.csv");
    if (file) {
      while (file.available()) {
        Serial.write(file.read());
      }
      file.close();
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 41 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 41 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
     "End of File"
# 41 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino" 3
     ); &__c[0];}))
# 41 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 41 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 41 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
     "End of File"
# 41 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino" 3
     ); &__c[0];}))
# 41 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
     )));
    } else {
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 43 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 43 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
     "File not found!"
# 43 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino" 3
     ); &__c[0];}))
# 43 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 43 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 43 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
     "File not found!"
# 43 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino" 3
     ); &__c[0];}))
# 43 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
     )));
    }
  }
}

void BlackBoxClear() {
  if (SD.exists("Blackbox.csv")) {
    BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 50 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 50 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
   "BLACKBOX CLEARED"
# 50 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino" 3
   ); &__c[0];}))
# 50 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
   ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 50 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 50 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
   "BLACKBOX CLEARED"
# 50 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino" 3
   ); &__c[0];}))
# 50 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
   )));
    SD.remove("Blackbox.csv");
    InitBlackBox();
  } else {
    BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 54 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 54 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
   "BLACKBOX ERROR"
# 54 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino" 3
   ); &__c[0];}))
# 54 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
   ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 54 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 54 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
   "BLACKBOX ERROR"
# 54 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino" 3
   ); &__c[0];}))
# 54 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
   )));
  }
}

bool BlackBoxStatus() {
  return GetStatus(15);
}

void BlackBoxAppend(String blackBoxInput) {
  if (BlackBoxStatus()) {
    File blackBoxFile = SD.open("Blackbox.csv", (O_READ | O_WRITE | O_CREAT | O_APPEND));
    if (blackBoxFile) {
      if (newLine) {
        blackBoxFile.print((String)millis());
        blackBoxFile.print((reinterpret_cast<const __FlashStringHelper *>(
# 68 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino" 3
                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 68 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
                          ";"
# 68 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino" 3
                          ); &__c[0];}))
# 68 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
                          )));
        newLine = false;
      }
      blackBoxFile.print(blackBoxInput);
      blackBoxFile.close();
    } else {
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 74 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 74 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
     "Blackbox Error"
# 74 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino" 3
     ); &__c[0];}))
# 74 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 74 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 74 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
     "Blackbox Error"
# 74 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino" 3
     ); &__c[0];}))
# 74 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
     )));

      SetStatus(15, false);
    }
  }
}

void BlackBoxAppendln() {
  BlackBoxAppend("\r\n");
  newLine = true;
}

void BlackBoxAppendln(String blackBoxInput) {
  BlackBoxAppend(blackBoxInput);
  BlackBoxAppendln();
}

void BlackBoxAppend(int blackBoxInput) {
  BlackBoxAppend((String)blackBoxInput);
}

void BlackBoxAppendln(int blackBoxInput) {
  BlackBoxAppend((String)blackBoxInput);
  BlackBoxAppendln();
}

void BlackBoxAppend(long int blackBoxInput, int type) {
  if (BlackBoxStatus()) {
    File blackBoxFile = SD.open("Blackbox.csv", (O_READ | O_WRITE | O_CREAT | O_APPEND));
    if (blackBoxFile) {
      if (newLine) {
        blackBoxFile.print(millis());
        blackBoxFile.print((reinterpret_cast<const __FlashStringHelper *>(
# 106 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino" 3
                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 106 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
                          ";"
# 106 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino" 3
                          ); &__c[0];}))
# 106 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
                          )));
        newLine = false;
      }
      blackBoxFile.print(blackBoxInput, type);
      blackBoxFile.close();
    } else {
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 112 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 112 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
     "Blackbox Error"
# 112 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino" 3
     ); &__c[0];}))
# 112 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 112 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 112 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
     "Blackbox Error"
# 112 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino" 3
     ); &__c[0];}))
# 112 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Blackbox.ino"
     )));

      SetStatus(15, false);
    }
  }
}

void BlackBoxAppendln(long int blackBoxInput, int type) {
  BlackBoxAppend(blackBoxInput, type);
  BlackBoxAppendln();
}

void BlackBoxAppendln(float blackBoxInput) {
  BlackBoxAppend((String)blackBoxInput);
  BlackBoxAppendln();
}

void BlackBoxAppend(float blackBoxInput) {
  BlackBoxAppend((String)blackBoxInput);
}

void BlackBoxAppend(long int blackBoxInput) {
  BlackBoxAppend((String)blackBoxInput);
}

void BlackBoxAppendln(long int blackBoxInput) {
  BlackBoxAppend((String)blackBoxInput);
  BlackBoxAppendln();
}

void BlackBoxAppendln(unsigned long blackBoxInput) {
  BlackBoxAppend((String)blackBoxInput);
  BlackBoxAppendln();
}

void BlackBoxAppend(unsigned long blackBoxInput) {
  BlackBoxAppend((String)blackBoxInput);
}
# 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
/*

  GeoRover System Control



  Mads Rosenhøj Jeppesen

  Aarhus University

  2021

*/
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
bool SystemEnable(int module) {
  if (GetStatus(module)) return true;
  BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 11 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 11 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
 "SYS Enable: "
# 11 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
 ); &__c[0];}))
# 11 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
 ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 11 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 11 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
 "SYS Enable: "
# 11 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
 ); &__c[0];}))
# 11 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
 )));
  BlackBoxAppend(((module) == 0 ? "Primary Power" : ((module) == 1 ? "Secondary Power (5V)" : ((module) == 2 ? "Secondary Power (12V)" : ((module) == 3 ? "Secondary Power (24V)" : ((module) == 4 ? "Secondary Power (Motors)" : ((module) == 5 ? "Motors" : ((module) == 6 ? "Motor Left" : ((module) == 7 ? "Motor Right" : ((module) == 8 ? "Motor Active" : ((module) == 9 ? "CAN-bus" : ((module) == 10 ? "Short Range Communication (RF)" : ((module) == 11 ? "Long Range Communication (Iridium)" : ((module) == 12 ? "Global Navigation Satellite System" : ((module) == 13 ? "Accelerometer" : ((module) == 14 ? "Local Storage" : ((module) == 15 ? "Blackbox" : ((module) == 16 ? "Waypoint Route" : ((module) == 17 ? "Debug Comm." : ((module) == 18 ? "System LED" : ((module) == 19 ? "Heating" : ((module) == 20 ? "Temperature" : ((module) == 21 ? "Backup CPU" : ((module) == 22 ? "Emergency Stop" : ((module) == 23 ? "Reserved" : ("Unknown")))))))))))))))))))))))))); Serial.print(((module) == 0 ? "Primary Power" : ((module) == 1 ? "Secondary Power (5V)" : ((module) == 2 ? "Secondary Power (12V)" : ((module) == 3 ? "Secondary Power (24V)" : ((module) == 4 ? "Secondary Power (Motors)" : ((module) == 5 ? "Motors" : ((module) == 6 ? "Motor Left" : ((module) == 7 ? "Motor Right" : ((module) == 8 ? "Motor Active" : ((module) == 9 ? "CAN-bus" : ((module) == 10 ? "Short Range Communication (RF)" : ((module) == 11 ? "Long Range Communication (Iridium)" : ((module) == 12 ? "Global Navigation Satellite System" : ((module) == 13 ? "Accelerometer" : ((module) == 14 ? "Local Storage" : ((module) == 15 ? "Blackbox" : ((module) == 16 ? "Waypoint Route" : ((module) == 17 ? "Debug Comm." : ((module) == 18 ? "System LED" : ((module) == 19 ? "Heating" : ((module) == 20 ? "Temperature" : ((module) == 21 ? "Backup CPU" : ((module) == 22 ? "Emergency Stop" : ((module) == 23 ? "Reserved" : ("Unknown"))))))))))))))))))))))))));

  bool status = true;

  switch (module) {
    case 0:
      status = BatteryStatus();
      break;
    case 1:
      digitalWrite(42 /* External 5V power */, 0x1);
      break;
    case 2:
      digitalWrite(40 /* External 12V power */, 0x1);
      break;
    case 3:
      digitalWrite(38 /* External 24V power */, 0x1);
      break;
    case 4:
      digitalWrite(36 /* Relay to motor battery power*/, 0x1);
      break;
    case 5:
      status = InitializeMotors();
      break;
    case 6:
      status = MotorStatusLeft();
      break;
    case 7:
      status = MotorStatusRight();
      break;
    case 8:
      status = MotorState();
      break;
    case 9:
      status = InitializeCanBus();
      break;
    case 10:
      status = InitializeSBUS();
      break;
    case 11:
      digitalWrite(46, 0x1);
      status = InitializeIridium();
      break;
    case 13:
      status = InitializeAccel();
      break;
    case 12:
      status = InitializeGnss();
      break;
    case 14:
      status = InitializeSDReader();
      break;
    case 15:
      status = InitBlackBox();
      break;
    case 16:
      status = RouteCheck();
      break;
    case 19:
      status = HeatingStart();
      break;
    case 20:
      status = TemperatureStatus();
      break;
    case 21:
      status = GetStatus(21);
      break;
    case 18:
      status = true;
      break;
    default:
      BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 82 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 82 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     "- UNKNOWN Case"
# 82 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     ); &__c[0];}))
# 82 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 82 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 82 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     "- UNKNOWN Case"
# 82 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     ); &__c[0];}))
# 82 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     )));
      break;
  }

  if (status) {
    BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 87 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 87 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
   ": Enabled"
# 87 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
   ); &__c[0];}))
# 87 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
   ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 87 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 87 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
   ": Enabled"
# 87 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
   ); &__c[0];}))
# 87 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
   )));
  } else {
    BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 89 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 89 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
   ": ERROR"
# 89 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
   ); &__c[0];}))
# 89 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
   ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 89 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 89 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
   ": ERROR"
# 89 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
   ); &__c[0];}))
# 89 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
   )));
  }
  SetStatus(module, status);
  delay(5);
  return status;
}

// Enables Primary Systems
void SystemEnablePrimary() {
  SystemEnable(0);
  SystemEnable(14);
  SystemEnable(15);
  SystemEnable(12);
  SystemEnable(11);
}

// Enables module then flash LED indicating success or error
bool SystemSignalEnable(byte module) {
  bool status = SystemEnable(module);
  if (status) {
    StatusHaltLed(LED_SIGNAL_OK_SHORT_HALT);
  } else {
    StatusHaltLed(LED_SIGNAL_ERROR_SHORT_HALT);
  }
  return status;
}

// Enable systems for current mode
bool SystemEnableMode() {
  int modules = 0;
  bool status = true;
  bool enableStatus[10];
  switch (mode) {
    case 4 /* System remotely controllable*/:
      enableStatus[0] = SystemEnable(0);
      enableStatus[1] = SystemEnable(9);
      enableStatus[2] = SystemEnable(4);
      enableStatus[3] = SystemEnable(5);
      enableStatus[4] = SystemEnable(10);
      modules = 5;
      break;

    case 5 /* Autonomous driving mode*/:
      enableStatus[0] = SystemEnable(0);
      enableStatus[1] = SystemEnable(9);
      enableStatus[2] = SystemEnable(4);
      enableStatus[3] = SystemEnable(5);
      enableStatus[4] = SystemEnable(13);
      enableStatus[5] = SystemEnable(10);
      modules = 6;
      break;

    default:
      status = false;
      break;
  }

  for (int i = 0; i < modules; i++) {
    if (!SystemEnableStatus(enableStatus[i])) {
      status = false;
    }
  }
  return status;
}

// Sets navigationPreCheck flag to false and signals
bool SystemEnableStatus(bool status) {
  if (status) {
    StatusHaltLed(LED_SIGNAL_OK_SHORT_HALT);
  } else {
    StatusHaltLed(LED_SIGNAL_ERROR_SHORT_HALT);
  }
  return status;
}

// Disables specified module
void SystemDisable(int module) {
  if (!GetStatus(module)) return;
  bool status = false;
  BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 168 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 168 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
 "SYS Disable: "
# 168 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
 ); &__c[0];}))
# 168 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
 ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 168 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 168 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
 "SYS Disable: "
# 168 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
 ); &__c[0];}))
# 168 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
 )));
  BlackBoxAppend(((module) == 0 ? "Primary Power" : ((module) == 1 ? "Secondary Power (5V)" : ((module) == 2 ? "Secondary Power (12V)" : ((module) == 3 ? "Secondary Power (24V)" : ((module) == 4 ? "Secondary Power (Motors)" : ((module) == 5 ? "Motors" : ((module) == 6 ? "Motor Left" : ((module) == 7 ? "Motor Right" : ((module) == 8 ? "Motor Active" : ((module) == 9 ? "CAN-bus" : ((module) == 10 ? "Short Range Communication (RF)" : ((module) == 11 ? "Long Range Communication (Iridium)" : ((module) == 12 ? "Global Navigation Satellite System" : ((module) == 13 ? "Accelerometer" : ((module) == 14 ? "Local Storage" : ((module) == 15 ? "Blackbox" : ((module) == 16 ? "Waypoint Route" : ((module) == 17 ? "Debug Comm." : ((module) == 18 ? "System LED" : ((module) == 19 ? "Heating" : ((module) == 20 ? "Temperature" : ((module) == 21 ? "Backup CPU" : ((module) == 22 ? "Emergency Stop" : ((module) == 23 ? "Reserved" : ("Unknown")))))))))))))))))))))))))); Serial.print(((module) == 0 ? "Primary Power" : ((module) == 1 ? "Secondary Power (5V)" : ((module) == 2 ? "Secondary Power (12V)" : ((module) == 3 ? "Secondary Power (24V)" : ((module) == 4 ? "Secondary Power (Motors)" : ((module) == 5 ? "Motors" : ((module) == 6 ? "Motor Left" : ((module) == 7 ? "Motor Right" : ((module) == 8 ? "Motor Active" : ((module) == 9 ? "CAN-bus" : ((module) == 10 ? "Short Range Communication (RF)" : ((module) == 11 ? "Long Range Communication (Iridium)" : ((module) == 12 ? "Global Navigation Satellite System" : ((module) == 13 ? "Accelerometer" : ((module) == 14 ? "Local Storage" : ((module) == 15 ? "Blackbox" : ((module) == 16 ? "Waypoint Route" : ((module) == 17 ? "Debug Comm." : ((module) == 18 ? "System LED" : ((module) == 19 ? "Heating" : ((module) == 20 ? "Temperature" : ((module) == 21 ? "Backup CPU" : ((module) == 22 ? "Emergency Stop" : ((module) == 23 ? "Reserved" : ("Unknown"))))))))))))))))))))))))));

  switch (module) {
    case 0:
      status = BatteryStatus();
      break;
    case 1:
      digitalWrite(42 /* External 5V power */, 0x0);
      break;
    case 2:
      digitalWrite(40 /* External 12V power */, 0x0);
      break;
    case 3:
      digitalWrite(38 /* External 24V power */, 0x0);
      break;
    case 4:
      digitalWrite(36 /* Relay to motor battery power*/, 0x0);
      break;
    case 5:
      TerminateMotors();
      break;
    case 6:
      status = MotorStatusLeft();
      break;
    case 7:
      status = MotorStatusRight();
      break;
    case 8:
      status = MotorState();
      break;
    case 9:
      TerminateCanBus();
      break;
    case 10:
      TerminateSBUS();
      digitalWrite(44, 0x0);
      break;
    case 11:
      TerminateIridium();
      digitalWrite(46, 0x0);
      break;
    case 12:
      TerminateGnss();
      break;
    case 13:
      TerminateAccel();
      break;
    case 14:
      status = SDReaderStatus();
      break;
    case 15:
      status = BlackBoxStatus();
      break;
    case 17:
      status = DebugCommStatus();
      break;
    case 18:
      status = LedStatus();
      break;
    case 19:
      HeatingStop();
      break;
    case 20:
      status = TemperatureStatus();
      break;
    case 21:
      status = HeartBeatStatus();
      break;
    case 22:
      status = EmergencyStopStatus();
      break;
    case 23:
      status = true;
      break;
    default:
      BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 244 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 244 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     " - Unknown Case"
# 244 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     ); &__c[0];}))
# 244 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 244 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 244 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     " - Unknown Case"
# 244 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     ); &__c[0];}))
# 244 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     )));
      BlackBoxAppendln(((module) == 0 ? "Primary Power" : ((module) == 1 ? "Secondary Power (5V)" : ((module) == 2 ? "Secondary Power (12V)" : ((module) == 3 ? "Secondary Power (24V)" : ((module) == 4 ? "Secondary Power (Motors)" : ((module) == 5 ? "Motors" : ((module) == 6 ? "Motor Left" : ((module) == 7 ? "Motor Right" : ((module) == 8 ? "Motor Active" : ((module) == 9 ? "CAN-bus" : ((module) == 10 ? "Short Range Communication (RF)" : ((module) == 11 ? "Long Range Communication (Iridium)" : ((module) == 12 ? "Global Navigation Satellite System" : ((module) == 13 ? "Accelerometer" : ((module) == 14 ? "Local Storage" : ((module) == 15 ? "Blackbox" : ((module) == 16 ? "Waypoint Route" : ((module) == 17 ? "Debug Comm." : ((module) == 18 ? "System LED" : ((module) == 19 ? "Heating" : ((module) == 20 ? "Temperature" : ((module) == 21 ? "Backup CPU" : ((module) == 22 ? "Emergency Stop" : ((module) == 23 ? "Reserved" : ("Unknown")))))))))))))))))))))))))); Serial.println(((module) == 0 ? "Primary Power" : ((module) == 1 ? "Secondary Power (5V)" : ((module) == 2 ? "Secondary Power (12V)" : ((module) == 3 ? "Secondary Power (24V)" : ((module) == 4 ? "Secondary Power (Motors)" : ((module) == 5 ? "Motors" : ((module) == 6 ? "Motor Left" : ((module) == 7 ? "Motor Right" : ((module) == 8 ? "Motor Active" : ((module) == 9 ? "CAN-bus" : ((module) == 10 ? "Short Range Communication (RF)" : ((module) == 11 ? "Long Range Communication (Iridium)" : ((module) == 12 ? "Global Navigation Satellite System" : ((module) == 13 ? "Accelerometer" : ((module) == 14 ? "Local Storage" : ((module) == 15 ? "Blackbox" : ((module) == 16 ? "Waypoint Route" : ((module) == 17 ? "Debug Comm." : ((module) == 18 ? "System LED" : ((module) == 19 ? "Heating" : ((module) == 20 ? "Temperature" : ((module) == 21 ? "Backup CPU" : ((module) == 22 ? "Emergency Stop" : ((module) == 23 ? "Reserved" : ("Unknown"))))))))))))))))))))))))));
      break;
  }

  if (status) {
    BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 250 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 250 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
   "Unable to Disable"
# 250 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
   ); &__c[0];}))
# 250 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
   ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 250 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 250 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
   "Unable to Disable"
# 250 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
   ); &__c[0];}))
# 250 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
   )));
  } else {
    BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 252 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 252 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
   "- Disabled"
# 252 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
   ); &__c[0];}))
# 252 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
   ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 252 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 252 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
   "- Disabled"
# 252 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
   ); &__c[0];}))
# 252 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
   )));
  }

  SetStatus(module, status);
}

// Disable all secondary systems
void SystemDisable() {
  SystemDisable(10);
  // SystemDisable(MODULE_IRIDIUM);
  // SystemDisable(MODULE_GNSS);
  SystemDisable(13);
  SystemDisable(4);
  SystemDisable(5);
  SystemDisable(9);
  SystemDisable(4);
  SystemDisable(1);
  SystemDisable(2);
  SystemDisable(3);
}

// Runs system check if time since last check > timeout, returns comparison between required modules to current
bool SystemCheckMode(int mode) {
  static bool status = true;

  if (millis() - lastSystemCheck < 30000) {
    return status;
  }

  lastSystemCheck = millis();
  SystemCheck();

  switch (mode) {
    case 4 /* System remotely controllable*/:
      status = ((((ToLong(SystemStatus) ^ SYSREQ_REMOTE_CONTROL) &
                  SYSREQ_REMOTE_CONTROL) |
                 (1L << 23)) == (1L << 23));
      if (!status) {
        BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 290 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
       (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 290 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
       "ERROR Code: "
# 290 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
       ); &__c[0];}))
# 290 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
       ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 290 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
       (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 290 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
       "ERROR Code: "
# 290 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
       ); &__c[0];}))
# 290 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
       )));
        BlackBoxAppendln(String(((ToLong(SystemStatus) ^ SYSREQ_REMOTE_CONTROL) & SYSREQ_REMOTE_CONTROL) | (1L << 23))); Serial.println(String(((ToLong(SystemStatus) ^ SYSREQ_REMOTE_CONTROL) & SYSREQ_REMOTE_CONTROL) | (1L << 23)))

                                                      ;
      }
      break;
    case 5 /* Autonomous driving mode*/:
      status =
          ((((ToLong(SystemStatus) ^ SYSREQ_AUTONOMOUS) & SYSREQ_AUTONOMOUS) |
            (1L << 23)) == (1L << 23));
      if (!status) {
        BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 301 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
       (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 301 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
       "ERROR Code: "
# 301 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
       ); &__c[0];}))
# 301 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
       ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 301 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
       (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 301 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
       "ERROR Code: "
# 301 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
       ); &__c[0];}))
# 301 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
       )));
        BlackBoxAppendln(((ToLong(SystemStatus) ^ SYSREQ_REMOTE_CONTROL) & SYSREQ_REMOTE_CONTROL) | (1L << 23)); Serial.println(((ToLong(SystemStatus) ^ SYSREQ_REMOTE_CONTROL) & SYSREQ_REMOTE_CONTROL) | (1L << 23))

                                              ;
      }
      break;
    default:
      break;
  }

  return status;
}

// Run full system test
bool SystemTest() {
  unsigned long testResults;
  bool testDone = false;
  switch (systemTestState) {
    case 0:
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 320 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 320 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     "Running Full System Test"
# 320 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     ); &__c[0];}))
# 320 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 320 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 320 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     "Running Full System Test"
# 320 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     ); &__c[0];}))
# 320 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     )));
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 321 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 321 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     "------------------------------------"
# 321 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     ); &__c[0];}))
# 321 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 321 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 321 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     "------------------------------------"
# 321 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     ); &__c[0];}))
# 321 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     )));
      systemTestState++;
      break;
    case 1:
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 325 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 325 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     "Running test (1/4) - Power Systems"
# 325 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     ); &__c[0];}))
# 325 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 325 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 325 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     "Running test (1/4) - Power Systems"
# 325 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     ); &__c[0];}))
# 325 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     )));
      SystemTestModule(0);
      SystemTestModule(1);
      SystemTestModule(2);
      SystemTestModule(3);
      SystemTestModule(4);
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 331 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 331 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     "Test (1/4) - Power Systems (Complete)"
# 331 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     ); &__c[0];}))
# 331 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 331 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 331 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     "Test (1/4) - Power Systems (Complete)"
# 331 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     ); &__c[0];}))
# 331 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     )));
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 332 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 332 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     "------------------------------------"
# 332 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     ); &__c[0];}))
# 332 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 332 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 332 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     "------------------------------------"
# 332 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     ); &__c[0];}))
# 332 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     )));
      systemTestState++;
      break;
    case 2:
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 336 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 336 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     "Running test (2/4) - Communication"
# 336 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     ); &__c[0];}))
# 336 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 336 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 336 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     "Running test (2/4) - Communication"
# 336 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     ); &__c[0];}))
# 336 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     )));
      systemTestState++;
      break;
    case 3:
      if (SystemTestModule(10)) systemTestState++;
      break;
    case 4:
      if (SystemTestModule(11)) systemTestState++;
      break;
    case 5:
      if (SystemTestModule(12)) systemTestState++;
      break;
    case 6:
      if (SystemTestModule(21)) systemTestState++;
      break;
    case 7:
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 352 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 352 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     "Test (2/4) - Communication (Complete)"
# 352 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     ); &__c[0];}))
# 352 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 352 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 352 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     "Test (2/4) - Communication (Complete)"
# 352 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     ); &__c[0];}))
# 352 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     )));
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 353 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 353 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     "------------------------------------"
# 353 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     ); &__c[0];}))
# 353 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 353 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 353 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     "------------------------------------"
# 353 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     ); &__c[0];}))
# 353 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     )));
      systemTestState++;
      break;
    case 8:
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 357 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 357 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     "Running test (3/4) - Subsystems"
# 357 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     ); &__c[0];}))
# 357 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 357 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 357 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     "Running test (3/4) - Subsystems"
# 357 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     ); &__c[0];}))
# 357 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     )));
      systemTestState++;
      break;
    case 9:
      if (SystemTestModule(13)) systemTestState++;
      break;
    case 10:
      if (SystemTestModule(14)) systemTestState++;
      break;
    case 11:
      if (SystemTestModule(15)) systemTestState++;
      break;
    case 12:
      if (SystemTestModule(20)) systemTestState++;
      break;
    case 13:
      if (SystemTestModule(18)) systemTestState++;
      break;
    case 14:
      if (SystemTestModule(16)) systemTestState++;
      break;
    case 15:
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 379 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 379 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     "Test (3/4) - Subsystems (Complete)"
# 379 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     ); &__c[0];}))
# 379 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 379 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 379 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     "Test (3/4) - Subsystems (Complete)"
# 379 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     ); &__c[0];}))
# 379 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     )));
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 380 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 380 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     "------------------------------------"
# 380 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     ); &__c[0];}))
# 380 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 380 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 380 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     "------------------------------------"
# 380 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     ); &__c[0];}))
# 380 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     )));
      systemTestState++;
      break;
    case 16:
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 384 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 384 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     "Running test (4/4) - Motors"
# 384 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     ); &__c[0];}))
# 384 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 384 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 384 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     "Running test (4/4) - Motors"
# 384 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     ); &__c[0];}))
# 384 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     )));
      systemTestState++;
      break;
    case 17:
      if (SystemTestModule(5)) systemTestState++;
      break;
    case 18:
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 391 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 391 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     "Test (4/4) - Motors (Complete)"
# 391 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     ); &__c[0];}))
# 391 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 391 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 391 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     "Test (4/4) - Motors (Complete)"
# 391 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     ); &__c[0];}))
# 391 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     )));
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 392 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 392 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     "------------------------------------"
# 392 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     ); &__c[0];}))
# 392 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 392 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 392 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     "------------------------------------"
# 392 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     ); &__c[0];}))
# 392 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     )));
      systemTestState++;
      break;
    case 19:
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 396 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 396 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     "Disabeling all systems"
# 396 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     ); &__c[0];}))
# 396 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 396 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 396 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     "Disabeling all systems"
# 396 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     ); &__c[0];}))
# 396 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     )));
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 397 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 397 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     "------------------------------------"
# 397 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     ); &__c[0];}))
# 397 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 397 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 397 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     "------------------------------------"
# 397 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     ); &__c[0];}))
# 397 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     )));
      testResults = ToLong(SystemStatus);
      SystemDisable();
      AttachSelectButton();
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 401 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 401 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     "------------------------------------"
# 401 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     ); &__c[0];}))
# 401 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 401 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 401 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     "------------------------------------"
# 401 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     ); &__c[0];}))
# 401 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     )));
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 402 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 402 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     "System Test Complete"
# 402 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     ); &__c[0];}))
# 402 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 402 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 402 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     "System Test Complete"
# 402 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     ); &__c[0];}))
# 402 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     )));
      BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 403 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 403 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     "  Results: "
# 403 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     ); &__c[0];}))
# 403 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 403 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 403 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     "  Results: "
# 403 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     ); &__c[0];}))
# 403 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     )));
      BlackBoxAppendln(String(testResults)); Serial.println(String(testResults));
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 405 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 405 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     "------------------------------------"
# 405 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     ); &__c[0];}))
# 405 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 405 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 405 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     "------------------------------------"
# 405 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     ); &__c[0];}))
# 405 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     )));
      testDone = true;
      systemTestState = 0;
      break;
    default:
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 410 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 410 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     "System Test Error: Stopping"
# 410 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     ); &__c[0];}))
# 410 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 410 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 410 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     "System Test Error: Stopping"
# 410 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     ); &__c[0];}))
# 410 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     )));
      systemTestState = 0;
      testDone = true;
      break;
  }

  return testDone;
}

// Runs module test
bool SystemTestModule(byte module) {
  SystemEnable(module);
  bool status = false;
  bool testDone = true;

  if (GetStatus(module)) {
    switch (module) {
      case 0:
        status = BatteryStatus(false);
        break;
      case 1:
        status = digitalRead(42 /* External 5V power */);
        break;
      case 2:
        status = digitalRead(40 /* External 12V power */);
        break;
      case 3:
        status = digitalRead(38 /* External 24V power */);
        break;
      case 4:
        status = digitalRead(36 /* Relay to motor battery power*/);
        break;
      case 5:
        testDone = MotorTest();
        status = MotorStatus();
        break;
      case 6:
        status = MotorStatusLeft();
        break;
      case 7:
        status = MotorStatusRight();
        break;
      case 8:
        status = MotorState();
        break;
      case 9:
        testDone = CanBusTest();
        status = CanBusStatus();
        break;
      case 10:
        testDone = SBusTest();
        status = SBusStatus();
        break;
      case 11:
        status = IridiumTest();
        break;
      case 12:
        testDone = GnssTest();
        status = GnssStatus();
        break;
      case 13:
        testDone = AccelTest();
        status = AccelStatus();
        break;
      case 14:
        status = SDReaderStatus();
        break;
      case 15:
        status = BlackBoxStatus();
        break;
      case 16:
        status = RouteTest();
        break;
      case 21:
        status = HeartBeatStatus();
        break;
      case 17:
        status = DebugCommStatus();
        break;
      case 18:
        LedTest();
        status = true;
        break;
      case 19:
        HeatingStart();
        delay(2000);
        HeatingStop();
        status = true;
        break;
      case 20:
        status = TemperatureStatus();
        testDone = TemperatureTest();
        break;
      case 22:
        status = EmergencyStopStatus();
        break;
      case 23:
        status = true;
        break;
      default:
        BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 510 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
       (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 510 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
       "MODULE TEST: Unknown System Module: "
# 510 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
       ); &__c[0];}))
# 510 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
       ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 510 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
       (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 510 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
       "MODULE TEST: Unknown System Module: "
# 510 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
       ); &__c[0];}))
# 510 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
       )));
        BlackBoxAppendln(((module) == 0 ? "Primary Power" : ((module) == 1 ? "Secondary Power (5V)" : ((module) == 2 ? "Secondary Power (12V)" : ((module) == 3 ? "Secondary Power (24V)" : ((module) == 4 ? "Secondary Power (Motors)" : ((module) == 5 ? "Motors" : ((module) == 6 ? "Motor Left" : ((module) == 7 ? "Motor Right" : ((module) == 8 ? "Motor Active" : ((module) == 9 ? "CAN-bus" : ((module) == 10 ? "Short Range Communication (RF)" : ((module) == 11 ? "Long Range Communication (Iridium)" : ((module) == 12 ? "Global Navigation Satellite System" : ((module) == 13 ? "Accelerometer" : ((module) == 14 ? "Local Storage" : ((module) == 15 ? "Blackbox" : ((module) == 16 ? "Waypoint Route" : ((module) == 17 ? "Debug Comm." : ((module) == 18 ? "System LED" : ((module) == 19 ? "Heating" : ((module) == 20 ? "Temperature" : ((module) == 21 ? "Backup CPU" : ((module) == 22 ? "Emergency Stop" : ((module) == 23 ? "Reserved" : ("Unknown")))))))))))))))))))))))))); Serial.println(((module) == 0 ? "Primary Power" : ((module) == 1 ? "Secondary Power (5V)" : ((module) == 2 ? "Secondary Power (12V)" : ((module) == 3 ? "Secondary Power (24V)" : ((module) == 4 ? "Secondary Power (Motors)" : ((module) == 5 ? "Motors" : ((module) == 6 ? "Motor Left" : ((module) == 7 ? "Motor Right" : ((module) == 8 ? "Motor Active" : ((module) == 9 ? "CAN-bus" : ((module) == 10 ? "Short Range Communication (RF)" : ((module) == 11 ? "Long Range Communication (Iridium)" : ((module) == 12 ? "Global Navigation Satellite System" : ((module) == 13 ? "Accelerometer" : ((module) == 14 ? "Local Storage" : ((module) == 15 ? "Blackbox" : ((module) == 16 ? "Waypoint Route" : ((module) == 17 ? "Debug Comm." : ((module) == 18 ? "System LED" : ((module) == 19 ? "Heating" : ((module) == 20 ? "Temperature" : ((module) == 21 ? "Backup CPU" : ((module) == 22 ? "Emergency Stop" : ((module) == 23 ? "Reserved" : ("Unknown"))))))))))))))))))))))))));
        break;
    }
  }

  SetStatus(module, status);

  return testDone;
}

// Checks all Systems
void SystemCheck() {
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 523 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 523 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
 "------------------------------------"
# 523 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
 ); &__c[0];}))
# 523 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 523 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 523 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
 "------------------------------------"
# 523 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
 ); &__c[0];}))
# 523 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
 )));
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 524 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 524 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
 "Running full system check"
# 524 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
 ); &__c[0];}))
# 524 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 524 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 524 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
 "Running full system check"
# 524 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
 ); &__c[0];}))
# 524 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
 )));
  BatteryStatus(true);
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 526 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 526 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
 "------------------------------------"
# 526 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
 ); &__c[0];}))
# 526 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 526 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 526 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
 "------------------------------------"
# 526 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
 ); &__c[0];}))
# 526 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
 )));
  for (int i = 0; i < 24 - 2; i++) {
    SystemCheckModule(i);
  }

  SetStatus(22, EmergencyStopStatus());
  SetStatus(23, true);
}

// Runs module check
bool SystemCheckModule(byte module) {
  bool status = false;

  switch (module) {
    case 0:
      status = BatteryStatus(false);
      break;
    case 1:
      status = digitalRead(42 /* External 5V power */);
      break;
    case 2:
      status = digitalRead(40 /* External 12V power */);
      break;
    case 3:
      status = digitalRead(38 /* External 24V power */);
      break;
    case 4:
      status = digitalRead(36 /* Relay to motor battery power*/);
      break;
    case 5:
      status = MotorStatus(); // Left && Right motor
      break;
    case 6:
      status = MotorStatusLeft();
      break;
    case 7:
      status = MotorStatusRight();
      break;
    case 8:
      status = MotorState(); // <-- Active or idle?
      break;
    case 9:
      status = CanBusStatus();
      break;
    case 10:
      status = SBusStatus();
      break;
    case 11:
      status = IridiumStatus();
      break;
    case 12:
      status = GnssStatus();
      break;
    case 13:
      status = AccelStatus();
      break;
    case 14:
      status = SDReaderStatus();
      break;
    case 15:
      status = BlackBoxStatus();
      break;
    case 16:
      status = RouteCheck();
      break;
    case 17:
      status = DebugCommStatus();
      break;
    case 18:
      status = LedStatus();
      break;
    case 19:
      status = HeatingStatus();
      break;
    case 20:
      status = TemperatureStatus();
      break;
    case 21:
      status = HeartBeatStatus();
      break;
    default:
      BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 607 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 607 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     "MODULE CHECK: Unknown System Module: "
# 607 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     ); &__c[0];}))
# 607 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 607 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 607 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     "MODULE CHECK: Unknown System Module: "
# 607 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino" 3
     ); &__c[0];}))
# 607 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Control.ino"
     )));
      BlackBoxAppendln(module); Serial.println(module);
      break;
  }

  SetStatus(module, status);
  return status;
}
# 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
/*

  GeoRover Heartbeat functions.

  Send and receive heartbeat to backup MCU.



  Mads Rosenhøj Jeppesen

  Aarhus University

  2021

*/
# 10 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
unsigned long lastMillisResetBackup = 0;
unsigned long lastMillisHeartbeatIn = 0;
unsigned long lastMillisHeartbeatOut = 0;
unsigned long lastMillisHeartbeatBlackbox = 300000 /* Time between system status log*/;
bool receivedHeartBeat = false;

// Runs heartbeat in and out
void HeartBeat() {
  HeartBeatOut();
  HeartBeatIn();
  HeartBeatTimeout();
  HeartbeatBlackBox();
}

// Sends heartbeat with with frequency of HRTBEAT_FRQ
void HeartBeatOut() {
  if (millis() - lastMillisHeartbeatOut < 60000 / 12 /* times per minute*/) {
    return;
  }
  // DEBUG_PRINTLN("Sending Heartbeat!");
  lastMillisHeartbeatOut = millis();
  digitalWrite(7, true);
  delay(5);
  digitalWrite(7, false);
}

bool HeartBeatStatus() {
  HeartBeatTimeout();

  return GetStatus(21);
}

// Checks if time since last heartbeat received > maximum treshold
// Will attempt to reset backup CPU with a frequency of  BACKUP_RST_FRQ
void HeartBeatTimeout() {
  if (millis() - lastMillisHeartbeatIn < 60000 /* Treshold for late heartbeat*/) {
    return;
  }

  // Reset backup CPU
  if (GetStatus(21)) {
    lastMillisResetBackup = millis();
    SetStatus(21, false);
    BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 53 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 53 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
   "------------------------------------"
# 53 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
   ); &__c[0];}))
# 53 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
   ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 53 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 53 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
   "------------------------------------"
# 53 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
   ); &__c[0];}))
# 53 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
   )));
    BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 54 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 54 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
   "Error: Backup CPU Heartbeat Timeout, attempting to Reset Backup System."
# 54 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
   ); &__c[0];}))
# 54 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
   ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 54 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 54 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
   "Error: Backup CPU Heartbeat Timeout, attempting to Reset Backup System."
# 54 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
   ); &__c[0];}))
# 54 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
   )));
    ResetBackupCPU();
    BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 56 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 56 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
   "------------------------------------"
# 56 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
   ); &__c[0];}))
# 56 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
   ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 56 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 56 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
   "------------------------------------"
# 56 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
   ); &__c[0];}))
# 56 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
   )));
  } else {
    if (millis() - lastMillisResetBackup > 60000 /* Treshold for late heartbeat*/ /* Time between each reset attempt*/) {
      lastMillisResetBackup = millis();
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 60 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 60 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
     "------------------------------------"
# 60 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
     ); &__c[0];}))
# 60 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 60 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 60 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
     "------------------------------------"
# 60 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
     ); &__c[0];}))
# 60 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
     )));
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 61 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 61 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
     "Error: Backup CPU offline, attempting to Reset."
# 61 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
     ); &__c[0];}))
# 61 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 61 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 61 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
     "Error: Backup CPU offline, attempting to Reset."
# 61 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
     ); &__c[0];}))
# 61 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
     )));
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 62 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 62 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
     "------------------------------------"
# 62 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
     ); &__c[0];}))
# 62 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 62 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 62 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
     "------------------------------------"
# 62 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
     ); &__c[0];}))
# 62 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
     )));
      ResetBackupCPU();
    }
  }
}

// Resets Heartbeatflag if set and resets heartbeat in timer
void HeartBeatIn() {
  if (!receivedHeartBeat) {
    return;
  }
  receivedHeartBeat = false;

  if (millis() - lastMillisHeartbeatIn < 500) {
    return;
  }

  lastMillisHeartbeatIn = millis();
  // DEBUG_PRINTLN("Heartbeat Received!");
  if (!GetStatus(21)) {
    SetStatus(21, true);
    BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 83 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 83 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
   "------------------------------------"
# 83 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
   ); &__c[0];}))
# 83 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
   ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 83 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 83 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
   "------------------------------------"
# 83 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
   ); &__c[0];}))
# 83 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
   )));
    BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 84 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 84 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
   "Backup CPU online"
# 84 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
   ); &__c[0];}))
# 84 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
   ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 84 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 84 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
   "Backup CPU online"
# 84 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
   ); &__c[0];}))
# 84 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
   )));
    BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 85 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 85 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
   "------------------------------------"
# 85 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
   ); &__c[0];}))
# 85 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
   ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 85 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 85 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
   "------------------------------------"
# 85 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
   ); &__c[0];}))
# 85 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
   )));
  }
}

// Set Heartbeat flag
void HeartBeatInInterrupt() {
  if (receivedHeartBeat) {
    return;
  }
  receivedHeartBeat = true;
}

// Attempt to reset backup CPU
void ResetBackupCPU() {
  digitalWrite(6 /* Backup CPU reset pin*/, false);
  delay(5);
  digitalWrite(6 /* Backup CPU reset pin*/, true);
}

void HeartbeatBlackBox() {
  if (millis() - lastMillisHeartbeatBlackbox > 300000 /* Time between system status log*/) {
    lastMillisHeartbeatBlackbox = millis();
    BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 107 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 107 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
   "------------------------------------"
# 107 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
   ); &__c[0];}))
# 107 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
   ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 107 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 107 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
   "------------------------------------"
# 107 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
   ); &__c[0];}))
# 107 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
   )));
    BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 108 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 108 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
   "Heartbeat System Check: "
# 108 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
   ); &__c[0];}))
# 108 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
   ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 108 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 108 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
   "Heartbeat System Check: "
# 108 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
   ); &__c[0];}))
# 108 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
   )));
    SystemCheck();
    BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 110 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 110 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
   "System Status: "
# 110 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
   ); &__c[0];}))
# 110 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
   ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 110 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 110 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
   "System Status: "
# 110 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
   ); &__c[0];}))
# 110 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
   )));
    BlackBoxAppendln(String(ToLong(SystemStatus))); Serial.println(String(ToLong(SystemStatus)));
    BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 112 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 112 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
   "------------------------------------"
# 112 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
   ); &__c[0];}))
# 112 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
   ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 112 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 112 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
   "------------------------------------"
# 112 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino" 3
   ); &__c[0];}))
# 112 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Heartbeat.ino"
   )));
  }
}
# 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
/*

  GeoRover Motor control



  Mads Rosenhøj Jeppesen

  Aarhus University

  2022

*/
# 8 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
unsigned long millisLastMotorStep;

// Initialize motor power
bool InitializeMotors() {
  // if (!GetStatus(MODULE_PWR_MOTOR)) {
  //   return false;
  // }

  digitalWrite(34 /* Enables CAN power, Motor HW Enable and Motor Driver Power*/, true);

  if (!MotorCycle()) return false;

  // return MotorStatus();
  return true;
}

// Disable Motor HW Enable
void TerminateMotors() {
  digitalWrite(34 /* Enables CAN power, Motor HW Enable and Motor Driver Power*/, false);
}

// Moves motors based on direction and speed input within the range of [-1 and 1]
// -1 full left, 1 full right (dir)
// -1 full reverse, 1 full forward (speed)
void MotorUpdate(float dir, float speed) {
  float velocityLeft;
  float velocityRight;

  SpeedCalculation(dir, speed, velocityLeft, velocityRight);

  // Update CAN messages
  motorLeft.Update(velocityLeft);
  motorRight.Update(velocityRight);
}

// Moves motors based on direction and speed input within the range of [-1 and 1]
// -1 full left, 1 full right (dir)
// -1 full reverse, 1 full forward (speed)
void MotorUpdateTorque(float dir, float speed) {
  float torqueLeft;
  float torqueRight;

  TorqueCalculation(dir, speed, torqueLeft, torqueRight);

  // Update CAN messages
  motorLeft.UpdateTorque(torqueLeft);
  motorRight.UpdateTorque(torqueRight);
}

void TorqueCalculation(float dir, float speed, float &torqueLeft, float &torqueRight) {
  float steerFactorLeft = 1.0f;
  float steerFactorRight = 1.0f;

  // turn right, right wheel moves slower
  if (dir > 0) {
    steerFactorRight = SteerFactor(dir);
  }
  // turn left, left wheel moves slower
  else if (dir < 0) {
    steerFactorLeft = SteerFactor(dir);
  }

  torqueLeft = speed * steerFactorLeft;
  torqueRight = speed * steerFactorRight;

  if (((torqueLeft)>0?(torqueLeft):-(torqueLeft)) < 0.1f /* %*/) torqueLeft = 0;
  if (((torqueRight)>0?(torqueRight):-(torqueRight)) < 0.1f /* %*/) torqueRight = 0;
}

// Calculates skid steering based on direction and speed
void SpeedCalculation(float dir, float speed, float &velocityLeft, float &velocityRight) {
  float steerFactorLeft = 1.0f;
  float steerFactorRight = 1.0f;

  // turn right, right wheel moves slower
  if (dir > 0) {
    steerFactorRight = SteerFactor(dir);
  }
  // turn left, left wheel moves slower
  else if (dir < 0) {
    steerFactorLeft = SteerFactor(dir);
  }

  float speedLeft = speed * steerFactorLeft;
  float speedRight = speed * steerFactorRight;

  if (speedLeft < 0) {
    velocityLeft = 5.0f /* kmh*/ * speedLeft;
  } else {
    velocityLeft = 10.0f /* kmh*/ * speedLeft;
  }

  if (speedRight < 0) {
    velocityRight = 5.0f /* kmh*/ * speedRight;
  } else {
    velocityRight = 10.0f /* kmh*/ * speedRight;
  }

  if (((velocityLeft)>0?(velocityLeft):-(velocityLeft)) < 1.0f /* kmh*/) velocityLeft = 0;
  if (((velocityRight)>0?(velocityRight):-(velocityRight)) < 1.0f /* kmh*/) velocityRight = 0;
}

// Run CAN cycle until both motors report NO ERROR or times out
bool MotorCycle() {
  if (!GetStatus(9)) {
    return false;
  }
  if (!GetStatus(4)) {
    return false;
  }

  MotorUpdate(0, 0);
  unsigned long millisMotorStart = millis();

  // (!motorLeft.CanStatus() || !motorRight.CanStatus()) && 1

  // Run canbus for a while to get motor states and test data connection
  while ((!motorLeft.CanStatus() || !motorRight.CanStatus()) && (millis() - millisMotorStart) < 5000) {
    if (!CanBusProcess()) {
      return false;
    }
  }

  return MotorCanStatus();
}

// Returns true if both motors are operational
bool MotorState() {
  return motorLeft.GetState() || motorRight.GetState();
}

// Motors operational?
bool MotorStatus() {
  // bool status = (MotorStatusLeft() && MotorStatusRight());
  bool status = GetStatus(5);

  return status;
}

// Returns true if both motors have a valid can signal
bool MotorCanStatus(){
  return motorLeft.CanStatus() && motorRight.CanStatus();
}
// Left motor status
bool MotorStatusLeft() {
  return motorLeft.Status();
}

// Right motor status
bool MotorStatusRight() {
  return motorRight.Status();
}

// Calculates steering factor from 2nd order function (-2x^2 + 1), used in skid steering
float SteerFactor(float dir) {
  float scale;

  if (dir >= 1.0f)
    scale = -1.0f;

  else if (dir <= -1.0f)
    scale = -1.0f;

  else
    scale = -2.0f * (dir * dir) + 1.0f;

  return scale;
}

// Full motor test
bool MotorTest() {
  if (!GetStatus(5)) {
    return true;
  }
  bool testDone = false;
  static int motorTestState;

  switch (motorTestState) {
    case 0:
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 187 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 187 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     "Motor Test 1 - Linear Ramp"
# 187 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     ); &__c[0];}))
# 187 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 187 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 187 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     "Motor Test 1 - Linear Ramp"
# 187 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     ); &__c[0];}))
# 187 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     )));
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 188 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 188 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     "------------------------------------"
# 188 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     ); &__c[0];}))
# 188 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 188 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 188 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     "------------------------------------"
# 188 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     ); &__c[0];}))
# 188 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     )));
      motorTestState++;
      break;
    case 1:
      if (MotorTestRamp()) motorTestState++;
      break;
    case 2:
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 195 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 195 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     "Motor Test 2 - Steering"
# 195 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     ); &__c[0];}))
# 195 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 195 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 195 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     "Motor Test 2 - Steering"
# 195 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     ); &__c[0];}))
# 195 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     )));
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 196 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 196 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     "------------------------------------"
# 196 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     ); &__c[0];}))
# 196 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 196 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 196 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     "------------------------------------"
# 196 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     ); &__c[0];}))
# 196 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     )));
      motorTestState++;
      break;
    case 3:
      if (MotorTestSteer()) motorTestState++;
      break;
    case 4:
      motorTestState = 0;
      testDone = true;
      MotorUpdate(0, 0);
    default:
      break;
  }

  CanBusProcess();

  return testDone;
}

// Runs Motor Ramp Test, ramps up to maximum speed FWD -> Ramps down to 0 -> Ramps up to maximum speed BWD -> Ramps down to 0
bool MotorTestRamp() {
  if (!GetStatus(5)) {
    return true;
  }

  static int motorTestRampState;
  static float speed;

  bool testDone = false;
  switch (motorTestRampState) {
    case 0:
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 227 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 227 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     "Ramping up - FWD"
# 227 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     ); &__c[0];}))
# 227 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 227 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 227 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     "Ramping up - FWD"
# 227 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     ); &__c[0];}))
# 227 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     )));
      motorTestRampState++;
      break;
    case 1:
      if (millis() - millisLastMotorStep > 20) {
        MotorUpdate(0, speed);
        speed += 0.01;
        millisLastMotorStep = millis();
      }
      if (speed >= 1) motorTestRampState++;
      break;
    case 2:
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 239 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 239 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     "------------------------------------"
# 239 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     ); &__c[0];}))
# 239 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 239 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 239 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     "------------------------------------"
# 239 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     ); &__c[0];}))
# 239 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     )));
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 240 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 240 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     "Ramping down"
# 240 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     ); &__c[0];}))
# 240 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 240 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 240 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     "Ramping down"
# 240 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     ); &__c[0];}))
# 240 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     )));
      motorTestRampState++;
      break;
    case 3:
      if (millis() - millisLastMotorStep > 20) {
        MotorUpdate(0, speed);
        speed -= 0.01;
        millisLastMotorStep = millis();
      }
      if (speed <= 0) {
        speed = 0;
        MotorUpdate(0, speed);
        motorTestRampState++;
      }
      break;
    case 4:
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 256 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 256 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     "------------------------------------"
# 256 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     ); &__c[0];}))
# 256 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 256 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 256 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     "------------------------------------"
# 256 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     ); &__c[0];}))
# 256 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     )));
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 257 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 257 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     "Ramping up - BWD"
# 257 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     ); &__c[0];}))
# 257 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 257 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 257 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     "Ramping up - BWD"
# 257 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     ); &__c[0];}))
# 257 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     )));
      motorTestRampState++;
      break;
    case 5:
      if (millis() - millisLastMotorStep > 20) {
        MotorUpdate(0, speed);
        speed -= 0.01;
        millisLastMotorStep = millis();
      }
      if (speed <= -1) motorTestRampState++;
      break;
    case 6:
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 269 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 269 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     "------------------------------------"
# 269 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     ); &__c[0];}))
# 269 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 269 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 269 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     "------------------------------------"
# 269 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     ); &__c[0];}))
# 269 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     )));
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 270 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 270 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     "Ramping down"
# 270 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     ); &__c[0];}))
# 270 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 270 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 270 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     "Ramping down"
# 270 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     ); &__c[0];}))
# 270 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     )));
      motorTestRampState++;
      break;
    case 7:
      if (millis() - millisLastMotorStep > 20) {
        MotorUpdate(0, speed);
        speed += 0.01;
        millisLastMotorStep = millis();
      }
      if (speed >= 0) {
        speed = 0;
        motorTestRampState++;
        MotorUpdate(0, speed);
      }
      break;
    case 8:
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 286 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 286 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     "------------------------------------"
# 286 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     ); &__c[0];}))
# 286 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 286 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 286 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     "------------------------------------"
# 286 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     ); &__c[0];}))
# 286 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     )));
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 287 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 287 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     "Motor Test 2 - Steering"
# 287 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     ); &__c[0];}))
# 287 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 287 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 287 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     "Motor Test 2 - Steering"
# 287 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     ); &__c[0];}))
# 287 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     )));
      motorTestRampState++;
      break;
    case 9:
      motorTestRampState = 0;
      testDone = true;
      MotorUpdate(0, 0);
    default:
      testDone = true;
      break;
  }

  return testDone;
}

// Runs Motor Steering Test, Runs at lower speed, turns max steer right -> Max Steer Left -> Centered
bool MotorTestSteer() {
  if (!GetStatus(5)) {
    return true;
  }
  static int motorTestSteerState;
  static float dir;
  float speed = 10.0f /* kmh*/ * 0.25;
  bool testDone = false;

  switch (motorTestSteerState) {
    case 0:
      BlackBoxAppendln("Turning Right -->"); Serial.println("Turning Right -->");
      motorTestSteerState++;
      break;
    case 1:
      if (millis() - millisLastMotorStep > 20) {
        MotorUpdate(dir, speed);
        dir += 0.02;
        millisLastMotorStep = millis();
      }
      if (dir >= 1) motorTestSteerState++;
      break;
    case 2:
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 326 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 326 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     "------------------------------------"
# 326 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     ); &__c[0];}))
# 326 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 326 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 326 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     "------------------------------------"
# 326 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     ); &__c[0];}))
# 326 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     )));
      BlackBoxAppendln("Turning Left <--"); Serial.println("Turning Left <--");
      motorTestSteerState++;
      break;
    case 3:
      if (millis() - millisLastMotorStep > 20) {
        MotorUpdate(dir, speed);
        dir -= 0.02;
        millisLastMotorStep = millis();
      }
      if (dir <= -1) motorTestSteerState++;
      break;
    case 4:
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 339 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 339 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     "------------------------------------"
# 339 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     ); &__c[0];}))
# 339 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 339 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 339 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     "------------------------------------"
# 339 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino" 3
     ); &__c[0];}))
# 339 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Motors.ino"
     )));
      BlackBoxAppendln("Centering --> || <--"); Serial.println("Centering --> || <--");
      motorTestSteerState++;
      break;
    case 5:
      if (millis() - millisLastMotorStep > 20) {
        MotorUpdate(dir, speed);
        dir += 0.02;
        millisLastMotorStep = millis();
      }
      if (dir >= 0) motorTestSteerState++;
      break;
    case 6:
      motorTestSteerState = 0;
      testDone = true;
      MotorUpdate(0, 0);
    default:
      testDone = true;
      break;
  }

  return testDone;
}
# 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Navigation.ino"
/*

  GeoRover Navigation Stack



  Mads Rosenhøj Jeppesen

  Aarhus University

  2021

*/
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Navigation.ino"
/* Algorithm:

  1. Check coordinates valid

    1.1 Format correct?

    1.2 Distance < min distance?

        -> Valid gps fix? (time since last)

    else break

  2. Calculate current bearing

    2.1 Distance between previous waypoints > Min Distance?

        -> Calculate using Forward Azimuth

    2.2 Else skip (Set bearing: 0)

  3. Calculate target bearing (Difference between current position and target waypoint)

    3.1 Is bearing valid?

        -> Calculate using forward Azimuth

    3.2 Else skip (Set target bearing: 0)

  4. Calculate change bearing

    4.1 difference between current and target



*/
# 28 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Navigation.ino"
# 29 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Navigation.ino" 2
# 30 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Navigation.ino" 2


# 31 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Navigation.ino"
bool navigationPreCheck = false;
long millisLastNavCheck;
float maxSpeedScale;

bool Navigate() {
  if (millis() - millisLastNavCheck < 1000) {
    return false;
  }

  millisLastNavCheck = millis();

  if (!NavigationRunCheck()) {
    MotorUpdate(0, 0);
    return false;
  }

  PathingProcess();

  NavigationMotorUpdate();
}

// Autonomous Motor Controller, updates motor targets based on 
void NavigationMotorUpdate(){
  AutonomousTopSpeedUpdate();

  MotorUpdate(BearingDirection(), 1.0f * maxSpeedScale);
}

// Increases top speed based on time since autonomy start to avoid high speed start up
void AutonomousTopSpeedUpdate(){
  float timeScaledSpeed = millisAutonomyStart * 0.9f / 30000.0f /* linear scale, max speed after 5 minutes*/;
  maxSpeedScale = ((1.0f)<(timeScaledSpeed)?(1.0f):(timeScaledSpeed));
}

void NavigationPreCheckReset() {
  navigationPreCheck = false;
  ResetRouteStatus();

  millisLastNavCheck = millis() - 1000;
}

// Resets current waypoint index and restarts route
void ResetNavigation() {
  waypointIndex = 0;
  UpdateWaypoint();
}

//  Checks to run before starting autonomous navigation
bool NavigationPreCheck() {
  if (navigationPreCheck) return true;

  if (millis() - millisLastNavCheck < 1000) {
    return navigationPreCheck;
  }

  LedSetSignal(LED_SIGNAL_IDLE);

  millisLastNavCheck = millis();
  navigationPreCheck = true;

  bool routeValid = RouteTest();
  bool gnssValid = GnssSignal();
  bool motorValid = MotorCycle();

  PreCheckStatus(routeValid);
  PreCheckStatus(gnssValid);
  PreCheckStatus(motorValid);

  LedSetSignal();
  return navigationPreCheck;
}

// Sets navigationPreCheck flag to false and signals
void PreCheckStatus(bool status) {
  if (!status) {
    navigationPreCheck = false;
    StatusHaltLed(LED_SIGNAL_ERROR_SHORT_HALT);
  } else {
    StatusHaltLed(LED_SIGNAL_OK_SHORT_HALT);
  }
}

// Check system status while navigation is running.
// GNSS status, Accelerometer output (tilt?), Battery Status
bool NavigationRunCheck() {
  return SystemCheckMode(5 /* Autonomous driving mode*/);
}
# 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
/*

  GeoRover pathing protocols



  Mads Rosenhøj Jeppesen

  Aarhus University

  2021

*/
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
long latRoute, lonRoute; // Latest position buffer (degrees *10^-7)
int idxRoute; //
int lengthRoute;
int indexRoute;

bool routeStatus = false;
bool routeTestRun = false;

char operatorName[numChars];

// Target coordinate (read from EEPROM)
long latTarget = 0;
long lonTarget = 0;
float navigationDir = 0;

// Current coordinate (From GNSS)
long headingError = 0; // deg * 10^-5

// returns true if waypoint file exists
bool SDRoute() {
  return SD.exists("Waypoints.csv");
}

// Clears current route data
void ClearRoute() {
  latRoute = 0;
  lonRoute = 0;

  memset(operatorName, '\0', sizeof(operatorName));
  lengthRoute = 0;
  indexRoute = 0;
}

// Loads Waypoint file data and first waypoint into buffer
bool LoadRoute() {
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 44 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 44 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 "Loading Waypoint file"
# 44 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 44 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 44 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 44 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 "Loading Waypoint file"
# 44 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 44 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 )));

  ClearRoute();

  // Load first waypoint into buffer
  if (!RouteSD(latRoute, lonRoute, 0)) {
    return false;
  }

  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 53 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 53 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 "Done Loading"
# 53 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 53 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 53 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 53 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 "Done Loading"
# 53 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 53 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 )));
  return true;
}

// Test full route, only run once (used in Navigation pre-check). To perform new test, run ResetRouteStatus()
bool RouteTest() {
  if (routeTestRun) routeStatus;

  routeTestRun = true; // Avoid several route tests being run

  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 63 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 63 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 "Running full route test"
# 63 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 63 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 63 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 63 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 "Running full route test"
# 63 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 63 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 )));

  routeStatus = false;
  if (!LoadRoute()) {
    return routeStatus;
  }
  if (!RouteSDValid()) {
    return routeStatus;
  }
  if (!CompareEepromSdRoute()) {
    if (!FlashRouteEeprom()) {
      return routeStatus;
    }
    if (!CompareEepromSdRoute()) {
      return routeStatus;
    }
  }

  routeStatus = true;

  return routeStatus;
}

// Compares current EEPROM route data to SD data
// TODO: Check full route, currently only checks route index, route length and 1st waypoint
bool CompareEepromSdRoute() {
  bool validity = true;

  int routeIndexEeprom = (int)(((EEPROM.read(1 + 8) << 0) & 0xFF) + ((EEPROM.read(1 + 8 + 1) << 8) & 0xFF00));
  int routeLengthEeprom = (int)(((EEPROM.read(1 + 8 + 2) << 0) & 0xFF) + ((EEPROM.read(1 + 8 + 2 + 1) << 8) & 0xFF00));

  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 94 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 94 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 "------------------------------------"
# 94 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 94 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 94 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 94 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 "------------------------------------"
# 94 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 94 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 )));
  BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 95 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 95 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 "EEPROM route Index: "
# 95 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 95 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 95 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 95 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 "EEPROM route Index: "
# 95 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 95 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 )));
  BlackBoxAppend(routeIndexEeprom); Serial.print(routeIndexEeprom);
  BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 97 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 97 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 "\t EEPROM route Length: "
# 97 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 97 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 97 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 97 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 "\t EEPROM route Length: "
# 97 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 97 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 )));

  BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 99 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 99 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 "Waypoint.csv route Index: "
# 99 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 99 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 99 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 99 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 "Waypoint.csv route Index: "
# 99 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 99 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 )));
  BlackBoxAppendln(indexRoute); Serial.println(indexRoute);
  BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 101 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 101 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 "\t Waypoint.csv route Length: "
# 101 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 101 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 101 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 101 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 "\t Waypoint.csv route Length: "
# 101 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 101 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 )));
  BlackBoxAppendln(lengthRoute); Serial.println(lengthRoute);
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 103 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 103 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 "------------------------------------"
# 103 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 103 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 103 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 103 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 "------------------------------------"
# 103 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 103 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 )));

  if (indexRoute != routeIndexEeprom) return false;
  if (lengthRoute != routeLengthEeprom) return false;

  long latEE, lonEE;

  if (!RouteEe(latEE, lonEE, 0)) return false;
  RouteEe(latEE, lonEE, 0);

  BlackBoxAppend(routeLengthEeprom); Serial.print(routeLengthEeprom);
  BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 114 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 114 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 "\t - EEPROM 1st waypoint: Lat: "
# 114 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 114 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 114 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 114 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 "\t - EEPROM 1st waypoint: Lat: "
# 114 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 114 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 )));
  BlackBoxAppend(latEE); Serial.print(latEE);
  BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 116 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 116 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 "\t lon: "
# 116 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 116 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 116 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 116 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 "\t lon: "
# 116 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 116 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 )));
  BlackBoxAppendln(lonEE); Serial.println(lonEE);

  BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 119 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 119 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 "\t - Waypoint.csv 1st waypoint: Lat: "
# 119 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 119 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 119 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 119 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 "\t - Waypoint.csv 1st waypoint: Lat: "
# 119 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 119 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 )));
  BlackBoxAppend(latRoute); Serial.print(latRoute);
  BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 121 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 121 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 "\t lon: "
# 121 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 121 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 121 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 121 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 "\t lon: "
# 121 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 121 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 )));
  BlackBoxAppendln(lonRoute); Serial.println(lonRoute);

  if ((latRoute != latEE) && (lonRoute != lonEE)) return false;

  return validity;
}

bool RouteSDValid(){
  if (!SDRoute) {
    return false;
  }

  // DEBUG_PRINTLN(F("Validating Waypoint file"));

  File waypoints = SD.open("Waypoints.csv", O_READ);

  if (!waypoints) {
    // DEBUG_PRINTLN(F("Error opening waypoints file"));
    return false;
  }

  long latRouteOld, lonRouteOld;

  // Read waypoint file
  for (int i = 0; i < lengthRoute + 3; i++) {
    ParseWaypoint(waypoints.readStringUntil('\n'), i);
    // Skip first 3 (header, data, header)
    if (i > 2) {
      if (!CoordinateValidityFormat(latRoute, lonRoute)) {
        waypoints.close();
        return false;
      }
    }
    // Range checks from 2nd waypoint
    if (i > 3) {
      if (!CoordinateValidityRange(latRouteOld, lonRouteOld, latRoute, lonRoute)) {
        waypoints.close();
        return false;
      }
    }
    latRouteOld = latRoute;
    lonRouteOld = lonRoute;
  }

  return true;
}
// Check if route is valid (SD and EEPROM)
bool RouteCheck() {
  return RouteSDValid() && CompareEepromSdRoute();
}

bool PathingProcess() {
  GnssUpdate();

  // Calculate distance from current pos to target pos
  WaypointUpdate();

  // Calculate bearing to target
  BearingUpdate();
}

// Updates waypoint based on distance to target
void WaypointUpdate() {
  if (WaypointWithinRange) {
    IncrementWaypoint();
  }
}

// Updates bearing offset based on target and current bearing
void BearingUpdate() {
  long heading = GnssGetHeading(); // deg * 10^-5
  long targetHeading = (long)(CourseToLong(latCurrent, lonCurrent, latTarget, lonTarget)) * 100000;

  headingError = targetHeading - heading;

  // Check headingError > 180 degree to take smallest rotation (190 degrees to avoid rotation overlap)
  if(headingError > 190){
    headingError -= 360;
  } else if (headingError < -190){
    headingError += 360;
  }

  if (((headingError)>0?(headingError):-(headingError)) < 250000 /* (deg * 10^-5) (2.5 degres)*/) {
    navigationDir = 0;
  } else if (headingError > 250000 /* (deg * 10^-5) (2.5 degres)*/) {
    navigationDir = 0.5f;
  } else {
    navigationDir = -0.5f;
  }

}

// Returns latest navigation direction
float BearingDirection() {
  return navigationDir;
}

// Checks if distance from current position to target is < min
bool WaypointWithinRange() {
  long distance = DistanceBetweenLong(latCurrent, lonCurrent, latTarget, lonTarget);
  return distance < 10 /* max distance to accept waypoint (meters)*/;
}

// Increments current waypoint returns to 0 if > total number of points
void IncrementWaypoint() {
  if (waypointIndex == lengthRoute) {
    waypointIndex = 0;
  } else {
    waypointIndex++;
  }

  UpdateWaypoint();
}

// Reads EEPROM waypoint lat, lon from current waypointIndex
void UpdateWaypoint() {
  EEPROMReadLatLon(waypointIndex);
  EEPROM.put(1 + 8 + 2 + 2, waypointIndex);
}

// Reads latitude and longitude at given index
bool RouteSD(long &lat, long &lon, int index) {
  if (!SDRoute) {
    return false;
  }

  File waypoints = SD.open("Waypoints.csv", O_READ);

  if (!waypoints) {
    BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 251 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 251 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
   "Error opening waypoints file"
# 251 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
   ); &__c[0];}))
# 251 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
   ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 251 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 251 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
   "Error opening waypoints file"
# 251 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
   ); &__c[0];}))
# 251 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
   )));
    return false;
  }

  // Read waypoint file until index
  int idx;
  while (waypoints.available() && idx < index + 3) {
    waypoints.readStringUntil('\n');
    idx++;
  }

  if (idx != index + 3) {
    BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 263 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 263 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
   "Waypoint file doesn't contain index"
# 263 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
   ); &__c[0];}))
# 263 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
   ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 263 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 263 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
   "Waypoint file doesn't contain index"
# 263 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
   ); &__c[0];}))
# 263 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
   )));
    return false;
  }

  if (!PopulateLatLon(waypoints.readStringUntil('\n'), lat, lon)) {
    waypoints.close();
    return false;
  }

  waypoints.close();
  return true;
}

// Reads EEPROM Latitude and Longitude values at given index, returns false if non found
bool RouteEe(long &lat, long &lon, int index) {
  EEPROM.get(1 + 8 + 2 + 2 + 2 + index * 8, lat);
  EEPROM.get(1 + 8 + 2 + 2 + 2 + 4 + index * 8, lon);

  if (lat > 900000000 /* deg * 10^-7*/ || lat < -900000000 /* deg * 10^-7*/ || lat == 0) {
    // DEBUG_PRINTLN(F("EEPROM lat value outside allowed values or 0"));
    return false;
  }
  if (lon > 1800000000 /* deg * 10^-7*/ || lon < -1800000000 /* deg * 10^-7*/ || lon == 0) {
    // DEBUG_PRINTLN(F("EEPROM lon value outside allowed values or 0"));
    return false;
  }

  return true;
}

// Flash waypoints to EEPROM
bool FlashRouteEeprom() {
  if (!SDRoute) {
    return false;
  }

  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 299 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 299 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 "Flashing Waypoint file to EEPROM"
# 299 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 299 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 299 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 299 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 "Flashing Waypoint file to EEPROM"
# 299 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 299 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 )));

  File waypoints = SD.open("Waypoints.csv", O_READ);

  if (!waypoints) {
    BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 304 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 304 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
   "Error opening waypoints file"
# 304 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
   ); &__c[0];}))
# 304 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
   ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 304 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 304 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
   "Error opening waypoints file"
# 304 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
   ); &__c[0];}))
# 304 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
   )));
    return false;
  }

  // Read waypoint file
  for (int i = 0; i < lengthRoute + 3; i++) {
    ParseWaypoint(waypoints.readStringUntil('\n'), i);
    // Skip first 3 (header, data, header)
    if (i > 2) {
      EEPROMWriteLatLon(i - 3);
    }
  }

  return true;
}

/* Parses waypoint string separating into latitude and longitude values

    waypointIdx,Latitude,Longitude

*/
# 323 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
bool PopulateLatLon(String waypoint, long &lat, long &lon) {
  char waypointBuffer[waypoint.length() + 1];
  waypoint.toCharArray(waypointBuffer, waypoint.length() + 1);

  char *strtokIndx; // strtok() index

  // Route Index
  strtokIndx = strtok(waypointBuffer, ",");
  idxRoute = atoi(strtokIndx);

  // Latitude
  strtokIndx = strtok(
# 334 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3 4
                     __null
# 334 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
                         , ",");
  latRoute = atol(strtokIndx);
  lat = latRoute;

  // Longitude
  strtokIndx = strtok(
# 339 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3 4
                     __null
# 339 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
                         , ",");
  lonRoute = atol(strtokIndx);
  lon = lonRoute;

  BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 343 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 343 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 "Latitude: "
# 343 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 343 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 343 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 343 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 "Latitude: "
# 343 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 343 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 )));
  BlackBoxAppend(lat); Serial.print(lat);
  BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 345 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 345 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 "\t Longitude: "
# 345 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 345 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 345 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 345 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 "\t Longitude: "
# 345 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 345 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 )));
  BlackBoxAppendln(lon); Serial.println(lon);

  if (!CoordinateValidityFormat(lat, lon)) {
    return false;
  }

  return true;
}

bool PopulateLatLon(String waypoint) {
  PopulateLatLon(waypoint, latRoute, lonRoute);
}

void PopulateWaypointData(String waypoint) {
  char waypointBuffer[waypoint.length() + 1];
  waypoint.toCharArray(waypointBuffer, waypoint.length() + 1);

  char *strtokIndx; // strtok() index

  // Route Index
  strtokIndx = strtok(waypointBuffer, ",");
  indexRoute = atoi(strtokIndx);

  // Waypoint Length
  strtokIndx = strtok(
# 370 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3 4
                      __null
# 370 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
                          , ",");
  lengthRoute = atoi(strtokIndx);

  // Operator
  strtokIndx = strtok(
# 374 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3 4
                     __null
# 374 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
                         , ",");
  strcpy(operatorName, strtokIndx);

  BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 377 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 377 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 "Route Index: "
# 377 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 377 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 377 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 377 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 "Route Index: "
# 377 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 377 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 )));
  BlackBoxAppend(indexRoute); Serial.print(indexRoute);
  BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 379 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 379 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 "\t Route Length: "
# 379 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 379 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 379 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 379 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 "\t Route Length: "
# 379 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 379 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 )));
  BlackBoxAppend(lengthRoute); Serial.print(lengthRoute);
  BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 381 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 381 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 "\t Operator: "
# 381 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 381 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 381 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 381 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 "\t Operator: "
# 381 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 381 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 )));
  BlackBoxAppendln(operatorName); Serial.println(operatorName);
}

// Parse waypoint string
void ParseWaypoint(String waypoint, int index) {
  switch (index) {
    case 0:
      // Header (Waypoint Index,Waypoint Length,Operator)
      break;
    case 1:
      PopulateWaypointData(waypoint);
      break;
    case 2:
      // Header (Index,Lat,Lon)
      break;
    default:
      PopulateLatLon(waypoint);
      break;
  }
}

// Reset route status flag
void ResetRouteStatus() {
  routeStatus = false;
  routeTestRun = false;
}

// Populate buffer latitude longitude into EEPROM
void EEPROMWriteLatLon(int index) {
  EEPROM.put(1 + 8 + 2 + 2 + 2 + index * 8, latRoute);
  EEPROM.put(1 + 8 + 2 + 2 + 2 + 4 + index * 8, lonRoute);
}

// Populate buffer latitude longitude from EEPROM
void EEPROMReadLatLon(int index) {
  EEPROM.get(1 + 8 + 2 + 2 + 2 + index * 8, latRoute);
  EEPROM.get(1 + 8 + 2 + 2 + 2 + 4 + index * 8, lonRoute);
}

// Checks validity of coordinate format (-90 <= lat <= 90 && -180 <= lon <= 180)
bool CoordinateValidityFormat(long lat, long lon) {
  BlackBoxAppend("Checking format of coordinates... lat: " + String(lat, 10) + ", lon: " + String(lon, 10) + " ... "); Serial.print("Checking format of coordinates... lat: " + String(lat, 10) + ", lon: " + String(lon, 10) + " ... ");
  bool validity = true;
  if (lat == 0 || lon == 0) {
    validity = false;
    BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 427 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 427 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
   "coordinates must be non zero ... "
# 427 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
   ); &__c[0];}))
# 427 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
   ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 427 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 427 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
   "coordinates must be non zero ... "
# 427 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
   ); &__c[0];}))
# 427 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
   )));
  }

  if (lat < -900000000 /* deg * 10^-7*/ || lat > 900000000 /* deg * 10^-7*/) {
    validity = false;
    BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 432 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 432 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
   "latitude range must be (-90 < lat < 90) ... "
# 432 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
   ); &__c[0];}))
# 432 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
   ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 432 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 432 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
   "latitude range must be (-90 < lat < 90) ... "
# 432 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
   ); &__c[0];}))
# 432 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
   )));
  }

  if (lon < -1800000000 /* deg * 10^-7*/ || lon > 1800000000 /* deg * 10^-7*/) {
    validity = false;
    BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 437 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 437 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
   "longitude range must be (-180 < lon < 180) ... "
# 437 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
   ); &__c[0];}))
# 437 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
   ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 437 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 437 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
   "longitude range must be (-180 < lon < 180) ... "
# 437 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
   ); &__c[0];}))
# 437 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
   )));
  }

  if (validity) {
    BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 441 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 441 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
   "Coordinates format valid!"
# 441 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
   ); &__c[0];}))
# 441 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
   ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 441 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 441 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
   "Coordinates format valid!"
# 441 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
   ); &__c[0];}))
# 441 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
   )));
  } else {
    BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 443 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 443 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
   "Coordinates format NOT valid!"
# 443 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
   ); &__c[0];}))
# 443 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
   ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 443 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 443 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
   "Coordinates format NOT valid!"
# 443 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
   ); &__c[0];}))
# 443 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
   )));
  }

  return validity;
}

// Checks distance between two coordinate sets and returns true if within MAX_DISTANCE_VALID_WAYPOINT
bool CoordinateValidityRange(long lat1, long lon1, long lat2, long lon2) {
  BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 451 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 451 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 "Checking range between"
# 451 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 451 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 451 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 451 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 "Checking range between"
# 451 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 451 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 )));
  BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 452 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 452 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 " lat: "
# 452 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 452 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 452 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 452 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 " lat: "
# 452 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 452 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 )));
  BlackBoxAppend(lat1); Serial.print(lat1);
  BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 454 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 454 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 " lon: "
# 454 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 454 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 454 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 454 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 " lon: "
# 454 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 454 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 )));
  BlackBoxAppend(lon1); Serial.print(lon1);
  BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 456 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 456 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 "\t and \t"
# 456 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 456 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 456 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 456 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 "\t and \t"
# 456 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 456 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 )));
  BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 457 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 457 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 " lat: "
# 457 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 457 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 457 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 457 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 " lat: "
# 457 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 457 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 )));
  BlackBoxAppend(lat2); Serial.print(lat2);
  BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 459 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 459 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 " lon: "
# 459 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 459 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 459 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 459 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 " lon: "
# 459 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 459 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 )));
  BlackBoxAppendln(lon2); Serial.println(lon2);

  long distance = DistanceBetweenLong(lat1, lon1, lat2, lon2);

  BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 464 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 464 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 "Distance: "
# 464 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 464 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 464 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 464 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 "Distance: "
# 464 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino" 3
 ); &__c[0];}))
# 464 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pathing.ino"
 )));
  BlackBoxAppendln(distance); Serial.println(distance);

  return distance <= 1000 /* max valid waypoint distance (meters)*/;
}
# 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pins_Buttons.ino"
// Sets pinmode of all pins and writes initial values for outputs
void InitAllPins() {
  // External Inputs
  pinMode(2, 0x2);
  pinMode(19, 0x2);
  pinMode(18, 0x2);

  // Heartbeat
  pinMode(3, 0x2);
  pinMode(7, 0x1);

  // Reset
  digitalWrite(6 /* Backup CPU reset pin*/, true);
  pinMode(6 /* Backup CPU reset pin*/, 0x1);

  digitalWrite(7, true);
  digitalWrite(6 /* Backup CPU reset pin*/, true);

  // Status LED/Sound
  pinMode(32, 0x1);
  pinMode(30, 0x1);
  pinMode(28, 0x1);

  digitalWrite(28, 0x0);
  digitalWrite(30, 0x0);
  digitalWrite(32, 0x0);

  // Power control (Relays)
  pinMode(36 /* Relay to motor battery power*/, 0x1);
  pinMode(38 /* External 24V power */, 0x1);
  pinMode(40 /* External 12V power */, 0x1);
  pinMode(42 /* External 5V power */, 0x1);
  pinMode(44, 0x1);
  pinMode(46, 0x1);
  pinMode(48, 0x1);

  digitalWrite(36 /* Relay to motor battery power*/, 0x0);
  digitalWrite(38 /* External 24V power */, 0x0);
  digitalWrite(40 /* External 12V power */, 0x0);
  digitalWrite(42 /* External 5V power */, 0x0);
  digitalWrite(44, 0x0);
  digitalWrite(46, 0x0);
  digitalWrite(48, 0x0);

  // Analog Sensors
  pinMode((55), 0x0);
  pinMode((56), 0x0);
  pinMode((57), 0x0);
  pinMode((54), 0x0);

  // Motor Control
  pinMode(34 /* Enables CAN power, Motor HW Enable and Motor Driver Power*/, 0x1);
  digitalWrite(34 /* Enables CAN power, Motor HW Enable and Motor Driver Power*/, false);
}

// Initialization of the interrupts assigned to buttons
void InitButtons() {
  lastMillisMode = millis();
  lastMillisEstop = millis();

  // Input interrupts
  attachInterrupt(((19) == 2 ? 0 : ((19) == 3 ? 1 : ((19) >= 18 && (19) <= 21 ? 23 - (19) : -1))), ModeButtonInterruptHandler, 2); // Mode Button
  attachInterrupt(((2) == 2 ? 0 : ((2) == 3 ? 1 : ((2) >= 18 && (2) <= 21 ? 23 - (2) : -1))), EstopButtonInterruptHandler, 2); // E-STOP
  attachInterrupt(((3) == 2 ? 0 : ((3) == 3 ? 1 : ((3) >= 18 && (3) <= 21 ? 23 - (3) : -1))), HeartBeatInInterrupt, 2); // Backup-CPU Heartbeat
}

// Activates Emergency Strategy. Triggered by Estop button interrupt
void EstopButtonInterruptHandler() {
  systemReset();
}

// Mode Library activated by mode button interrupt
void ModeButtonInterruptHandler() {
  if (millis() - lastMillisMode > 2500) {
    if (GetStatus(22)) {
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 76 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pins_Buttons.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 76 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pins_Buttons.ino"
     "Mode button press, changing mode to Mode Library"
# 76 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pins_Buttons.ino" 3
     ); &__c[0];}))
# 76 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pins_Buttons.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 76 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pins_Buttons.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 76 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pins_Buttons.ino"
     "Mode button press, changing mode to Mode Library"
# 76 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pins_Buttons.ino" 3
     ); &__c[0];}))
# 76 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pins_Buttons.ino"
     )));
      lastMillisMode = millis();
      SetMode(1 /* Mode Library*/);
    } else {
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 80 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pins_Buttons.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 80 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pins_Buttons.ino"
     "Emergency Mode, Input Blocked!"
# 80 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pins_Buttons.ino" 3
     ); &__c[0];}))
# 80 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pins_Buttons.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 80 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pins_Buttons.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 80 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pins_Buttons.ino"
     "Emergency Mode, Input Blocked!"
# 80 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pins_Buttons.ino" 3
     ); &__c[0];}))
# 80 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Pins_Buttons.ino"
     )));
    }
  }
}
# 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino"
/*

  GeoRover Power control



  Mads Rosenhøj Jeppesen

  Aarhus University

  2021

*/
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino"
// BatteryReading = 19.2 kOhm / (19.2 kOhm + 110 kOhm) * Battery Voltage
// 3.567 V -> ~5V ... 730 -> 1022 (int)
// 24.0 V -> 33.6 V
const int batteryLevelVoltage[] = {1022, 1004, 986, 956, 931, 920, 913, 898, 883, 839, 730};
const int batteryLevelPct[] = {100, 98, 95, 91, 81, 62, 43, 25, 15, 6, 1};
//                            33.6, 33.0, 32.4, 31.44, 30.6, 30.24, 30.0, 29.52, 29.04, 27.6, 24.0

bool charge; // Battery Charge status

// Checks voltage levels above critical values?
bool VoltageCheck() {
  return BatteryLevel() > 20;
}

bool BatteryStatus() {
  return VoltageCheck();
}

bool BatteryStatus(bool print) {
  if (!print) {
    return VoltageCheck();
  }

  BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 32 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 32 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino"
 "Battery Level: "
# 32 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino" 3
 ); &__c[0];}))
# 32 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino"
 ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 32 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 32 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino"
 "Battery Level: "
# 32 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino" 3
 ); &__c[0];}))
# 32 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino"
 )));
  BlackBoxAppendln(BatteryLevel()); Serial.println(BatteryLevel());

  BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 35 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 35 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino"
 "Battery Voltage: "
# 35 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino" 3
 ); &__c[0];}))
# 35 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino"
 ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 35 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 35 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino"
 "Battery Voltage: "
# 35 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino" 3
 ); &__c[0];}))
# 35 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino"
 )));
  BlackBoxAppendln(BatteryVoltage()); Serial.println(BatteryVoltage());
  return VoltageCheck();
}

int BatteryLevel() {
  int voltage = analogRead((54));

  int i;

  while (voltage < batteryLevelVoltage[i]) {
    i++;
  }

  return batteryLevelPct[i];
}

float BatteryVoltage() {
  int voltageInt = analogRead((54));

  float voltageRead = voltageInt * 5.0f / 1024.0f;

  float voltageBattery = voltageRead * 129.2f / 19.2f;

  return voltageBattery;
}

// Sets charge flag based on current battery level, once charge started will charge until BATTERY_STD_RECHARGE before resuming
bool PowerCycle() {
  int batteryLevel = BatteryLevel();

  if (!charge) {
    if (batteryLevel > 80) {
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 68 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 68 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino"
     "------------------------------------"
# 68 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino" 3
     ); &__c[0];}))
# 68 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 68 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 68 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino"
     "------------------------------------"
# 68 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino" 3
     ); &__c[0];}))
# 68 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino"
     )));
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 69 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 69 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino"
     "Battery Recharged to > 80%"
# 69 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino" 3
     ); &__c[0];}))
# 69 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 69 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 69 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino"
     "Battery Recharged to > 80%"
# 69 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino" 3
     ); &__c[0];}))
# 69 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino"
     )));
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 70 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 70 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino"
     "------------------------------------"
# 70 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino" 3
     ); &__c[0];}))
# 70 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 70 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 70 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino"
     "------------------------------------"
# 70 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino" 3
     ); &__c[0];}))
# 70 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino"
     )));
      charge = true;
    }
  }

  if (charge && batteryLevel < 20) {
    BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 76 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 76 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino"
   "------------------------------------"
# 76 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino" 3
   ); &__c[0];}))
# 76 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino"
   ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 76 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 76 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino"
   "------------------------------------"
# 76 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino" 3
   ); &__c[0];}))
# 76 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino"
   )));
    BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 77 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 77 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino"
   "Battery Level < 20%, starting charge cycle"
# 77 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino" 3
   ); &__c[0];}))
# 77 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino"
   ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 77 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 77 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino"
   "Battery Level < 20%, starting charge cycle"
# 77 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino" 3
   ); &__c[0];}))
# 77 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino"
   )));
    BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 78 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 78 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino"
   "------------------------------------"
# 78 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino" 3
   ); &__c[0];}))
# 78 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino"
   ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 78 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 78 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino"
   "------------------------------------"
# 78 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino" 3
   ); &__c[0];}))
# 78 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Power.ino"
   )));
    charge = false;
  }

  return charge;
}


void ResetPowerCycle(){
  charge = true;
}
# 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
/*

  GeoRover local storage protocols



  Mads Rosenhøj Jeppesen

  Aarhus University

  2021

*/
# 8 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 2

// Initialize SD card reader module.
bool InitializeSDReader() {
  bool status = false;
  if (!SDReaderStatus()) {
    status = SD.begin(24 /* 50 (MISO), 51 (MOSI), 52 (SCK)*/);

    SetStatus(14, status);
  }
  return status;
}

void TerminateSDReader() {
  SD.end();
  SetStatus(14, false);
}
// Checks status of SD reader
bool SDReaderStatus() {
  return GetStatus(14);
}

// Query all files on SD card and print to serial prompt
void SDQuery() {
  if (SDReaderStatus()) {
    File file = SD.open("/");
    file.rewindDirectory();
    printFiles(file);
    file.rewindDirectory();
    file.close();
    delay(10);
  }
}

// Print all files to serial port
void printFiles(File dir) {
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 44 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 44 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
 ("Files in system:")
# 44 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
 ); &__c[0];}))
# 44 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 44 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 44 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
 ("Files in system:")
# 44 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
 ); &__c[0];}))
# 44 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
 )));
  while (true) {
    File entry = dir.openNextFile();
    if (!entry) {
      File entry = dir.openNextFile();
      if (!entry)
        break;
    }

    if (!entry.isDirectory()) {
      BlackBoxAppend(entry.name()); Serial.print(entry.name());
      BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 55 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 55 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
     ("\t\t")
# 55 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
     ); &__c[0];}))
# 55 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
     ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 55 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 55 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
     ("\t\t")
# 55 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
     ); &__c[0];}))
# 55 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
     )));
      BlackBoxAppendln(entry.size(), 10); Serial.println(entry.size(), 10);
    }
    entry.close();
  }
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 60 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 60 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
 ("End of storage")
# 60 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
 ); &__c[0];}))
# 60 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 60 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 60 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
 ("End of storage")
# 60 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
 ); &__c[0];}))
# 60 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
 )));
  BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 61 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 61 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
 "------------------------------------"
# 61 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
 ); &__c[0];}))
# 61 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
 ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 61 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
 (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 61 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
 "------------------------------------"
# 61 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
 ); &__c[0];}))
# 61 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
 )));
}

// Print size of file on SD card
void SDSize(char fileName[]) {
  if (SDReaderStatus()) {
    appendCsv(fileName);
    BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 68 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 68 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
   "Opening file: "
# 68 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
   ); &__c[0];}))
# 68 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
   ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 68 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 68 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
   "Opening file: "
# 68 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
   ); &__c[0];}))
# 68 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
   )));
    BlackBoxAppendln(fileName); Serial.println(fileName);
    File file = SD.open(fileName);
    if (file) {
      BlackBoxAppendln("File size: " + (String)file.size() + " bytes"); Serial.println("File size: " + (String)file.size() + " bytes");
      file.close();
    } else {
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 75 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 75 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
     "File not found!"
# 75 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
     ); &__c[0];}))
# 75 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 75 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 75 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
     "File not found!"
# 75 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
     ); &__c[0];}))
# 75 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
     )));
    }
  }
}

// Print all data from datafile to serial port
void SDDownload(char fileName[]) {
  if (SDReaderStatus()) {
    appendCsv(fileName);
    BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 84 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 84 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
   "Downloading file: "
# 84 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
   ); &__c[0];}))
# 84 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
   ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 84 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 84 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
   "Downloading file: "
# 84 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
   ); &__c[0];}))
# 84 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
   )));
    BlackBoxAppendln(fileName); Serial.println(fileName);
    File file = SD.open(fileName);
    if (file) {
      while (file.available()) {
        Serial.write(file.read());
      }
      file.close();
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 92 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 92 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
     "End of File"
# 92 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
     ); &__c[0];}))
# 92 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 92 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 92 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
     "End of File"
# 92 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
     ); &__c[0];}))
# 92 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
     )));
    } else {
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 94 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 94 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
     "File not found!"
# 94 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
     ); &__c[0];}))
# 94 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 94 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 94 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
     "File not found!"
# 94 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
     ); &__c[0];}))
# 94 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
     )));
    }
  }
}

// Delete file on SD card
void SDDelete(char fileName[]) {
  if (SDReaderStatus()) {
    appendCsv(fileName);
    if (SD.exists(fileName)) {
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 104 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 104 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
     "Deleting file: "
# 104 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
     ); &__c[0];}))
# 104 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 104 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 104 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
     "Deleting file: "
# 104 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
     ); &__c[0];}))
# 104 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
     )));
      BlackBoxAppendln(fileName); Serial.println(fileName);
      SD.remove(fileName);
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 107 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 107 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
     "File Removed"
# 107 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
     ); &__c[0];}))
# 107 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 107 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 107 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
     "File Removed"
# 107 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
     ); &__c[0];}))
# 107 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
     )));
    } else {
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 109 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 109 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
     "File not found"
# 109 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
     ); &__c[0];}))
# 109 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 109 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 109 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
     "File not found"
# 109 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
     ); &__c[0];}))
# 109 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
     )));
    }
  }
}

// Create empty file on SD card
void SDCreate(char fileName[]) {
  if (SDReaderStatus()) {
    appendCsv(fileName);
    if (SD.exists(fileName)) {
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 119 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 119 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
     "File already exist"
# 119 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
     ); &__c[0];}))
# 119 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 119 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 119 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
     "File already exist"
# 119 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
     ); &__c[0];}))
# 119 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
     )));
    } else {
      BlackBoxAppendln((reinterpret_cast<const __FlashStringHelper *>(
# 121 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 121 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
     "Creating file: "
# 121 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
     ); &__c[0];}))
# 121 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
     ))); Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 121 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
     (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 121 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
     "Creating file: "
# 121 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino" 3
     ); &__c[0];}))
# 121 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SdReader.ino"
     )));
      BlackBoxAppendln(fileName); Serial.println(fileName);
      File file = SD.open(fileName, (O_READ | O_WRITE | O_CREAT | O_APPEND));
      file.close();
    }
  }
}
# 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SensorStack.ino"
/*

  GeoRover Sensor Stack



  Mads Rosenhøj Jeppesen

  Aarhus University

  2021

*/
# 9 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_SensorStack.ino"
// void InitializeSensorStack() {
//   DEBUG_PRINT("Initializing Sensor stack... ");
//   DEBUG_PRINT("Initializing Communication... ");
//   Wire.begin();
//   DEBUG_PRINTLN("Communication Active.");
// }

// // Checks GNSS signal availability
// bool GPSValid() {
//   bool valid = true;

//   // Signal valid?

//   return valid;
// }

// // Checks temperature sensor availability
// bool TempSensorCheck() {
//   bool valid = true;

//   // Currently measuring?

//   return valid;
// }

// // Checks accelerometer sensor availability and values within limits
// bool AccelSensorCheck() {
//   bool valid = true;

//   // Currently measurements valid?

//   return valid;
// }
# 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_StatusLED.ino"
byte currentLedStatus = 0;
unsigned long lastMillisLed;
unsigned long millisSinceLast;
unsigned long lastMillisModeBlinkState;

// Set LED
void InitStatusLed() {
  // LedBlinkHalt(BINARY_CODE_LED_RED, LED_BLINK_SHORT, LED_BLINK_VERY_SHORT);
  // LedBlinkHalt(BINARY_CODE_LED_YEL, LED_BLINK_SHORT, 0);
}

bool LedStatus() {
  return (digitalRead(32) || digitalRead(30) || digitalRead(32));
}

// Sets current LED Status from byte
// [100] [RED - -]
// [010] [- YEL -]
// [001] [- - GRN]
void LedSet(byte color) {
  // Do nothing if arguments are the same
  if (color == currentLedStatus)
    return;

  digitalWrite(32, (1 /* 001*/ & color) > 0);
  digitalWrite(30, (2 /* 010*/ & color) > 0);
  digitalWrite(28, (4 /* 100*/ & color) > 0);

  currentLedStatus = color;
}

void LedSetSignal(LED_SIGNAL signal){
  switch (signal)
  {
  case LED_SIGNAL_OK:
    LedSet(1 /* 001*/);
    break;
  case LED_SIGNAL_IDLE:
    LedSet(2 /* 010*/);
    break;
  case LED_SIGNAL_LOADING:
    LedSet(2 /* 010*/);
    break;
  case LED_SIGNAL_ERROR:
    LedSet(4 /* 100*/);
    break;
  default:
    LedSet(0);
    break;
  }
}

void LedSetSignal(){
  LedSet(0);
}

// Sets current LED Status from byte and alternate blink based on input durations
// [100] [RED - -]
// [010] [- YEL -]
// [001] [- - GRN]
bool LedBlink(byte color, unsigned int onDuration, unsigned int offDuration) {
  millisSinceLast = millis() - lastMillisLed;
  bool cycleDone = false;
  if (currentLedStatus > 0 && millisSinceLast > onDuration) {
    LedSet(0);
    lastMillisLed = millis();
    cycleDone = true;
  }
  else if (currentLedStatus == 0 && millisSinceLast > offDuration) {
    LedSet(color);
    lastMillisLed = millis();
  }
  return cycleDone;
}

int LedColor = 0;
void LedBlink(byte colorOne, byte colorTwo, unsigned int onDuration, unsigned int offDuration) {
  switch (LedColor) {
    case 0:
      if (LedBlink(colorOne, onDuration/2, offDuration)) LedColor++;
      break;
    case 1:
      if (LedBlink(colorTwo, onDuration/2, 0)) LedColor++;
      break;
    default:
      LedColor = 0;
      break;
  }
}

// Resets current flashing LED and resets timers
void ResetLed(){
  lastMillisLed = 0;
  LedColor = 0;
  LedSet(0);
}

// Flash LED in specified color while halting system for specified duration
void LedBlinkHalt(byte color, unsigned int duration) {
  LedSet(color);
  delay(duration);
  LedSet(0);
}

// Flash LED in specified color while halting system for specified duration, during and after LED duration.
void LedBlinkHalt(byte color, unsigned int duration, unsigned int afterHalt) {
  LedSet(color);
  delay(duration);
  LedSet(0);
  delay(afterHalt);
}

// Flahses LED once in specified color (Halts program)
void LedBlinkSingleShort(byte color) {
  LedBlinkHalt(color, 100);
}

// Flashes LED twice in specified color (very short duration)
void LedBlinkDoubleShort(byte color) {
  LedBlinkHalt(color, 100, 100);
  LedBlinkHalt(color, 100);
}

// Flashes LED twice in specified colors (very short duration)
void LedBlinkDoubleShort(byte color1, byte color2) {
  LedBlinkHalt(color1, 100, 100);
  LedBlinkHalt(color2, 100);
}

// Flashes LED twice in specified color (very short duration)
void LedBlinkTripleShort(byte color) {
  LedBlinkHalt(color, 100, 100);
  LedBlinkHalt(color, 100, 100);
  LedBlinkHalt(color, 100);
}

// Flashes LED twice in specified colors (very short duration)
void LedBlinkTripleShort(byte color1, byte color2, byte color3) {
  LedBlinkHalt(color1, 100, 100);
  LedBlinkHalt(color2, 100, 100);
  LedBlinkHalt(color3, 100);
}

void StrategyStartLed() {
  // switch (mode) {
  //   case MODE_EMERGENCY:
  //     LedBlinkDoubleShort(BINARY_CODE_LED_RED);
  //     break;
  //   case MODE_IDLE:
  //     LedBlinkDoubleShort(BINARY_CODE_LED_YEL);
  //     break;
  //   case MODE_SYSTEMTEST:
  //     LedBlinkDoubleShort(BINARY_CODE_LED_YEL, BINARY_CODE_LED_GRN);
  //     break;
  //   case MODE_REMOTECONTROL:
  //     LedBlinkDoubleShort(BINARY_CODE_LED_GRN, BINARY_CODE_LED_YEL);
  //     break;
  //   case MODE_AUTONOMOUS:
  //     LedBlinkDoubleShort(BINARY_CODE_LED_GRN);
  //     break;
  //   case MODE_MODELIBRARY:
  //     LedBlinkSingleShort(BINARY_CODE_LED_YEL);
  //     break;
  //   default:
  //     break;
  // }
}

// Blink light of given mode (Non-blocking)
void StrategyRunLed(byte mode) {
  switch (mode) {
    case 0 /* Emergency Stop Mode*/:
      LedBlink(4 /* 100*/, 1500, 1500);
      break;
    case 2 /* Standby mode*/:
      LedBlink(2 /* 010*/, 1500, 1500);
      break;
    case 3 /* Test main systems*/:
      LedBlink(4 /* 100*/, 2 /* 010*/, 1500, 1500);
      break;
    case 4 /* System remotely controllable*/:
      LedBlink(1 /* 001*/, 2 /* 010*/, 1500, 1500);
      break;
    case 5 /* Autonomous driving mode*/:
      LedBlink(1 /* 001*/, 1500, 1500);
      break;
    default:
      break;
  }
}

// Blink light of current mode (Non-blocking)
void StrategyRunLed() {
  StrategyRunLed(mode);
}

// Blink light with specific signal (Non-blocking)
void StatusRunLed(LED_SIGNAL signal) {
  switch (signal) {
    case LED_SIGNAL_OK:
      LedBlink(1 /* 001*/, 250, 2500);
      break;
    case LED_SIGNAL_ERROR:
      LedBlink(4 /* 100*/, 250, 2500);
      break;
    case LED_SIGNAL_IDLE:
      LedBlink(2 /* 010*/, 250, 2500);
      break;
    case LED_SIGNAL_LOADING:
      LedBlink(2 /* 010*/, 1 /* 001*/, 250, 2500);
      break;
    default:
      break;
  }
}

// Blink light with specific signal (Blocking)
void StatusHaltLed(LED_SIGNAL signal) {
  switch (signal) {
    case LED_SIGNAL_OK:
      LedBlinkHalt(1 /* 001*/, 750);
      break;
    case LED_SIGNAL_OK_SHORT:
      LedBlinkHalt(1 /* 001*/, 100);
      break;
    case LED_SIGNAL_OK_SHORT_HALT:
      LedBlinkHalt(1 /* 001*/, 100, 100);
      break;
    case LED_SIGNAL_ERROR:
      LedBlinkHalt(4 /* 100*/, 750);
      break;
    case LED_SIGNAL_ERROR_SHORT:
      LedBlinkHalt(4 /* 100*/, 100);
      break;
    case LED_SIGNAL_ERROR_SHORT_HALT:
      LedBlinkHalt(4 /* 100*/, 100, 100);
      break;
    case LED_SIGNAL_IDLE:
      LedBlinkHalt(2 /* 010*/, 250);
      break;
    case LED_SIGNAL_LOADING:
      LedBlinkHalt(2 /* 010*/, 250, 250);
      LedBlinkHalt(1 /* 001*/, 250);
      break;
    default:
      break;
  }
}

void StrategyFinishLed(int strategy) {
}

void ModuleStartLed(int module) {
}

void ModuleStopLed(int module) {
}

void LedTest() {
  Serial.println("Red");
  LedBlinkHalt(4 /* 100*/, 2500, 1500);
  Serial.println("Yellow");
  LedBlinkHalt(2 /* 010*/, 2500, 1500);
  Serial.println("Green");
  LedBlinkHalt(1 /* 001*/, 2500, 1500);

  // LedBlinkDoubleShort(BINARY_CODE_LED_RED,BINARY_CODE_LED_RED);
  // delay(LED_BLINK_SHORT);
  // LedBlinkDoubleShort(BINARY_CODE_LED_RED,BINARY_CODE_LED_YEL);
  // delay(LED_BLINK_SHORT);
  // LedBlinkDoubleShort(BINARY_CODE_LED_RED,BINARY_CODE_LED_GRN);
  // delay(LED_BLINK_SHORT);
  // LedBlinkDoubleShort(BINARY_CODE_LED_YEL,BINARY_CODE_LED_RED);
  // delay(LED_BLINK_SHORT);
  // LedBlinkDoubleShort(BINARY_CODE_LED_YEL,BINARY_CODE_LED_YEL);
  // delay(LED_BLINK_SHORT);
  // LedBlinkDoubleShort(BINARY_CODE_LED_YEL,BINARY_CODE_LED_GRN);
  // delay(LED_BLINK_SHORT);
  // LedBlinkDoubleShort(BINARY_CODE_LED_GRN,BINARY_CODE_LED_RED);
  // delay(LED_BLINK_SHORT);
  // LedBlinkDoubleShort(BINARY_CODE_LED_GRN,BINARY_CODE_LED_YEL);
  // delay(LED_BLINK_SHORT);
  // LedBlinkDoubleShort(BINARY_CODE_LED_GRN,BINARY_CODE_LED_GRN);
}
# 1 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Strategies.ino"
// Set pointers for strategies methods
void InitStrategyMethods() {
  strategyMethods[0][0 /* Emergency Stop Mode*/] = StartStrategyEmergency;
  strategyMethods[1][0 /* Emergency Stop Mode*/] = RunStrategyEmergency;
  strategyMethods[2][0 /* Emergency Stop Mode*/] = FinishStrategyEmergency;
  strategyMethods[3][0 /* Emergency Stop Mode*/] = SelectFunctionEmergency;

  strategyMethods[0][1 /* Mode Library*/] = StartStrategyModeLibrary;
  strategyMethods[1][1 /* Mode Library*/] = RunStrategyModeLibrary;
  strategyMethods[2][1 /* Mode Library*/] = FinishStrategyModeLibrary;
  strategyMethods[3][1 /* Mode Library*/] = SelectFunctionModeLibrary;

  strategyMethods[0][2 /* Standby mode*/] = StartStrategyIdle;
  strategyMethods[1][2 /* Standby mode*/] = RunStrategyIdle;
  strategyMethods[2][2 /* Standby mode*/] = FinishStrategyIdle;
  strategyMethods[3][2 /* Standby mode*/] = SelectFunctionIdle;

  strategyMethods[0][3 /* Test main systems*/] = StartStrategySystemTest;
  strategyMethods[1][3 /* Test main systems*/] = RunStrategySystemTest;
  strategyMethods[2][3 /* Test main systems*/] = FinishStrategySystemTest;
  strategyMethods[3][3 /* Test main systems*/] = SelectFunctionSystemTest;

  strategyMethods[0][4 /* System remotely controllable*/] = StartStrategyRemote;
  strategyMethods[1][4 /* System remotely controllable*/] = RunStrategyRemote;
  strategyMethods[2][4 /* System remotely controllable*/] = FinishStrategyRemote;
  strategyMethods[3][4 /* System remotely controllable*/] = SelectFunctionRemote;

  strategyMethods[0][5 /* Autonomous driving mode*/] = StartStrategyAutonomous;
  strategyMethods[1][5 /* Autonomous driving mode*/] = RunStrategyAutonomous;
  strategyMethods[2][5 /* Autonomous driving mode*/] = FinishStrategyAutonomous;
  strategyMethods[3][5 /* Autonomous driving mode*/] = SelectFunctionAutonomous;
}

// Set last mode from EEPROM
void InitMode() {
  mode = EEPROM.read(0);
  if (!digitalRead(2)) {
    mode = 0 /* Emergency Stop Mode*/;
  }
  if (mode == 1 /* Mode Library*/) mode = 2 /* Standby mode*/;

  SetMode(mode);

  strategyMethods[0][mode]();
}

// Checks if mode is updated and finish exit operations before changing
void ModeUpdater() {
  if (!isModeUpdated) {
    return;
  }

  isModeUpdated = false;
  // Skip finish operation when going to emergency
  if (mode != 0 /* Emergency Stop Mode*/) {
    strategyMethods[2][prevMode](); // finish any operations for prevMode here
  }

  strategyMethods[0][mode](); // init new strategy according to the new mode value
}

void AttachSelectButton() {
  lastMillisSelect = millis();
  attachInterrupt(((18) == 2 ? 0 : ((18) == 3 ? 1 : ((18) >= 18 && (18) <= 21 ? 23 - (18) : -1))), strategyMethods[3][mode], 2);
}

void AttachModeButton() {
  attachInterrupt(((19) == 2 ? 0 : ((19) == 3 ? 1 : ((19) >= 18 && (19) <= 21 ? 23 - (19) : -1))), ModeButtonInterruptHandler, 2);
}

void DetachModeButton() {
  detachInterrupt(((19) == 2 ? 0 : ((19) == 3 ? 1 : ((19) >= 18 && (19) <= 21 ? 23 - (19) : -1))));
}

void DetachSelectButton() {
  detachInterrupt(((18) == 2 ? 0 : ((18) == 3 ? 1 : ((18) >= 18 && (18) <= 21 ? 23 - (18) : -1))));
}

// Tries set the mode and isModeUpdated flag
boolean SetMode(byte newMode) {
  if (newMode < 6 /* Total number of modes*/) {
    prevMode = mode;
    mode = newMode;
    IncrementModeCycle();
    isModeUpdated = true;
    EEPROM.write(0, mode);
    BlackBoxAppend((reinterpret_cast<const __FlashStringHelper *>(
# 87 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Strategies.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 87 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Strategies.ino"
   "Mode Set: "
# 87 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Strategies.ino" 3
   ); &__c[0];}))
# 87 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Strategies.ino"
   ))); Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 87 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Strategies.ino" 3
   (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 87 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Strategies.ino"
   "Mode Set: "
# 87 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Strategies.ino" 3
   ); &__c[0];}))
# 87 "d:\\Projects\\ArcticRover\\src\\arduino\\_Mainframe\\Sys_Strategies.ino"
   )));
    BlackBoxAppendln(((mode) == 0 ? "Emergency" : ((mode) == 1 ? "Mode Library" : ((mode) == 2 ? "Idle" : ((mode) == 3 ? "System Test" : ((mode) == 4 ? "Remote Control" : ((mode) == 5 ? "Autonomous" : "Unknown"))))))); Serial.println(((mode) == 0 ? "Emergency" : ((mode) == 1 ? "Mode Library" : ((mode) == 2 ? "Idle" : ((mode) == 3 ? "System Test" : ((mode) == 4 ? "Remote Control" : ((mode) == 5 ? "Autonomous" : "Unknown")))))));
    return true;
  }
  return false;
}

// Cycle Mode
void IncrementModeCycle() {
  if(mode == 1 /* Mode Library*/){
    return;
  }
  modeCycle = mode;
  if (modeCycle + 1 < 2 /* Minimum mode index, that could be set via mode button.*/ || modeCycle + 1 >= 6 /* Total number of modes*/) {
    modeCycle = 2 /* Minimum mode index, that could be set via mode button.*/;
  } else {
    modeCycle = modeCycle + 1;
  }
}
