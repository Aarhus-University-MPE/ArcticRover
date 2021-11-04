/*
  GeoRover SBUS communication protocols for RF communication

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021
*/

#include <SBUS.h>

SBUS sbus(COM_SERIAL_RF);

// Initialize RF Communication
bool InitializeSBUS()
{
  sbus.begin(false);
  bool status = COM_SERIAL_RF;

  return status;
}

void TerminateSBUS()
{
  COM_SERIAL_RF.end();
}

bool SBusStatus()
{
  return (COM_SERIAL_RF);
}


// Scale SBUS channel value from range [0, 256] to [-1, 1]
float getChannelFloat(int channel)
{
  int value = getChannel(channel);

  float valueFloat = value / 128.0 - 1;

  return valueFloat;
}

int minChannel = 0;
int maxChannel = 128;
// Scale the S.BUS channel values into the range [0, 256]
int getChannel(int channel)
{
  int value = sbus.getNormalizedChannel(channel);

  if (value < minChannel)
  {
    minChannel = value;
  }
  if (value > maxChannel)
  {
    maxChannel = value;
  }

  float result = value;

  result -= minChannel;
  result /= (maxChannel - minChannel);
  result *= 256;

  return (int)result;
}
