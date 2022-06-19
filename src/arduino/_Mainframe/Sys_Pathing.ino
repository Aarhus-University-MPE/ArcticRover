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
unsigned int distanceToWaypoint = 0;

bool routeStatus  = false;
bool routeTestRun = false;

char operatorName[numChars];

long heading;
long targetHeading;

// Target coordinate (read from EEPROM)
float navigationDir = 0;

// Current coordinate (From GNSS)
long headingError = 0;  // deg * 10^-5

// returns true if waypoint file exists
bool SDRoute() {
  return SD.exists("WAYPOINT.csv");
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
  // DEBUG_PRINTLN(F("Loading Waypoint file"));

  ClearRoute();

  // Load first waypoint into buffer
  if (!RouteSD(latRoute, lonRoute, 0)) {
    return false;
  }

  // DEBUG_PRINTLN(F("Done Loading"));
  return true;
}

// Test full route, only run once (used in Navigation pre-check). To perform new test, run ResetRouteStatus()
bool RouteTest() {
  if (routeTestRun) routeStatus;

  routeTestRun = true;  // Avoid several route tests being run

  // DEBUG_PRINTLN(F("Running full route test"));

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

  // DEBUG_PRINTLINE();
  // DEBUG_PRINT(F("EEPROM route Index: "));
  // DEBUG_PRINT(routeIndexEeprom);
  // DEBUG_PRINT(F("\t EEPROM route Length: "));
  // DEBUG_PRINTLN(routeLengthEeprom);

  // DEBUG_PRINT(F("Waypoint.csv route Index: "));
  // DEBUG_PRINT(indexRoute);
  // DEBUG_PRINT(F("\t Waypoint.csv route Length: "));
  // DEBUG_PRINTLN(lengthRoute);
  // DEBUG_PRINTLINE();

  if (indexRoute != routeIndexEeprom) return false;
  if (lengthRoute != routeLengthEeprom) return false;

  long latEE, lonEE;

  // if (!RouteEe(latEE, lonEE, 0)) return false;
  RouteEe(latEE, lonEE, lengthRoute - 1);

  // DEBUG_PRINT(F("EEPROM Last waypoint: Lat: "));
  // DEBUG_PRINT(latEE);
  // DEBUG_PRINT(F("\t lon: "));
  // DEBUG_PRINTLN(lonEE);

  // DEBUG_PRINT(F("Waypoint.csv Last waypoint: Lat: "));
  // DEBUG_PRINT(latRoute);
  // DEBUG_PRINT(F("\t lon: "));
  // DEBUG_PRINTLN(lonRoute);

  if ((latRoute != latEE) && (lonRoute != lonEE)) return false;

  return validity;
}

