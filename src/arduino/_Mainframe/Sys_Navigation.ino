
/*
  GeoRover Navigation Stack

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021
*/

/* Algorithm:
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

#include "GeneralFunctions.h"
#include "Math.h"

// Target coordinate (read from EEPROM)
long lat_target = 0;
long lon_target = 0;

// Current coordinate (From GNSS)
long lat_current = 0;
long lon_current = 0;

// Previous coordinate (From GNSS, updated when distance between last point > threshold)
long lat_previous = 0;
long lon_previous = 0;

// Validity flags
bool validCoordinate = false;
bool gnssFixValid    = false;

bool navigationFlag = false;

bool Navigate() {
  if (!NavigationPreCheck()) {
    return false;
  }
  if (!NavigationRunCheck()) {
    return false;
  }

  // Calculate distance from current pos to target pos

  // Check distance -> increment if < min accept

  // Calculate bearing to target

  // -> Run motor
}

//  Checks to run before starting autonomous navigation
//  System checks, Route Checks
bool NavigationPreCheck() {
  static bool navigationPreCheck = false;

  if (!NavigationRunCheck()) {
    navigationPreCheck = false;
    return navigationPreCheck;
  }

  if (!navigationPreCheck) {
    navigationPreCheck = RouteTest();
  }

  return navigationPreCheck;
}

// Check system status while navigation is running.
// GNSS status, Accelerometer output (tilt?), Battery Status
bool NavigationRunCheck() {
  return SystemCheck(MODE_AUTONOMOUS);
}

bool RouteCheck() {
  return NavigationPreCheck();
}

// Test full route (used in Navigation pre-check)
bool RouteTest() {
  DEBUG_PRINTLN(F("Running full route test"));
  if (!RouteFileCheck()) return false;
  if (!RouteFormatCheck()) return false;
  if (!RouteRangeCheck()) return false;
  return true;
}

// Checks if route is present (and loadable)
// Load route from SD card and compare with EEPROM route
bool RouteFileCheck() {
  bool validity = true;

  int routeLen = EEPROM_READ_INT(MEMADDR_ROUTELEN_START);

  if (routeLen <= 0) {
     // LoadRoute(); // Load route from SD card
  }

  if(SDRoute()){
    
  }


  for (int i = 0; i < routeLen; i++) {
    // load value
  }

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
  DEBUG_PRINTLN(F("Checking validity of coordinates ... "));
  bool validity = true;
  if (!CoordinateValidityFormat(lat_target, lon_target)) {
    DEBUG_PRINT(F("Coordinate format NOT valid!"));
  }
  return (CoordinateValidityFormat(lat_target, lon_target) && CoordinateValidityRange());
}

// Checks validity of coordinate format (-90 <= lat <= 90 && -180 <= lon <= 180)
bool CoordinateValidityFormat(long lat, long lon) {
  DEBUG_PRINT("Checking format of coordinates... lat: " + String(lat, DEC) + ", lon: " + String(lon, DEC) + " ... ");
  bool validity = true;
  if (lat == 0 || lon == 0) {
    validity = false;
    DEBUG_PRINT(F("coordinates must be non zero ... "));
  }

  if (lat < -MAX_LAT_VALUE || lat > MAX_LAT_VALUE) {
    validity = false;
    DEBUG_PRINT(F("latitude range must be (-90 < lat < 90) ... "));
  }

  if (lon < -MAX_LONG_VALUE || lon > MAX_LONG_VALUE) {
    validity = false;
    DEBUG_PRINT(F("longitude range must be (-180 < lon < 180) ... "));
  }

  if (validity) {
    DEBUG_PRINTLN(F("Coordinates format valid!"));
  } else {
    DEBUG_PRINTLN(F("Coordinates format NOT valid!"));
  }

  return validity;
}

// Checks if distance to coordinate is below maximum range (MAX_DISTANCE_VALID_WAYPOINT)
bool CoordinateValidityRange() {
  DEBUG_PRINT("Checking range between current and target coordinates... lat_current: " + String(lat_current, DEC) + ", lon_current: " + String(lon_current, DEC) + " ... " + "lat_target: " + String(lat_target, DEC) + ", lon_target: " + String(lon_target, DEC) + " ... ");
  bool validity = (CoordinateDistance(lat_current, lon_current, lat_target, lon_target) <= MAX_DISTANCE_VALID_WAYPOINT);

  if (validity) {
    DEBUG_PRINTLN(F("Coordinates within maximum range!"));
  } else {
    DEBUG_PRINTLN(F("Range NOT within maximum range!"));
  }
  return validity;
}
