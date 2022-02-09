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
  return GetStatus(MODULE_RF);
}

int sbusTestState = 0;
long millisSbusTestStart = 0;
long millisLastSbusPrint = 0;

bool SBusTest() {
  bool testDone = false;

  switch (sbusTestState) {
    case 0:
      DEBUG_PRINT("SBUS feed starting for: ");
      DEBUG_PRINT(SYS_TEST_DURATION);
      DEBUG_PRINTLN(" ms");
      millisSbusTestStart = millis();
      sbusTestState++;
      break;
    case 1:
      sbus.process();
      if (millis() - millisLastSbusPrint > SYS_PRINT_PERIOD_SHORT) {
        millisLastSbusPrint = millis();
        printChannels();
      }

      if (millis() - millisSbusTestStart > SYS_TEST_DURATION) sbusTestState++;
      break;
    case 2:
      sbusTestState = 0;
      testDone = true;
      SetStatus(MODULE_RF,sbus.getGoodFrames() > 0);
    default:
      break;
  }

  return testDone;
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
