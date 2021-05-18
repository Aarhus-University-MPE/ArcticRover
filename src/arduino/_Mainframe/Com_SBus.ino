/*
GeoRover SBUS communication protocols for RF communication

Mads Rosenhøj Jepepsen
Aarhus University 
2021
*/

#include <SBUS.h>
#include <limits.h>


SBUS sbus(Serial3);

// Initialize RF Communication
void InitializeSBUS(){
    DEBUG_PRINT("Initializing RF communication... ");
    sbus.begin(false);
    LedBlinkHalt(BINARY_CODE_LED_YEL, LED_BLINK_VERY_SHORT, LED_BLINK_VERY_SHORT);
    LedBlinkHalt(BINARY_CODE_LED_GRN, LED_BLINK_VERY_SHORT, 0);
    DEBUG_PRINTLN("Sbus Initialized.");
}

void TerminateSBUS(){
  DEBUG_PRINT("Terminating RF communication... ");
  sbus.end();
  LedBlinkHalt(BINARY_CODE_LED_YEL, LED_BLINK_VERY_SHORT, LED_BLINK_VERY_SHORT);
  LedBlinkHalt(BINARY_CODE_LED_RED, LED_BLINK_VERY_SHORT, 0);
  DEBUG_PRINTLN("RF communication Terminated. ");
}

static int minChannel = INT_MAX;
static int maxChannel = INT_MIN;

// Scale SBUS channel value from range [0, 256] to [-1, 1]
float getChannelFloat(int channel){
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