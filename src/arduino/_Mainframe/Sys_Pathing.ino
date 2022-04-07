/*
  GeoRover pathing protocols

  Mads Rosenhøj Jeppesen
  Aarhus University
  2021
*/

long latRoute, lonRoute;  // Latest position buffer (degrees *10^-7)
int idxRoute;             //
int lengthRoute;
int indexRoute;

bool routeStatus  = false;
bool routeTestRun = false;

char operatorName[numChars];

// Target coordinate (read from EEPROM)
long latTarget      = 0;
long lonTarget      = 0;
float navigationDir = 0;

// Current coordinate (From GNSS)
long headingError = 0;  // deg * 10^-5

// returns true if waypoint file exists
bool SDRoute() {
  return SD.exists("Waypoints.csv");
}

// Clears current route data
void ClearRoute() {
  latRoute = 0;
  lonRoute = 0;

  memset(operatorName, '\0', sizeof(operatorName));
  lengthRoute = 0;
  indexRoute  = 0;
}

// Loads Waypoint file data and first waypoint into buffer
bool LoadRoute() {
  DEBUG_PRINTLN(F("Loading Waypoint file"));

  ClearRoute();

  // Load first waypoint into buffer
  if (!RouteSD(latRoute, lonRoute, 0)) {
    return false;
  }

  DEBUG_PRINTLN(F("Done Loading"));
  return true;
}

// Test full route, only run once (used in Navigation pre-check). To perform new test, run ResetRouteStatus()
bool RouteTest() {
  if (routeTestRun) routeStatus;

  routeTestRun = true;  // Avoid several route tests being run

  DEBUG_PRINTLN(F("Running full route test"));

  routeStatus = false;
  if (!LoadRoute()) {
    return routeStatus;
  }
  if (!RouteSDValid()) {
    return routeStatus;
  }
  if (!CompareEepromSdRoute()) {
    if (!FlashRouteEeprom()) {
      return routeStatus;
    }
    if (!CompareEepromSdRoute()) {
      return routeStatus;
    }
  }

  routeStatus = true;

  return routeStatus;
}

// Compares current EEPROM route data to SD data
// TODO: Check full route, currently only checks route index, route length and 1st waypoint
bool CompareEepromSdRoute() {
  bool validity = true;

  int routeIndexEeprom  = EEPROM_READ_INT(MEMADDR_ROUTEIDX_START);
  int routeLengthEeprom = EEPROM_READ_INT(MEMADDR_ROUTELEN_START);

  DEBUG_PRINTLINE();
  DEBUG_PRINT(F("EEPROM route Index: "));
  DEBUG_PRINT(routeIndexEeprom);
  DEBUG_PRINT(F("\t EEPROM route Length: "));

  DEBUG_PRINT(F("Waypoint.csv route Index: "));
  DEBUG_PRINTLN(indexRoute);
  DEBUG_PRINT(F("\t Waypoint.csv route Length: "));
  DEBUG_PRINTLN(lengthRoute);
  DEBUG_PRINTLINE();

  if (indexRoute != routeIndexEeprom) return false;
  if (lengthRoute != routeLengthEeprom) return false;

  long latEE, lonEE;

  if (!RouteEe(latEE, lonEE, 0)) return false;
  RouteEe(latEE, lonEE, 0);

  DEBUG_PRINT(routeLengthEeprom);
  DEBUG_PRINT(F("\t - EEPROM 1st waypoint: Lat: "));
  DEBUG_PRINT(latEE);
  DEBUG_PRINT(F("\t lon: "));
  DEBUG_PRINTLN(lonEE);

  DEBUG_PRINT(F("\t - Waypoint.csv 1st waypoint: Lat: "));
  DEBUG_PRINT(latRoute);
  DEBUG_PRINT(F("\t lon: "));
  DEBUG_PRINTLN(lonRoute);

  if ((latRoute != latEE) && (lonRoute != lonEE)) return false;

  return validity;
}

