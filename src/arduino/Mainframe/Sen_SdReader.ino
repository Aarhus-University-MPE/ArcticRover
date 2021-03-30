// Initialize SD card reader module.
void InitSDReader(){
  DEBUG_PRINT("Initializing SD card...");

  if (!SD.begin(SD_CS)) DEBUG_PRINTLN("initialization failed!")
  else DEBUG_PRINTLN("initialization done.");  
}

//