bool RouteSDValid() {
  if (!SDRoute) {
    return false;
  }

  // DEBUG_PRINTLN(F("Validating Waypoint file"));

  File waypoints = SD.open("WAYPOINT.csv", FILE_READ);

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

// Update current position and heading and check distance to current waypoint, if below threshold, set next target
bool PathingProcess() {
  // Get latest position from GNSS
  GnssUpdate();

  // Calculate distance from current pos to target pos
  WaypointUpdate();

  // Calculate heading to target
  HeadingUpdate();
}

// Updates waypoint based on distance to target
void WaypointUpdate() {
  if (HomePosOverride()) return;

  if (WaypointWithinRange()) IncrementWaypoint();
}

// Home position override, navigate towards home
// TODO: halt once at home position
bool HomePosOverride() {
  if (!navigationOverride) return false;

  if (alternativeHome) {
    latRoute = COORDINATE_HOME_LAT_ALT;
    lonRoute = COORDINATE_HOME_LON_ALT;
  } else {
    latRoute = COORDINATE_HOME_LAT;
    lonRoute = COORDINATE_HOME_LON;
  }

  return navigationOverride;
}

// Set override flags, head home
void SetHomePosOverride(bool overrideRoute, bool alternative) {
  navigationOverride = overrideRoute;
  alternativeHome    = alternative;

  if (!navigationOverride) return;

  if (alternativeHome) {
    latRoute = COORDINATE_HOME_LAT_ALT;
    lonRoute = COORDINATE_HOME_LON_ALT;
  } else {
    latRoute = COORDINATE_HOME_LAT;
    lonRoute = COORDINATE_HOME_LON;
  }
}

// Updates heading offset based on target and current heading
void HeadingUpdate() {
  heading       = GnssGetHeading();  // deg * 10^-5
  targetHeading = (long)(CourseToLong(latCurrent, lonCurrent, latRoute, lonRoute)) * 100000;

  headingError = targetHeading - heading;

  // Check headingError > 180 degree to take smallest rotation (190 degrees to avoid rotation overlap)
  if (headingError > 19000000) {
    headingError -= 36000000;
  } else if (headingError < -19000000) {
    headingError += 36000000;
  }
  float turnStrength = headingError / 100000.0f * 1.0f / 45.0f;  // TODO: Rewrite to constant (AUTONOMY_TURN_STRENGTH)

  if (abs(headingError) < MAX_VALID_BEARING) {
    navigationDir = 0;
  } else if (headingError > MAX_VALID_BEARING) {
    // navigationDir = MAX_AUTONOMOUS_TURN;
    navigationDir = min(MAX_AUTONOMOUS_TURN, turnStrength);
  } else {
    // navigationDir = -MAX_AUTONOMOUS_TURN;
    navigationDir = max(-MAX_AUTONOMOUS_TURN, turnStrength);
  }
}

// Returns latest navigation direction
float HeadingDirection() {
  return navigationDir;
}

// Checks if distance from current position to target is < min
bool WaypointWithinRange() {
  // write coordinate
  // DEBUG_PRINT(F("Current - Lat: "));
  // DEBUG_PRINT(latCurrent);
  // DEBUG_PRINT(F(" Lon: "));
  // DEBUG_PRINTLN(lonCurrent);

  distanceToWaypoint = DistanceBetweenLong(latCurrent, lonCurrent, latRoute, lonRoute);
  return distanceToWaypoint < MAX_DISTANCE_WAYPOINT_ACCEPT;
}

// Increments current waypoint returns to 0 if > total number of points
// TODO: halt after last waypoint? Currently repeats last steps
void IncrementWaypoint() {
  if (waypointIndex >= lengthRoute - 1) {
    waypointIndex = 0;
  } else {
    waypointIndex++;
  }

  UpdateWaypoint();
}

// Reads EEPROM waypoint lat, lon from current waypointIndex
void UpdateWaypoint() {
  DEBUG_PRINT("Current Waypoint: ");
  DEBUG_PRINTLN(waypointIndex);
  EEPROMReadLatLon(waypointIndex);
  EEPROM_WRITE_INDEX(waypointIndex);
}

unsigned long lastMillisNavPrint = 0;
void NavigationPrint() {
  if (millis() - lastMillisNavPrint < CANBUS_PRINT_PERIOD * 5) {
    return;
  }
  lastMillisNavPrint = millis();

  DEBUG_PRINTLINE();
  // DEBUG_PRINT(F("Target - Lat: "));
  // DEBUG_PRINT(latRoute);
  // DEBUG_PRINT(F(" Lon: "));
  // DEBUG_PRINTLN(lonRoute);

  // DEBUG_PRINT(F("Current - Lat: "));
  // DEBUG_PRINT(latCurrent);
  // DEBUG_PRINT(F(" Lon: "));
  // DEBUG_PRINTLN(lonCurrent);
  DEBUG_PRINT("Distance to waypoint: ");
  DEBUG_PRINTLN(distanceToWaypoint);

  // DEBUG_PRINT(F("Target Heading: "));
  // DEBUG_PRINT(targetHeading);
  // DEBUG_PRINT(F("\tCurrent Heading: "));
  // DEBUG_PRINT(heading);
  // DEBUG_PRINT(F("\tHeading Error: "));
  // DEBUG_PRINT(headingError);
  // if (navigationDir == 0) {
  //   DEBUG_PRINTLN(F("\tStraight Forward"));
  // } else if (navigationDir < 0) {
  //   DEBUG_PRINTLN(F("\tTurn LEFT"));
  // } else {
  //   DEBUG_PRINTLN(F("\tTurn RIGHT"));
  // }
  // DEBUG_PRINTLINE();
}

// Reads latitude and longitude at given index
bool RouteSD(long &lat, long &lon, int index) {
  if (!SDRoute) {
    return false;
  }

  File waypoints = SD.open("waypoint.csv");

  if (!waypoints) {
    DEBUG_PRINTLN(F("Error opening waypoints file"));
    return false;
  }

  int idx = 0;
  // Read waypoint file until index
  while (waypoints.available() && idx < index + 3) {
    if (waypoints.read() == '\n') idx++;
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

  File waypoints = SD.open("WAYPOINT.csv");

  if (!waypoints) {
    DEBUG_PRINTLN(F("Error opening waypoints file"));
    return false;
  }

  EEPROMWriteRouteIndex(indexRoute);
  EEPROMWriteRouteLength(lengthRoute);

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

  // DEBUG_PRINT(F("Latitude: "));
  // DEBUG_PRINT(lat);
  // DEBUG_PRINT(F("\t Longitude: "));
  // DEBUG_PRINTLN(lon);

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

  // DEBUG_PRINT(F("Route Index: "));
  // DEBUG_PRINT(indexRoute);
  // DEBUG_PRINT(F("\t Route Length: "));
  // DEBUG_PRINT(lengthRoute);
  // DEBUG_PRINT(F("\t Operator: "));
  // DEBUG_PRINTLN(operatorName);
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
  DEBUG_PRINT(F("Lat: "));
  DEBUG_PRINT(latRoute);
  DEBUG_PRINT(F("Lon: "));
  DEBUG_PRINT(lonRoute);
  DEBUG_PRINT(F("index: "));
  DEBUG_PRINTLN(index);

  EEPROM_WRITE_LAT(index, latRoute);
  EEPROM_WRITE_LON(index, lonRoute);
}

void EEPROMWriteRouteIndex(int routeIndex) {
  DEBUG_PRINT(F("Writing routeIndex: "));
  DEBUG_PRINTLN(routeIndex);
  EEPROM_WRITE_INT(MEMADDR_ROUTEIDX_START, routeIndex);
  int EEPROMrouteIndex = EEPROM_READ_INT(MEMADDR_ROUTELEN_START);
  DEBUG_PRINT(F("Read routeIndex: "));
  DEBUG_PRINTLN(EEPROMrouteIndex);
}

void EEPROMWriteRouteLength(int routeLength) {
  DEBUG_PRINT(F("Writing routeLength: "));
  DEBUG_PRINTLN(routeLength);
  EEPROM_WRITE_INT(MEMADDR_ROUTELEN_START, routeLength);
  int EEPROMrouteLength = EEPROM_READ_INT(MEMADDR_ROUTELEN_START);
  DEBUG_PRINT(F("Read routeLength: "));
  DEBUG_PRINTLN(EEPROMrouteLength);
}

// Populate buffer latitude longitude from EEPROM
void EEPROMReadLatLon(int index) {
  EEPROM_READ_LAT(index, latRoute);
  EEPROM_READ_LON(index, lonRoute);
  DEBUG_PRINT(F("Target - Lat: "));
  DEBUG_PRINT(latRoute);
  DEBUG_PRINT(F(" Lon: "));
  DEBUG_PRINTLN(lonRoute);
}

// Checks validity of coordinate format (-90 <= lat <= 90 && -180 <= lon <= 180)
bool CoordinateValidityFormat(long lat, long lon) {
  // DEBUG_PRINT("Checking format of coordinates... lat: " + String(lat, DEC) + ", lon: " + String(lon, DEC) + " ... ");
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
    // DEBUG_PRINTLN(F("Coordinates format valid!"));
  } else {
    DEBUG_PRINTLN(F("Coordinates format NOT valid!"));
  }

  return validity;
}

// Checks distance between two coordinate sets and returns true if within MAX_DISTANCE_VALID_WAYPOINT
bool CoordinateValidityRange(long lat1, long lon1, long lat2, long lon2) {
  // DEBUG_PRINT(F("Checking range between"));
  // DEBUG_PRINT(F(" lat: "));
  // DEBUG_PRINT(lat1);
  // DEBUG_PRINT(F(" lon: "));
  // DEBUG_PRINT(lon1);
  // DEBUG_PRINT(F("\t and \t"));
  // DEBUG_PRINT(F(" lat: "));
  // DEBUG_PRINT(lat2);
  // DEBUG_PRINT(F(" lon: "));
  // DEBUG_PRINTLN(lon2);

  long distance = DistanceBetweenLong(lat1, lon1, lat2, lon2);

  // DEBUG_PRINT(F("Distance: "));
  // DEBUG_PRINTLN(distance);

  return distance <= ((long)MAX_DISTANCE_VALID_WAYPOINT * (long)MAX_DISTANCE_VALID_WAYPOINT);
}
