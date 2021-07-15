/*
  GeoRover Navigation Stack

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021
*/

/*
  Algorithm:

  1. Check coordinates valid
    1.1 Format correct?
    1.2 Distance < min distance?
        -> Valid gps fix? (time since last)
    else break
  2. Calculate current bearing
    2.1 Distance between previous waypoints > Min Distance?
        -> Calculate using Forward Azimuth
    2.2 Else skip (Set bearing: 0)
  3. Calculate target bearing (Difference between current position and target waypoint)
    3.1 Is bearing valid?
        -> Calculate using forward Azimuth
    3.2 Else skip (Set target bearing: 0)
  4. Calculate change bearing
    4.1 difference between current and target

*/

#include "Math.h"
#include "GeneralFunctions.h"

// Target coordinate (read from EEPROM)
double lat_target   = 0;
double lon_target   = 0;

// Current coordinate (From GNSS)
double lat_current  = 0;
double lon_current  = 0;

// Previous coordinate (From GNSS, updated when distance between last point > threshold)
double lat_previous = 0;
double lon_previous = 0;

// Validity flags
bool validCoordinate = false;
bool gnssFixValid = false;

bool navigationPreCheck = false;
bool navigationFlag = false;

bool Navigate() {
  if (navigationPreCheck) {
    if (NavigationCheck()) {
      // Calculate distance from current pos to target pos

      // Check distance -> increment if < min accept

      // Calculate bearing to target

      // -> Run motor
    }
  }
}

/*
  Check system status while navigation is running.
  GNSS status, Accelerometer output (tilt?), Battery Status
*/
bool NavigationCheck() {
  DEBUG_PRINT("Running Navigation Checks... ");
  // GNSS valid?

  // Battery status?

  // Accelerometer within limits?

  // Motor heartbeat
}

/*
  Checks to run before starting autonomous navigation
  System checks, Route Checks, Sensor Checks
*/
bool NavigationPreChecks() {
  DEBUG_PRINT("Running Navigation Prechecks...");
  bool precheck = true;
  bool precheckCode[8];

  // System Checks
  precheckCode[0] = VoltageCheck();   // Required
  precheckCode[1] = MotorCheck();     // Required
  precheckCode[2] = GPSValid();       // Required

  // Route Checks
  precheckCode[3] = RouteFileCheck();     // Required
  precheckCode[4] = RouteFormatCheck();   // Required
  precheckCode[5] = RouteRangeCheck();    // Required

  // Sensor Checks
  precheckCode[6] = TempSensorCheck();    // Optional
  precheckCode[7] = AccelSensorCheck();   // Optional

  byte precheckCodeByte = boolarr_to_byte(precheckCode);


  // Compare precheck with required
  precheck = ((precheckCodeByte | BINARY_CODE_PRECHECK_OPTIONAL) == BINARY_CODE_PRECHECK_FULL);

  if (precheck) DEBUG_PRINTLN("System ready, Precheck Code: " + precheckCodeByte);
  else DEBUG_PRINTLN("System Error, Precheck Code: " + precheckCodeByte);

  return precheck;
}

// Checks if route is present (and loadable)
bool RouteFileCheck() {
  bool validity = true;
  // Load route

  return validity;
}

// Check if route is valid (format)
bool RouteFormatCheck() {
  bool validity = true;
  // Check route Validity
  // Check formats

  return validity;
}

// Check if route is valid (distance to waypoints < max)
bool RouteRangeCheck() {
  bool validity = true;
  // Check route Validity
  // Calculate distances from current position to start point and between points.

  return validity;
}

// Check validity of coordinate in the form of format and range of coordinate
bool CoordinateValiditiy() {
  DEBUG_PRINTLN("Checking validity of coordinates ... ");
  bool validity = true;
  if (!CoordinateValidityFormat(lat_target, lon_target)) {
    DEBUG_PRINT("Coordinate format NOT valid!");
  }
  return (CoordinateValidityFormat(lat_target, lon_target) && CoordinateValidityRange());
}

// Checks validity of coordinate format (-90 <= lat <= 90 && -180 <= lon <= 180)
bool CoordinateValidityFormat(double lat, double lon) {
  DEBUG_PRINT("Checking format of coordinates... lat: " + String(lat, DEC) + ", lon: " + String(lon, DEC) + " ... ");
  bool validity = true;
  if (lat == 0 || lon == 0) {
    validity = false;
    DEBUG_PRINT("coordinates must be non zero ... ");
  }

  if (lat < -90 || lat > 90) {
    validity = false;
    DEBUG_PRINT("latitude range must be (-90 < lat < 90) ... ");
  }

  if (lon < -180 || lon > 180) {
    validity = false;
    DEBUG_PRINT("longitude range must be (-180 < lon < 180) ... ");
  }

  if (validity) DEBUG_PRINTLN("Coordinates format valid!");
  else DEBUG_PRINTLN("Coordinates format NOT valid!");

  return validity;
}

// Checks if distance to coordinate is below maximum range (MAX_DISTANCE_VALID_WAYPOINT)
bool CoordinateValidityRange() {
  DEBUG_PRINT("Checking range between current and target coordinates... lat_current: " + String(lat_current, DEC) + ", lon_current: " + String(lon_current, DEC) + " ... " + "lat_target: " + String(lat_target, DEC) + ", lon_target: " + String(lon_target, DEC) + " ... ");
  bool validity = (CoordinateDistance(lat_current, lon_current, lat_target, lon_target) <= MAX_DISTANCE_VALID_WAYPOINT);

  if (validity) DEBUG_PRINTLN("Coordinates within maximum range!");
  else DEBUG_PRINTLN("Range NOT within maximum range!");
  return validity;
}