bool RouteSDValid(){
  if (!SDRoute) {
    return false;
  }

  // DEBUG_PRINTLN(F("Validating Waypoint file"));

  File waypoints = SD.open("Waypoints.csv", FILE_READ);

  if (!waypoints) {
    // DEBUG_PRINTLN(F("Error opening waypoints file"));
    return false;
  }

  long latRouteOld, lonRouteOld;

  // Read waypoint file
  for (int i = 0; i < lengthRoute + 3; i++) {
    ParseWaypoint(waypoints.readStringUntil('\n'), i);
    // Skip first 3 (header, data, header)
    if (i > 2) {
      if (!CoordinateValidityFormat(latRoute, lonRoute)) {
        waypoints.close();
        return false;
      }
    }
    // Range checks from 2nd waypoint
    if (i > 3) {
      if (!CoordinateValidityRange(latRouteOld, lonRouteOld, latRoute, lonRoute)) {
        waypoints.close();
        return false;
      }
    }
    latRouteOld = latRoute;
    lonRouteOld = lonRoute;
  }

  return true;
}
// Check if route is valid (SD and EEPROM)
bool RouteCheck() {
  return RouteSDValid() && CompareEepromSdRoute();
}

bool PathingProcess() {
  GnssUpdate();

  // Calculate distance from current pos to target pos
  WaypointUpdate();

  // Calculate bearing to target
  BearingUpdate();
}

// Updates waypoint based on distance to target
void WaypointUpdate() {
  if (WaypointWithinRange) {
    IncrementWaypoint();
  }
}

// Updates bearing offset based on target and current bearing
void BearingUpdate() {
  long heading       = GnssGetHeading();  // deg * 10^-5
  long targetHeading = (long)(CourseToLong(latCurrent, lonCurrent, latTarget, lonTarget)) * 100000;

  headingError = targetHeading - heading;

  // Check headingError > 180 degree to take smallest rotation (190 degrees to avoid rotation overlap)
  if(headingError > 190){
    headingError -= 360;
  } else if (headingError < -190){
    headingError += 360;
  }

  if (abs(headingError) < MAX_VALID_BEARING) {
    navigationDir = 0;
  } else if (headingError > MAX_VALID_BEARING) {
    navigationDir = MAX_AUTONOMOUS_TURN;
  } else {
    navigationDir = -MAX_AUTONOMOUS_TURN;
  }

}

// Returns latest navigation direction
float BearingDirection() {
  return navigationDir;
}

// Checks if distance from current position to target is < min
bool WaypointWithinRange() {
  long distance = DistanceBetweenLong(latCurrent, lonCurrent, latTarget, lonTarget);
  return distance < MAX_DISTANCE_WAYPOINT_ACCEPT;
}

// Increments current waypoint returns to 0 if > total number of points
void IncrementWaypoint() {
  if (waypointIndex == lengthRoute) {
    waypointIndex = 0;
  } else {
    waypointIndex++;
  }

  UpdateWaypoint();
}

// Reads EEPROM waypoint lat, lon from current waypointIndex
void UpdateWaypoint() {
  EEPROMReadLatLon(waypointIndex);
  EEPROM_WRITE_INDEX(waypointIndex);
}

// Reads latitude and longitude at given index
bool RouteSD(long &lat, long &lon, int index) {
  if (!SDRoute) {
    return false;
  }

  File waypoints = SD.open("Waypoints.csv", FILE_READ);

  if (!waypoints) {
    DEBUG_PRINTLN(F("Error opening waypoints file"));
    return false;
  }

  // Read waypoint file until index
  int idx;
  while (waypoints.available() && idx < index + 3) {
    waypoints.readStringUntil('\n');
    idx++;
  }

  if (idx != index + 3) {
    DEBUG_PRINTLN(F("Waypoint file doesn't contain index"));
    return false;
  }

  if (!PopulateLatLon(waypoints.readStringUntil('\n'), lat, lon)) {
    waypoints.close();
    return false;
  }

  waypoints.close();
  return true;
}

// Reads EEPROM Latitude and Longitude values at given index, returns false if non found
bool RouteEe(long &lat, long &lon, int index) {
  EEPROM_READ_LAT(index, lat);
  EEPROM_READ_LON(index, lon);

  if (lat > MAX_LAT_VALUE || lat < -MAX_LAT_VALUE || lat == 0) {
    // DEBUG_PRINTLN(F("EEPROM lat value outside allowed values or 0"));
    return false;
  }
  if (lon > MAX_LON_VALUE || lon < -MAX_LON_VALUE || lon == 0) {
    // DEBUG_PRINTLN(F("EEPROM lon value outside allowed values or 0"));
    return false;
  }

  return true;
}

