// Initialize SD card reader module.
void InitSDReader(){
  DEBUG_PRINT("Initializing SD card...");

  if (!SD.begin(PO_SPISS_SDCARD)) DEBUG_PRINTLN("initialization failed!");
  else DEBUG_PRINTLN("initialization done.");  
}

//