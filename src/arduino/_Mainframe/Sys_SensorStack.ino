/*
  GeoRover Sensor Stack

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021
*/

void InitializeSensorStack() {
  DEBUG_PRINT("Initializing Sensor stack... ");
  DEBUG_PRINT("Initializing Communication... ");
  Wire.begin();
  DEBUG_PRINTLN("Communication Active.");
}

// Checks GNSS signal availability
bool GPSValid() {
  bool valid = true;

  // Signal valid?

  return valid;
}

// Checks temperature sensor availability
bool TempSensorCheck() {
  bool valid = true;

  // Currently measuring?

  return valid;
}

// Checks accelerometer sensor availability and values within limits
bool AccelSensorCheck() {
  bool valid = true;

  // Currently measurements valid?

  return valid;
}
