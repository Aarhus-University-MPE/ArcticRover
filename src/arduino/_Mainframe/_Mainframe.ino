/*
GeoRover mainframe - Manages primary systems as the Rover backbone.

Mads Rosenhøj Jepepsen
Aarhus University 
2021
*/

#include "_constants.h"
#include "_shared.h"
#include "_pinout.h"

typedef void (*functionPtr)();

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
  DBG_ONLY(while(!Serial));
  DEBUG_PRINTLN("Debug mode. Entered setup...");
  
  // System initialization
  InitAllPins();
  InitButtons();
  InitStatusLed();

  // Strategy initialization
  InitMode();
  InitStrategyMethods();

  // Setup finished
  LedBlinkDoubleShort(BINARY_CODE_LED_GRN);
  DEBUG_PRINTLN("Setup complete.");
}

// ------------------------------------------------------------ //
//                          MAIN LOOP                           //
// ------------------------------------------------------------ //
void loop() {

  while (isModeUpdated) {
    isModeUpdated=false;
    LedBlinkDoubleShort(BINARY_CODE_LED_GRN);
    
    // Skip finish operation when going to emergency
    if(mode != MODE_EMERGENCY){ 
      strategyMethods[2][prevMode](); // finish any operations for prevMode here
    }
    strategyMethods[0][mode](); // init new strategy according to the new mode value
  }

  strategyMethods[1][mode]();
}

// ------------------------------------------------------------ //
//                       SYSTEM FUNCTIONS                       //
// ------------------------------------------------------------ //

// Sets pinmode of all pins and writes initial values for outputs
void InitAllPins(){
  
  // External Inputs
  pinMode(PI_BUTTON_ESTOP,  INPUT_PULLUP);
  pinMode(PI_BUTTON_MODE,   INPUT_PULLUP);
  pinMode(PI_BUTTON_SELECT, INPUT_PULLUP);

  // Status LED/Sound
  pinMode(PO_LED_STATUS_GRN,   OUTPUT);
  pinMode(PO_LED_STATUS_YEL,   OUTPUT);
  pinMode(PO_LED_STATUS_RED,   OUTPUT);
  
  digitalWrite(PO_LED_STATUS_RED, LOW);
  digitalWrite(PO_LED_STATUS_YEL, LOW);
  digitalWrite(PO_LED_STATUS_GRN, LOW);

  // Power control (Relays)
  pinMode(PO_POWER_MOTOR,   OUTPUT);
  pinMode(PO_POWER_12V,     OUTPUT);
  pinMode(PO_POWER_5V,      OUTPUT);
  pinMode(PO_POWER_RF,      OUTPUT);
  pinMode(PO_POWER_IRIDIUM, OUTPUT);

  digitalWrite(PO_POWER_MOTOR,    LOW);
  digitalWrite(PO_POWER_12V,      LOW);
  digitalWrite(PO_POWER_5V,       LOW);
  digitalWrite(PO_POWER_RF,       LOW);
  digitalWrite(PO_POWER_IRIDIUM,  LOW);

  // Analog Sensors
  pinMode(PA_SENSOR_WIND,   INPUT);

  pinMode(PA_SENSOR_TEMP1, INPUT);
  pinMode(PA_SENSOR_TEMP2, INPUT);
  pinMode(PA_SENSOR_TEMP3, INPUT);

  pinMode(PA_SENSOR_RELH1, INPUT);
  pinMode(PA_SENSOR_RELH2, INPUT);
  pinMode(PA_SENSOR_RELH3, INPUT);
}

// Reset MCU
void(* resetFunc) (void) = 0;
