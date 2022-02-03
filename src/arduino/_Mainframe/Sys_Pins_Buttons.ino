// Sets pinmode of all pins and writes initial values for outputs
void InitAllPins() {
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
  pinMode(PO_POWER_24V, OUTPUT);
  pinMode(PO_POWER_12V, OUTPUT);
  pinMode(PO_POWER_5V, OUTPUT);
  pinMode(PO_POWER_RF, OUTPUT);
  pinMode(PO_POWER_IRIDIUM, OUTPUT);
  pinMode(PO_POWER_HEATING, OUTPUT);

  digitalWrite(PO_POWER_MOTOR, LOW);
  digitalWrite(PO_POWER_24V, LOW);
  digitalWrite(PO_POWER_12V, LOW);
  digitalWrite(PO_POWER_5V, LOW);
  digitalWrite(PO_POWER_RF, LOW);
  digitalWrite(PO_POWER_IRIDIUM, LOW);
  digitalWrite(PO_POWER_HEATING, LOW);

  // Analog Sensors
  pinMode(PA_SENSOR_TEMP1, INPUT);
  pinMode(PA_SENSOR_TEMP2, INPUT);
  pinMode(PA_SENSOR_TEMP3, INPUT);
  pinMode(PA_SENSOR_BATT, INPUT);

  // Motor Control
  pinMode(PO_MOTOR_EN, OUTPUT);
  digitalWrite(PO_MOTOR_EN, false);
}

// Initialization of the interrupts assigned to buttons
void InitButtons() {
  lastMillisMode = millis();
  lastMillisEstop = millis();

  // Input interrupts
  attachInterrupt(PI_INT_BUTTON_MODE, ModeButtonInterruptHandler, FALLING);    // Mode Button
  attachInterrupt(PI_INT_BUTTON_ESTOP, EstopButtonInterruptHandler, FALLING);  // E-STOP
  attachInterrupt(PI_INT_HRTBEAT, HeartBeatInInterrupt, FALLING);              // Backup-CPU Heartbeat
}

// Activates Emergency Strategy. Triggered by Estop button interrupt
void EstopButtonInterruptHandler() {
  systemReset();
}

// Mode Library activated by mode button interrupt
void ModeButtonInterruptHandler() {
  if (millis() - lastMillisMode > BTN_DEBOUNCE_TIME_LONG) {
    if (GetStatus(MODULE_ESTOP)) {
      DEBUG_PRINTLN("Mode button press, changing mode to Mode Library");
      lastMillisMode = millis();
      SetMode(MODE_MODELIBRARY);
    } else {
      DEBUG_PRINTLN("Emergency Mode, Input Blocked!");
    }
  }
}
