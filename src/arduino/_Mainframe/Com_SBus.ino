/*
  GeoRover SBUS communication protocols for RF communication

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021
*/

#include <SBUS.h>
#include <limits.h>


SBUS sbus(COM_SERIAL_RF);

// Initialize RF Communication
bool InitializeSBUS() {
  DEBUG_PRINT("Initializing RF communication... ");
  bool status = true;
  sbus.begin(false);
  LedBlinkDoubleShort(BINARY_CODE_LED_YEL, BINARY_CODE_LED_GRN);
  DEBUG_PRINTLN("Sbus Initialized.");
  return status;
}

void TerminateSBUS() {
  DEBUG_PRINT("Terminating RF communication... ");
  // sbus.deactivate();
  LedBlinkDoubleShort(BINARY_CODE_LED_YEL, BINARY_CODE_LED_RED);
  DEBUG_PRINTLN("RF communication Terminated. ");
}

static int minChannel = INT_MAX;
static int maxChannel = INT_MIN;

// Scale SBUS channel value from range [0, 256] to [-1, 1]
float getChannelFloat(int channel) {
  int value = getChannel(channel);

  float valueFloat = value / 128.0 - 1;

  return valueFloat;
}


// Scale the S.BUS channel values into the range [0, 256]
int getChannel(int channel) {
  int value = sbus.getChannel(channel);

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
