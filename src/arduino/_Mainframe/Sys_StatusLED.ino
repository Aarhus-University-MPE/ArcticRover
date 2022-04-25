/*
  GeoRover Status LED Control

  Mads Rosenhøj Jeppesen
  Aarhus University
  2021
*/

byte currentLedStatus = B000;
unsigned long lastMillisLed;
unsigned long millisSinceLast;
unsigned long lastMillisModeBlinkState;

// Set LED
void InitStatusLed() {
  // LedBlinkHalt(BINARY_CODE_LED_RED, LED_BLINK_SHORT, LED_BLINK_VERY_SHORT);
  // LedBlinkHalt(BINARY_CODE_LED_YEL, LED_BLINK_SHORT, 0);
}

bool LedStatus() {
  return (digitalRead(PO_LED_STATUS_GRN) || digitalRead(PO_LED_STATUS_YEL) || digitalRead(PO_LED_STATUS_GRN));
}

// Sets current LED Status from byte
// [100] [RED - -]
// [010] [- YEL -]
// [001] [- - GRN]
void LedSet(byte color) {
  // Do nothing if arguments are the same
  if (color == currentLedStatus)
    return;

  digitalWrite(PO_LED_STATUS_GRN, (BINARY_CODE_LED_GRN & color) > 0);
  digitalWrite(PO_LED_STATUS_YEL, (BINARY_CODE_LED_YEL & color) > 0);
  digitalWrite(PO_LED_STATUS_RED, (BINARY_CODE_LED_RED & color) > 0);

  currentLedStatus = color;
}

void LedSetSignal(LED_SIGNAL signal) {
  switch (signal) {
    case LED_SIGNAL_OK:
      LedSet(BINARY_CODE_LED_GRN);
      break;
    case LED_SIGNAL_IDLE:
      LedSet(BINARY_CODE_LED_YEL);
      break;
    case LED_SIGNAL_LOADING:
      LedSet(BINARY_CODE_LED_YEL);
      break;
    case LED_SIGNAL_ERROR:
      LedSet(BINARY_CODE_LED_RED);
      break;
    default:
      LedSet(0);
      break;
  }
}

void LedSetSignal() {
  LedSet(0);
}

// Sets current LED Status from byte and alternate blink based on input durations
// [100] [RED - -]
// [010] [- YEL -]
// [001] [- - GRN]
bool LedBlink(byte color, unsigned int onDuration, unsigned int offDuration) {
  millisSinceLast = millis() - lastMillisLed;
  bool cycleDone  = false;
  if (currentLedStatus > 0 && millisSinceLast > onDuration) {
    LedSet(0);
    lastMillisLed = millis();
    cycleDone     = true;
  } else if (currentLedStatus == 0 && millisSinceLast > offDuration) {
    LedSet(color);
    lastMillisLed = millis();
  }
  return cycleDone;
}

int LedColor = 0;
void LedBlink(byte colorOne, byte colorTwo, unsigned int onDuration, unsigned int offDuration) {
  switch (LedColor) {
    case 0:
      if (LedBlink(colorOne, onDuration / 2, offDuration)) LedColor++;
      break;
    case 1:
      if (LedBlink(colorTwo, onDuration / 2, 0)) LedColor++;
      break;
    default:
      LedColor = 0;
      break;
  }
}

// Resets current flashing LED and resets timers
void ResetLed() {
  lastMillisLed = 0;
  LedColor      = 0;
  LedSet(0);
}

// Flash LED in specified color while halting system for specified duration
void LedBlinkHalt(byte color, unsigned int duration) {
  LedSet(color);
  delay(duration);
  LedSet(0);
}

// Flash LED in specified color while halting system for specified duration, during and after LED duration.
void LedBlinkHalt(byte color, unsigned int duration, unsigned int afterHalt) {
  LedSet(color);
  delay(duration);
  LedSet(0);
  delay(afterHalt);
}

// Flahses LED once in specified color (Halts program)
void LedBlinkSingleShort(byte color) {
  LedBlinkHalt(color, LED_BLINK_VERY_SHORT);
}

// Flashes LED twice in specified color (very short duration)
void LedBlinkDoubleShort(byte color) {
  LedBlinkHalt(color, LED_BLINK_VERY_SHORT, LED_BLINK_VERY_SHORT);
  LedBlinkHalt(color, LED_BLINK_VERY_SHORT);
}

