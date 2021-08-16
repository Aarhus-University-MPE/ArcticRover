byte currentLedStatus = B000;
unsigned long lastMillisLed = 0;
unsigned long millisSinceLast;

// Set LED
void InitStatusLed() {
  LedBlinkHalt(BINARY_CODE_LED_RED, LED_BLINK_SHORT, LED_BLINK_VERY_SHORT);
  LedBlinkHalt(BINARY_CODE_LED_YEL, LED_BLINK_SHORT, 0);
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

// Sets current LED Status from byte and alternate blink based on input durations
// [100] [RED - -]
// [010] [- YEL -]
// [001] [- - GRN]
void LedBlink(byte color, unsigned int onDuration, unsigned int offDuration) {
  millisSinceLast = millis() - lastMillisLed;
  if (currentLedStatus > 0 && millisSinceLast > onDuration) {
    LedSet(0);
    lastMillisLed = millis();
  }
  if (currentLedStatus == 0 && millisSinceLast > offDuration) {
    LedSet(color);
    lastMillisLed = millis();
  }
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

// Flashes LED once in specified color (very short duration)
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


void StrategyStartLed(int strategy){
  switch (strategy)
  {
  case MODE_EMERGENCY:
    LedBlinkDoubleShort(BINARY_CODE_LED_RED);
    break;
  case MODE_IDLE:
    LedBlinkDoubleShort(BINARY_CODE_LED_YEL);
    break;
  case MODE_SYSTEMTEST:
    LedBlinkDoubleShort(BINARY_CODE_LED_YEL, BINARY_CODE_LED_GRN);
    break;
  case MODE_REMOTECONTROL:
    LedBlinkDoubleShort(BINARY_CODE_LED_GRN, BINARY_CODE_LED_YEL);
    break;
  case MODE_AUTONOMOUS:
    LedBlinkDoubleShort(BINARY_CODE_LED_GRN);
    break;
  default:
    break;
  }
}

void StrategyRunLed(int strategy){
  // Blink light of current mode
  if (millis() - lastMillistModeBlink > LED_BLINK_VERY_LONG)
  {
    lastMillistModeBlink = millis();
    
    switch (strategy)
    {
    case MODE_EMERGENCY:
      LedBlinkDoubleShort(BINARY_CODE_LED_RED);
      break;
    case MODE_IDLE:
      LedBlinkDoubleShort(BINARY_CODE_LED_YEL);
      break;
    case MODE_SYSTEMTEST:
      LedBlinkDoubleShort(BINARY_CODE_LED_YEL, BINARY_CODE_LED_GRN);
      break;
    case MODE_REMOTECONTROL:
      LedBlinkDoubleShort(BINARY_CODE_LED_GRN, BINARY_CODE_LED_YEL);
      break;
    case MODE_AUTONOMOUS:
      LedBlinkDoubleShort(BINARY_CODE_LED_GRN);
      break;
    default:
      break;
    }
  }
}

void StrategyFinishLed(int strategy){
  
}

void ModuleStartLed(int module){
  
}

void ModuleStopLed(int module){

}