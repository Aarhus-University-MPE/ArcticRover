// Sets pinmode of all pins and writes initial values for outputs
void InitAllPins()
{

  // External Inputs
  pinMode(PI_BUTTON_ESTOP, INPUT_PULLUP);
  pinMode(PI_BUTTON_MODE, INPUT_PULLUP);
  pinMode(PI_BUTTON_SELECT, INPUT_PULLUP);

  // Heartbeat
  pinMode(PI_BACKUP_HRTBEAT, INPUT_PULLUP);
  pinMode(PO_BACKUP_HRTBEAT, OUTPUT);
  pinMode(PO_BACKUP_RST, OUTPUT);

  digitalWrite(PO_BACKUP_HRTBEAT, true);
  digitalWrite(PO_BACKUP_RST, true);

  // Status LED/Sound
  pinMode(PO_LED_STATUS_GRN, OUTPUT);
  pinMode(PO_LED_STATUS_YEL, OUTPUT);
  pinMode(PO_LED_STATUS_RED, OUTPUT);

  digitalWrite(PO_LED_STATUS_RED, LOW);
  digitalWrite(PO_LED_STATUS_YEL, LOW);
  digitalWrite(PO_LED_STATUS_GRN, LOW);

  // Power control (Relays)
  pinMode(PO_POWER_MOTOR, OUTPUT);
  pinMode(PO_POWER_12V, OUTPUT);
  pinMode(PO_POWER_5V, OUTPUT);
  pinMode(PO_POWER_RF, OUTPUT);
  pinMode(PO_POWER_IRIDIUM, OUTPUT);

  digitalWrite(PO_POWER_MOTOR, LOW);
  digitalWrite(PO_POWER_12V, LOW);
  digitalWrite(PO_POWER_5V, LOW);
  digitalWrite(PO_POWER_RF, LOW);
  digitalWrite(PO_POWER_IRIDIUM, LOW);

  // Analog Sensors
  pinMode(PA_SENSOR_WIND, INPUT);

  pinMode(PA_SENSOR_TEMP1, INPUT);
  pinMode(PA_SENSOR_TEMP2, INPUT);
  pinMode(PA_SENSOR_TEMP3, INPUT);

  pinMode(PA_SENSOR_RELH1, INPUT);
  pinMode(PA_SENSOR_RELH2, INPUT);
  pinMode(PA_SENSOR_RELH3, INPUT);
}

// Initialization of the interrupts assigned to buttons
void InitButtons()
{

  // Assign mode button interrupt
  attachInterrupt(PI_BUTTON_MODE, ModeButtonInterruptHandler, FALLING);

  // Assign Emergency Stop button interrupt
  attachInterrupt(PI_BUTTON_ESTOP, EstopButtonInterruptHandler, FALLING);

  // Assign heartbeat interrupt
  attachInterrupt(PI_INT_HRTBEAT, HeartBeatInInterrupt, FALLING);
}

// Activates Emergency Strategy. Triggered by Estop button interrupt.
unsigned long lastMillisEstop = 0;
void EstopButtonInterruptHandler()
{
  if (millis() - lastMillisEstop > BTN_DEBOUNCE_TIME)
  {
    lastMillisEstop = millis();
    if (mode != MODE_EMERGENCY)
    {
      SetMode(MODE_EMERGENCY);
    }
    else
    {
      DEBUG_PRINTLN("Emergency Already Set");
    }
  }
}

// Loops through button-selectable modes. Triggered by button interrupt
unsigned long lastMillisMode = 0;
void ModeButtonInterruptHandler()
{
  if (millis() - lastMillisMode > BTN_DEBOUNCE_TIME)
  {
    lastMillisMode = millis();
    if (mode + 1 < MODES_MIN_BROWSABLE || mode + 1 >= MODES_MAX)
    {
      SetMode(MODES_MIN_BROWSABLE);
    }
    else
    {
      SetMode(mode + 1);
    }
  }
}