// Flash waypoints to EEPROM
bool FlashRouteEeprom() {
  if (!SDRoute) {
    return false;
  }

  DEBUG_PRINTLN(F("Flashing Waypoint file to EEPROM"));

  File waypoints = SD.open("Waypoints.csv", FILE_READ);

  if (!waypoints) {
    DEBUG_PRINTLN(F("Error opening waypoints file"));
    return false;
  }

  // Read waypoint file
  for (int i = 0; i < lengthRoute + 3; i++) {
    ParseWaypoint(waypoints.readStringUntil('\n'), i);
    // Skip first 3 (header, data, header)
    if (i > 2) {
      EEPROMWriteLatLon(i - 3);
    }
  }

  return true;
}

/* Parses waypoint string separating into latitude and longitude values
    waypointIdx,Latitude,Longitude
*/
bool PopulateLatLon(String waypoint, long &lat, long &lon) {
  char waypointBuffer[waypoint.length() + 1];
  waypoint.toCharArray(waypointBuffer, waypoint.length() + 1);

  char *strtokIndx;  // strtok() index

  // Route Index
  strtokIndx = strtok(waypointBuffer, ",");
  idxRoute   = atoi(strtokIndx);

  // Latitude
  strtokIndx = strtok(NULL, ",");
  latRoute   = atol(strtokIndx);
  lat        = latRoute;

  // Longitude
  strtokIndx = strtok(NULL, ",");
  lonRoute   = atol(strtokIndx);
  lon        = lonRoute;

  DEBUG_PRINT(F("Latitude: "));
  DEBUG_PRINT(lat);
  DEBUG_PRINT(F("\t Longitude: "));
  DEBUG_PRINTLN(lon);

  if (!CoordinateValidityFormat(lat, lon)) {
    return false;
  }

  return true;
}

bool PopulateLatLon(String waypoint) {
  PopulateLatLon(waypoint, latRoute, lonRoute);
}

void PopulateWaypointData(String waypoint) {
  char waypointBuffer[waypoint.length() + 1];
  waypoint.toCharArray(waypointBuffer, waypoint.length() + 1);

  char *strtokIndx;  // strtok() index

  // Route Index
  strtokIndx = strtok(waypointBuffer, ",");
  indexRoute = atoi(strtokIndx);

  // Waypoint Length
  strtokIndx  = strtok(NULL, ",");
  lengthRoute = atoi(strtokIndx);

  // Operator
  strtokIndx = strtok(NULL, ",");
  strcpy(operatorName, strtokIndx);

  DEBUG_PRINT(F("Route Index: "));
  DEBUG_PRINT(indexRoute);
  DEBUG_PRINT(F("\t Route Length: "));
  DEBUG_PRINT(lengthRoute);
  DEBUG_PRINT(F("\t Operator: "));
  DEBUG_PRINTLN(operatorName);
}

// Parse waypoint string
void ParseWaypoint(String waypoint, int index) {
  switch (index) {
    case 0:
      // Header (Waypoint Index,Waypoint Length,Operator)
      break;
    case 1:
      PopulateWaypointData(waypoint);
      break;
    case 2:
      // Header (Index,Lat,Lon)
      break;
    default:
      PopulateLatLon(waypoint);
      break;
  }
}

// Reset route status flag
void ResetRouteStatus() {
  routeStatus  = false;
  routeTestRun = false;
}

// Populate buffer latitude longitude into EEPROM
void EEPROMWriteLatLon(int index) {
  EEPROM_WRITE_LAT(index, latRoute);
  EEPROM_WRITE_LON(index, lonRoute);
}

// Populate buffer latitude longitude from EEPROM
void EEPROMReadLatLon(int index) {
  EEPROM_READ_LAT(index, latRoute);
  EEPROM_READ_LON(index, lonRoute);
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

  if (lon < -MAX_LON_VALUE || lon > MAX_LON_VALUE) {
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

// Checks distance between two coordinate sets and returns true if within MAX_DISTANCE_VALID_WAYPOINT
bool CoordinateValidityRange(long lat1, long lon1, long lat2, long lon2) {
  DEBUG_PRINT(F("Checking range between"));
  DEBUG_PRINT(F(" lat: "));
  DEBUG_PRINT(lat1);
  DEBUG_PRINT(F(" lon: "));
  DEBUG_PRINT(lon1);
  DEBUG_PRINT(F("\t and \t"));
  DEBUG_PRINT(F(" lat: "));
  DEBUG_PRINT(lat2);
  DEBUG_PRINT(F(" lon: "));
  DEBUG_PRINTLN(lon2);

  long distance = DistanceBetweenLong(lat1, lon1, lat2, lon2);

  DEBUG_PRINT(F("Distance: "));
  DEBUG_PRINTLN(distance);

  return distance <= MAX_DISTANCE_VALID_WAYPOINT;
}
