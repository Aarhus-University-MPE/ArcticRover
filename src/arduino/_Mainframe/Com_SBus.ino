/*
  GeoRover SBUS communication protocols for RF communication
  using: https://github.com/zendes/SBUS

  Mads Rosenhøj Jeppesen
  Aarhus University
  2021
*/

#include <SBUS.h>

SBUS sbus(COM_SERIAL_RF);
bool sBusStatus;
int sbusTestState;
long millisSbusTestStart;
long millisLastSbusPrint;
long millisLastSBusUpdate;

// Initialize RF Communication
bool InitializeSBUS() {
  digitalWrite(PO_POWER_RF, HIGH);
  sBusStatus = true;

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
      testDone      = true;
      SetStatus(MODULE_RF, sbus.getGoodFrames() > 0);
      DEBUG_PRINTLINE();
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
  if (millis() - millisLastSBusUpdate < REMOTE_PROCESS_DT) {
    return sBusStatus;
  }
  millisLastSBusUpdate = millis();

  if (getChannel(6) < REMOTE_CHANNEL_HIGH) {  // Enable (SF)
    MotorUpdate(0, 0);
    return sBusStatus;
  }

  float throttle1 = getChannelFloatFull(1);  // Left stick Vertical
  float dir       = getChannelFloat(2);      // Right stick Horisontal
  float throttle2 = getChannelFloat(3);      // Right stick Vertical
  int gear        = getChannel(5);           // Gear select (SA)

  int forwardDir;
  float speed;

  // Primary input (Left Stick)
  if (throttle1 > CONTROLLER_DEADZONE_FLOAT) {
    if (gear < REMOTE_CHANNEL_LOW) {
      speed = throttle1 * -1.0f;
    } else if (gear > REMOTE_CHANNEL_HIGH) {
      speed = throttle1;
    } else {
      speed = 0;
    }
    MotorUpdate(dir, speed);
  }
  // Secondary input (Right Stick)
  else if (abs(throttle2) > CONTROLLER_DEADZONE_FLOAT && gear < REMOTE_CHANNEL_HIGH && gear > REMOTE_CHANNEL_LOW) {
    if (throttle2 > 0) {
      speed = throttle2 / 2.0f;
    } else {
      speed = throttle2 / 2.0f;
    }
    MotorUpdateTorque(dir, speed);

  } else {
    MotorUpdate(0, 0);
  }

  // First timeout error returns true (catches large first read getLastTime)
  if (millis() - (unsigned long)sbus.getLastTime() < SBUS_TIMEOUT && sBusStatus) {
    sBusStatus = false;
    return true;
  } else {
    sBusStatus = true;
  }

  return sBusStatus;
}

void SBusPrint() {
  if (millis() - millisLastSbusPrint < 1000) {
    return;
  }
  millisLastSbusPrint = millis();
  printChannels();
  Serial.println(F("-------------------------------"));
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
