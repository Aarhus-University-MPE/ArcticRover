/*
  GeoRover SBUS communication protocols for RF communication
  using: https://github.com/zendes/SBUS
  
  Mads Rosenhøj Jepepsen
  Aarhus University
  2021
*/

#include <SBUS.h>

SBUS sbus(COM_SERIAL_RF);

// Initialize RF Communication
bool InitializeSBUS() {
  sbus.begin(false);
  bool status = COM_SERIAL_RF;

  return status;
}

void TerminateSBUS() {
  COM_SERIAL_RF.end();
}

bool SBusStatus() {
  return (COM_SERIAL_RF);
}

bool SBusTest() {
  bool status = sbus.getGoodFrames() > 0;
  long SBusTestStart = millis();
  long SBusRxLast = 0;

  while (millis() - SBusTestStart < COM_TEST_PERIOD) {
    sbus.process();
    if (millis() - SBusRxLast > SBUS_RX_PERIOD) {
      printChannels();
      SBusRxLast = millis();
    }
  }

  return status;
}

void printChannels() {
  for (int i = 0; i < 8; i++) {
    DEBUG_PRINT("CH ");
    DEBUG_PRINT(i);
    DEBUG_PRINT(": ");
    DEBUG_PRINT(getChannel(i));
    DEBUG_PRINT("\t");
  }
  DEBUG_PRINTLN();
}

// Scale SBUS channel value from range [0, 256] to [-1, 1]
float getChannelFloat(int channel) {
  int value = getChannel(channel);

  float valueFloat = value / 128.0 - 1;

  return valueFloat;
}

int minChannel = 0;
int maxChannel = 128;
// Scale the S.BUS channel values into the range [0, 256]
int getChannel(int channel) {
  int value = sbus.getNormalizedChannel(channel);

  if (value < minChannel) {
    minChannel = value;
  }
  if (value > maxChannel) {
    maxChannel = value;
  }

  float result = value;

  result -= minChannel;
  result /= (maxChannel - minChannel);
  result *= 256;

  return (int)result;
}
