/*
  GeoRover SBUS communication protocols for RF communication
  using: https://github.com/zendes/SBUS
  
  Mads Rosenhøj Jepepsen
  Aarhus University
  2021
*/

#include <SBUS.h>

SBUS sbus(COM_SERIAL_RF);
int sbusTestState = 0;
long millisSbusTestStart = 0;
long millisLastSbusPrint = 0;

// Initialize RF Communication
bool InitializeSBUS() {
  sbus.begin(false);

  return COM_SERIAL_RF;
}

void TerminateSBUS() {
  COM_SERIAL_RF.end();
}

bool SBusStatus() {
  return GetStatus(MODULE_RF);
}

bool SBusTest() {
  bool testDone = false;

  switch (sbusTestState) {
    case 0:
      DEBUG_PRINT(F("SBUS feed starting for: "));
      DEBUG_PRINT(SYS_TEST_DURATION);
      DEBUG_PRINTLN(F(" ms"));
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
      DEBUG_PRINTLINE();
    default:
      break;
  }

  return testDone;
}

void printChannels() {
  for (int i = 0; i < 8; i++) {
    Serial.print(F("CH "));
    Serial.print(i);
    Serial.print(F(": "));
    Serial.print(getChannelFloat(i));
    Serial.print(F("\t"));
  }
  Serial.print(F("Good frames: "));
  Serial.print(sbus.getGoodFrames());
  Serial.println();
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
