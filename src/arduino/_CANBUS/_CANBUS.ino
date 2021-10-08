/*
  GeoRover CanBUS communication protocols for openCAN communication (Motors)

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021
*/

#include <mcp2515.h>

#define CANBUS_DATA_LENGTH      8
#define CANBBUS_SPEED           CAN_500KBPS

struct can_frame canMsgLeft;
struct can_frame canMsgRight;

MCP2515 mcp2515(53);

bool InitializeCanBus() {
  SPI.begin(); //Begins SPI communication
  mcp2515.reset();
  mcp2515.setBitrate(CANBBUS_SPEED, MCP_8MHZ); //Sets CAN at speed 500KBPS and Clock 8MHz
  mcp2515.setNormalMode();

  canMsgLeft.can_dlc = CANBUS_DATA_LENGTH;
  canMsgLeft.can_id = 0x012;

  delay(20);

  return mcp2515.checkReceive();

}

void TerminateCanBus() {
  SPI.end();
}


void CanBusTransmit(struct can_frame canMsg) {
  DEBUG_PRINT("Transmitting CAN Bus message... ");

  mcp2515.sendMessage(&canMsg); //Sends the CAN message

  DEBUG_PRINTLN("CAN Bus message sent.");
}


void PopulateCanMsg(struct can_frame canMsg, unsigned int _data) {
  uint8_t dataArray[CANBUS_DATA_LENGTH];

  for (int i = 0; i < CANBUS_DATA_LENGTH; i++) {
    canMsg.data[i] = (_data >> 8 * i) & 0xFF;
  }
}

bool InitializeMotor(bool motor){
  bool status = true;

  return status;
}