// Flashes LED twice in specified colors (very short duration)
void LedBlinkDoubleShort(byte color1, byte color2) {
  LedBlinkHalt(color1, LED_BLINK_VERY_SHORT, LED_BLINK_VERY_SHORT);
  LedBlinkHalt(color2, LED_BLINK_VERY_SHORT);
}

// Flashes LED twice in specified color (very short duration)
void LedBlinkTripleShort(byte color) {
  LedBlinkHalt(color, LED_BLINK_VERY_SHORT, LED_BLINK_VERY_SHORT);
  LedBlinkHalt(color, LED_BLINK_VERY_SHORT, LED_BLINK_VERY_SHORT);
  LedBlinkHalt(color, LED_BLINK_VERY_SHORT);
}

// Flashes LED twice in specified colors (very short duration)
void LedBlinkTripleShort(byte color1, byte color2, byte color3) {
  LedBlinkHalt(color1, LED_BLINK_VERY_SHORT, LED_BLINK_VERY_SHORT);
  LedBlinkHalt(color2, LED_BLINK_VERY_SHORT, LED_BLINK_VERY_SHORT);
  LedBlinkHalt(color3, LED_BLINK_VERY_SHORT);
}

void StrategyStartLed() {
  // switch (mode) {
  //   case MODE_EMERGENCY:
  //     LedBlinkDoubleShort(BINARY_CODE_LED_RED);
  //     break;
  //   case MODE_IDLE:
  //     LedBlinkDoubleShort(BINARY_CODE_LED_YEL);
  //     break;
  //   case MODE_SYSTEMTEST:
  //     LedBlinkDoubleShort(BINARY_CODE_LED_YEL, BINARY_CODE_LED_GRN);
  //     break;
  //   case MODE_REMOTECONTROL:
  //     LedBlinkDoubleShort(BINARY_CODE_LED_GRN, BINARY_CODE_LED_YEL);
  //     break;
  //   case MODE_AUTONOMOUS:
  //     LedBlinkDoubleShort(BINARY_CODE_LED_GRN);
  //     break;
  //   case MODE_MODELIBRARY:
  //     LedBlinkSingleShort(BINARY_CODE_LED_YEL);
  //     break;
  //   default:
  //     break;
  // }
}

// Blink light of given mode (Non-blocking)
void StrategyRunLed(byte mode) {
  switch (mode) {
    case MODE_EMERGENCY:
      LedBlink(BINARY_CODE_LED_RED, LED_BLINK_LONG, LED_BLINK_LONG);
      break;
    case MODE_IDLE:
      LedBlink(BINARY_CODE_LED_YEL, LED_BLINK_LONG, LED_BLINK_LONG);
      break;
    case MODE_SYSTEMTEST:
      LedBlink(BINARY_CODE_LED_RED, BINARY_CODE_LED_YEL, LED_BLINK_LONG, LED_BLINK_LONG);
      break;
    case MODE_REMOTECONTROL:
      LedBlink(BINARY_CODE_LED_GRN, BINARY_CODE_LED_YEL, LED_BLINK_LONG, LED_BLINK_LONG);
      break;
    case MODE_AUTONOMOUS:
      LedBlink(BINARY_CODE_LED_GRN, LED_BLINK_LONG, LED_BLINK_LONG);
      break;
    default:
      break;
  }
}

// Flash LED indicating current status of Remote Control
void RemoteControlStatusLed() {
  if (!remoteActive) {
    StrategyRunLed(MODE_REMOTECONTROL);
    return;
  }

  if (MotorStatusLeft() && MotorStatusRight() && SBusStatus()) {
    StrategyRunLed(MODE_AUTONOMOUS);
  } else {
    StrategyRunLed(MODE_EMERGENCY);
  }
}

// Blink light of current mode (Non-blocking)
void StrategyRunLed() {
  StrategyRunLed(mode);
}

