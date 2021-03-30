/*
GeoRover mainframe - Manages primary systems as the Rover backbone.

Mads Rosenhøj Jepepsen
Aarhus University 
2021
*/

#include "Constants.h"
#include "_shared.h"

#include <SPI.h>
#include <SD.h>
#include <Wire.h>

typedef void (*functionPtr)();

//current and previous mode (strategy)
byte mode; 
byte prevMode;
boolean isModeUpdated = false;
boolean EmergencyStop = false;

// LED wrapper ptrs Read more: http://playground.arduino.cc/Code/LED
LED *statusLED1;
LED *statusLED2;

// pointers to key strategy methods.
// [0][MODES_MAX] - start sequence
// [1][MODES_MAX] - main sequence
// [2][MODES_MAX] - end sequence
functionPtr strategyMethods[3][MODES_MAX];

// ------------------------------------------------------------ //
//                            SETUP                             //
// ------------------------------------------------------------ //
void setup() {

  // Debug
  DBG_ONLY(Serial.begin(38400));
  DBG_ONLY(while (!Serial));
  DEBUG_PRINTLN("Debug mode. Entered setup...");
  
  // System Initialization
  InitAllPins();
  InitButtons();

  InitMode();
  InitStrategyMethods();

  InitStatusLeds();

  InitIRSensor();
  InitServiceInterrupt();

  // Setup finished
  DEBUG_PRINTLN("Setup complete.");
}

// ------------------------------------------------------------ //
//                          MAIN LOOP                           //
// ------------------------------------------------------------ //
void loop() {

  while (isModeUpdated) {
    isModeUpdated=false;
    statusLED2->blink(100);
    
    // Skip finish operation when going to emergency
    if(mode != MODE_EMERGENCY){ 
      strategyMethods[2][prevMode](); // finish any operations for prevMode here
    }
    strategyMethods[0][mode](); // init new strategy according to the new mode value
  }

  strategyMethods[1][mode]();
}

// ------------------------------------------------------------ //
//                          FUNCTIONS                           //
// ------------------------------------------------------------ //

// Sets pinmode of all pins and writes initial values for outputs
void InitAllPins(){
  
  // Power control (Relays)
  pinMode(pwrMotor,   OUTPUT);
  pinMode(pwr12VMain, OUTPUT);
  pinMode(pwr5VMain,  OUTPUT);
  pinMode(pwrRF,      OUTPUT);
  pinMode(pwrIridium, OUTPUT);

  digitalWrite(pwrMotor,  LOW);
  digitalWrite(pwr12VMain,LOW);
  digitalWrite(pwr5VMain, LOW);
  digitalWrite(pwrRF,     LOW);
  digitalWrite(pwrIridium,LOW);

  // Analog Sensors
  pinMode(sensorWind, INPUT);

  pinMode(sensorTemp1, INPUT);
  pinMode(sensorTemp2, INPUT);
  pinMode(sensorTemp3, INPUT);

  pinMode(sensorRelH1, INPUT);
  pinMode(sensorRelH2, INPUT);
  pinMode(sensorRelH3, INPUT);

  // External Inputs
  pinMode(emergencyStopBtn, INPUT_PULLUP)
  pinMode(inputBtn1, INPUT_PULLUP);
  pinMode(inputBtn2, INPUT_PULLUP);
}


// Set pointers for strategies methods
void InitStrategyMethods() {
  
  strategyMethods[0][MODE_EMERGENCY] = StartStrategyEmergency;
  strategyMethods[1][MODE_EMERGENCY] = RunStrategyEmergency;
  strategyMethods[2][MODE_EMERGENCY] = FinishStrategyEmergency;

  strategyMethods[0][MODE_SYSTEMTEST] = StartStrategySystemTest;
  strategyMethods[1][MODE_SYSTEMTEST] = RunStrategySystemTest;
  strategyMethods[2][MODE_SYSTEMTEST] = FinishStrategySystemTest;
  
  strategyMethods[0][MODE_BLINKER] = StartStrategyBlinker;
  strategyMethods[1][MODE_BLINKER] = RunStrategyBlinker;
  strategyMethods[2][MODE_BLINKER] = FinishStrategyBlinker;
  
  strategyMethods[0][MODE_TEST] = StartStrategyTest;
  strategyMethods[1][MODE_TEST] = RunStrategyTest;
  strategyMethods[2][MODE_TEST] = FinishStrategyTest;
  
  strategyMethods[0][MODE_LIGHTSEEKER] = StartStrategyLightSeeker;
  strategyMethods[1][MODE_LIGHTSEEKER] = RunStrategyLightSeeker;
  strategyMethods[2][MODE_LIGHTSEEKER] = FinishStrategyLightSeeker;
  
  strategyMethods[0][MODE_REMOTECONTROL] = StartStrategyRemote;
  strategyMethods[1][MODE_REMOTECONTROL] = RunStrategyRemote;
  strategyMethods[2][MODE_REMOTECONTROL] = FinishStrategyRemote;
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

// Set last mode from EEPROM
void InitMode() {
  mode = EEPROM.read(MEMADDR_LASTMODE);
  prevMode = 0;
  isModeUpdated = true;
}

// Set LED
void InitStatusLeds() {
  statusLED1 = new LED(PP_LED_STATUS_1);
  statusLED2 = new LED(PP_LED_STATUS_2);
  statusLED1->off();
  statusLED2->off();
}

// Reset MCU
void(* resetFunc) (void) = 0;


// tries set the mode and isModeUpdated flag
boolean SetMode(byte newMode) {
  if (newMode < MODES_MAX) {
    prevMode = mode;
    mode = newMode;
    isModeUpdated = true;
    EEPROM.write(MEMADDR_LASTMODE, mode);
    DEBUG_PRINT("Mode is set to ");
    DEBUG_PRINTLN(mode);
    return true;
  }
  return false;
}
