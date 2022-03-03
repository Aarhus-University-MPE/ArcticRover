/*
  GeoRover SBUS communication protocols for RF communication

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021
*/

#include <SBUS.h>

SBUS sbus(Serial3);


void setup(){
  Serial.begin(115200);
  pinMode(44, OUTPUT);

  InitializeSBUS();
}

unsigned long lastMillisPost = 0;

void loop(){
  
  if (millis() - lastMillisPost > 200)
  {
    lastMillisPost = millis();
    printSBUSStatus();
  }
  sbus.process();
  //delay(1000);
}

// Initialize RF Communication
bool InitializeSBUS()
{
  digitalWrite(44, HIGH);
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




void printSBUSStatus()
{
  Serial.print("Ch1  ");
  Serial.println(sbus.getNormalizedChannel(1));
  Serial.print("Ch2  ");
  Serial.println(sbus.getNormalizedChannel(2));
  Serial.print("Ch3  ");
  Serial.println(sbus.getNormalizedChannel(3));
  Serial.print("Ch4  ");
  Serial.println(sbus.getNormalizedChannel(4));
  Serial.print("Ch5  ");
  Serial.println(sbus.getNormalizedChannel(5));
  Serial.print("Ch6  ");
  Serial.println(sbus.getNormalizedChannel(6));
  Serial.print("Ch7  ");
  Serial.println(sbus.getNormalizedChannel(7));
  Serial.print("Ch8  ");
  Serial.println(sbus.getNormalizedChannel(8));
  Serial.println();
  Serial.print("Failsafe: ");
  if (sbus.getFailsafeStatus() == SBUS_FAILSAFE_ACTIVE) {
    Serial.println("Active");
  }
  if (sbus.getFailsafeStatus() == SBUS_FAILSAFE_INACTIVE) {
    Serial.println("Not Active");
  }

  Serial.print("Data loss on connection: ");
  Serial.print(sbus.getFrameLoss());
  Serial.println("%");

  Serial.print("Frames: ");
  Serial.print(sbus.getGoodFrames());
  Serial.print(" / ");
  Serial.print(sbus.getLostFrames());
  Serial.print(" / ");
  Serial.println(sbus.getDecoderErrorFrames());

  Serial.print("Time diff: ");
  Serial.println(millis() - (unsigned long)sbus.getLastTime());
}
// static int minChannel = INT_MAX;
// static int maxChannel = INT_MIN;

// // Scale SBUS channel value from range [0, 256] to [-1, 1]
// float getChannelFloat(int channel)
// {
//   int value = getChannel(channel);

//   float valueFloat = value / 128.0 - 1;

//   return valueFloat;
// }

// // Scale the S.BUS channel values into the range [0, 256]
// int getChannel(int channel)
// {
//   int value = sbus.getChannel(channel);

//   if (value < minChannel)
//   {
//     minChannel = value;
//   }
//   if (value > maxChannel)
//   {
//     maxChannel = value;
//   }

//   float result = value;

//   result -= minChannel;
//   result /= (maxChannel - minChannel);
//   result *= 256;

//   return (int)result;
// }
