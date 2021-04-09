// Initialization of the interrupts assigned to buttons
void InitButtons(){
  
}




// Loops through button-selectable modes. Triggered by button interrupt
unsigned long lastMillisMode = 0;
void ModeButtonInterruptHandler() {
  if (millis() - lastMillisMode > 300) {
    lastMillisMode=millis();
    if (mode+1 < MODES_MIN_BROWSABLE || mode+1 >= MODES_MAX)
      SetMode(MODES_MIN_BROWSABLE);
    else 
      SetMode(mode+1);
  }
}

// Set or Unset emergency stop. Triggered by emergency button interupt
unsigned long lastMillisEmergencyStop = 0;
void EmergencyStopInterruptHandler() {
  if (millis() - lastMillisEmergencyStop > 300) {
    lastMillisEmergencyStop=millis();
    if (EmergencyStop)
      EmergencyStop = TRUE;
      SetMode(MODES_EMERGENCY)
    else 
      EmergencyStop = FALSE;
      SetMode(prevMode)
  }
}