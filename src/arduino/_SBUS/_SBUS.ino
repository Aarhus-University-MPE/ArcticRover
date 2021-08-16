/*
  GeoRover SBUS communication protocols for RF communication

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021
*/

#include <SBUS.h>
#include <limits.h>

SBUS sbus(Serial3);

void setup(){
  Serial.begin(115200);
  pinMode(A0,INPUT);

  InitializeSBUS();
}

void loop(){
  sbus.process();
  Serial.println(sbus.getGoodFrames());
  for (int i = 0; i < 16; i++)
  {
    Serial.print(getChannelFloat(i));
    delay(10);
  }
  Serial.println();
  Serial.print("Analog value: ");
  Serial.println(analogRead(A0));
  delay(200);
}

// Initialize RF Communication
bool InitializeSBUS()
{
  Serial.println("Initializing Short Range Communication... ");
  sbus.begin(false);
  bool status = Serial3;
  if (status)
  {
    Serial.println("Initialized.");
  }
  else
  {
    Serial.println("Failed.");
  }
  return status;
}


static int minChannel = INT_MAX;
static int maxChannel = INT_MIN;

// Scale SBUS channel value from range [0, 256] to [-1, 1]
float getChannelFloat(int channel)
{
  int value = getChannel(channel);

  float valueFloat = value / 128.0 - 1;

  return valueFloat;
}

// Scale the S.BUS channel values into the range [0, 256]
int getChannel(int channel)
{
  int value = sbus.getChannel(channel);

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