// Blink light with specific signal (Non-blocking)
void StatusRunLed(LED_SIGNAL signal) {
  switch (signal) {
    case LED_SIGNAL_OK:
      LedBlink(BINARY_CODE_LED_GRN, LED_BLINK_SHORT, LED_BLINK_VERY_LONG);
      break;
    case LED_SIGNAL_ERROR:
      LedBlink(BINARY_CODE_LED_RED, LED_BLINK_SHORT, LED_BLINK_VERY_LONG);
      break;
    case LED_SIGNAL_IDLE:
      LedBlink(BINARY_CODE_LED_YEL, LED_BLINK_SHORT, LED_BLINK_VERY_LONG);
      break;
    case LED_SIGNAL_LOADING:
      LedBlink(BINARY_CODE_LED_YEL, BINARY_CODE_LED_GRN, LED_BLINK_SHORT, LED_BLINK_VERY_LONG);
      break;
    default:
      break;
  }
}

// Blink light with specific signal (Blocking)
void StatusHaltLed(LED_SIGNAL signal) {
  switch (signal) {
    case LED_SIGNAL_OK:
      LedBlinkHalt(BINARY_CODE_LED_GRN, LED_BLINK_NORMAL);
      break;
    case LED_SIGNAL_OK_SHORT:
      LedBlinkHalt(BINARY_CODE_LED_GRN, LED_BLINK_SHORT);
      break;
    case LED_SIGNAL_OK_SHORT_HALT:
      LedBlinkHalt(BINARY_CODE_LED_GRN, LED_BLINK_SHORT, LED_BLINK_SHORT);
      break;
    case LED_SIGNAL_ERROR:
      LedBlinkHalt(BINARY_CODE_LED_RED, LED_BLINK_NORMAL);
      break;
    case LED_SIGNAL_ERROR_SHORT:
      LedBlinkHalt(BINARY_CODE_LED_RED, LED_BLINK_SHORT);
      break;
    case LED_SIGNAL_ERROR_SHORT_HALT:
      LedBlinkHalt(BINARY_CODE_LED_RED, LED_BLINK_SHORT, LED_BLINK_SHORT);
      break;
    case LED_SIGNAL_IDLE:
      LedBlinkHalt(BINARY_CODE_LED_YEL, LED_BLINK_SHORT);
      break;
    case LED_SIGNAL_LOADING:
      LedBlinkHalt(BINARY_CODE_LED_YEL, LED_BLINK_SHORT, LED_BLINK_SHORT);
      LedBlinkHalt(BINARY_CODE_LED_GRN, LED_BLINK_SHORT);
      break;
    default:
      break;
  }
}

void StrategyFinishLed(int strategy) {
}

void ModuleStartLed(int module) {
}

void ModuleStopLed(int module) {
}

void LedTest() {
  DEBUG_PRINTLN("Red");
  LedBlinkHalt(BINARY_CODE_LED_RED, LED_BLINK_VERY_LONG, LED_BLINK_LONG);
  DEBUG_PRINTLN("Yellow");
  LedBlinkHalt(BINARY_CODE_LED_YEL, LED_BLINK_VERY_LONG, LED_BLINK_LONG);
  DEBUG_PRINTLN("Green");
  LedBlinkHalt(BINARY_CODE_LED_GRN, LED_BLINK_VERY_LONG, LED_BLINK_LONG);

  // LedBlinkDoubleShort(BINARY_CODE_LED_RED,BINARY_CODE_LED_RED);
  // delay(LED_BLINK_SHORT);
  // LedBlinkDoubleShort(BINARY_CODE_LED_RED,BINARY_CODE_LED_YEL);
  // delay(LED_BLINK_SHORT);
  // LedBlinkDoubleShort(BINARY_CODE_LED_RED,BINARY_CODE_LED_GRN);
  // delay(LED_BLINK_SHORT);
  // LedBlinkDoubleShort(BINARY_CODE_LED_YEL,BINARY_CODE_LED_RED);
  // delay(LED_BLINK_SHORT);
  // LedBlinkDoubleShort(BINARY_CODE_LED_YEL,BINARY_CODE_LED_YEL);
  // delay(LED_BLINK_SHORT);
  // LedBlinkDoubleShort(BINARY_CODE_LED_YEL,BINARY_CODE_LED_GRN);
  // delay(LED_BLINK_SHORT);
  // LedBlinkDoubleShort(BINARY_CODE_LED_GRN,BINARY_CODE_LED_RED);
  // delay(LED_BLINK_SHORT);
  // LedBlinkDoubleShort(BINARY_CODE_LED_GRN,BINARY_CODE_LED_YEL);
  // delay(LED_BLINK_SHORT);
  // LedBlinkDoubleShort(BINARY_CODE_LED_GRN,BINARY_CODE_LED_GRN);
}