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
  pinMode(PO_POWER_MOTOR_ON,  OUTPUT);
  pinMode(PO_POWER_MOTOR_OFF, OUTPUT);
  pinMode(PO_POWER_12V,       OUTPUT);
  pinMode(PO_POWER_5V,        OUTPUT);
  pinMode(PO_POWER_RF,        OUTPUT);
  pinMode(PO_POWER_IRIDIUM,   OUTPUT);

  digitalWrite(PO_POWER_MOTOR_ON,   LOW);
  digitalWrite(PO_POWER_MOTOR_OFF,  LOW);
  digitalWrite(PO_POWER_12V,        LOW);
  digitalWrite(PO_POWER_5V,         LOW);
  digitalWrite(PO_POWER_RF,         LOW);
  digitalWrite(PO_POWER_IRIDIUM,    LOW);

  // Analog Sensors
  pinMode(PA_SENSOR_TEMP1, INPUT);
  pinMode(PA_SENSOR_TEMP2, INPUT);
  pinMode(PA_SENSOR_TEMP3, INPUT);

  pinMode(PA_SENSOR_RELH1, INPUT);
  pinMode(PA_SENSOR_RELH2, INPUT);
}

// Initialization of the interrupts assigned to buttons
void InitButtons()
{
  lastMillisMode = millis();
  lastMillisEstop = millis();
  // Assign mode button interrupt
  attachInterrupt(PI_INT_BUTTON_MODE, ModeButtonInterruptHandler, FALLING);

  // Assign Emergency Stop button interrupt
  attachInterrupt(PI_INT_BUTTON_ESTOP, EstopButtonInterruptHandler, FALLING);
  SetStatus(MODULE_ESTOP, true);

  // Assign heartbeat interrupt
  attachInterrupt(PI_INT_HRTBEAT, HeartBeatInInterrupt, FALLING);
}

// Activates Emergency Strategy. Triggered by Estop button interrupt.
void EstopButtonInterruptHandler()
{
  if (millis() - lastMillisEstop > BTN_DEBOUNCE_TIME)
  {
    lastMillisEstop = millis();
    if (mode != MODE_EMERGENCY)
    {
      SetMode(MODE_EMERGENCY);
    }
  }
}



// Loops through button-selectable modes. Triggered by button interrupt
void ModeButtonInterruptHandler()
{
  if (millis() - lastMillisMode > BTN_DEBOUNCE_TIME_LONG)
  {
    if(GetStatus(MODE_EMERGENCY)){
      DEBUG_PRINTLN("Mode button press, changing mode to Mode Library");
      lastMillisMode = millis();
      SetMode(MODE_MODELIBRARY);    
    }
    else
    {
      DEBUG_PRINTLN("Emergency Mode, Input Blocked!");
    }
    
    
  }
}
