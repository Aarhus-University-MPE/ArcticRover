/*
  GeoRover GNSS functionalities, utilizing the ZED-F9P
  (SparkFun GPS-RTK-SMA Breakout https://www.sparkfun.com/products/16481)

  Primary library used:
    https://github.com/sparkfun/SparkFun_u-blox_GNSS_Arduino_Library

  Mads Rosenhøj Jeppesen
  Aarhus University
  2021
*/

#include <SparkFun_u-blox_GNSS_Arduino_Library.h>

SFE_UBLOX_GNSS gnss;

long lastTimeGNSS = 0;  // Local timer, limits I2C traffic to u-blox module.

int gnssTestState        = 0;
long millisGnssTestStart = 0;
long millisLastGnssPrint = 0;

bool InitializeGnss() {
  Wire.begin();
  bool status = gnss.begin();
  if (status) {
    gnss.setI2COutput(COM_TYPE_UBX);                                // Set the I2C port to output UBX only (turn off NMEA noise)
    gnss.setVal(UBLOX_CFG_RATE_MEAS, GNSS_QUERY_UPDATE_FREQUENCY);  // Set measurement rate to 1000ms (1Hz update rate)
  }
  return status;
}

bool GnssStatus() {
  return gnss.isConnected();
}

// Updates current position and Gnss status
bool GnssUpdate() {
  latCurrent = GnssGetLat();
  lonCurrent = GnssGetLong();
}

bool GnssTest() {
  bool testDone = false;
  switch (gnssTestState) {
    case 0:
      DEBUG_PRINT(F("GNSS feed starting for: "));
      DEBUG_PRINT(SYS_TEST_DURATION);
      DEBUG_PRINTLN(F(" ms"));
      millisGnssTestStart = millis();
      gnssTestState++;
      break;
    case 1:
      if (millis() - millisLastGnssPrint > SYS_PRINT_PERIOD_LONG) {
        millisLastGnssPrint = millis();
        QueryGnss();
      }

      if (millis() - millisGnssTestStart > SYS_TEST_DURATION) gnssTestState++;
      break;
    case 2:
      SetStatus(MODULE_GNSS, GnssTime(true));
      gnssTestState = 0;
      testDone      = true;
      DEBUG_PRINTLINE();
    default:
      break;
  }

  return testDone;
}

bool GnssTime(bool print) {
  bool status;
  if (print) DEBUG_PRINT(F("Time and Date is: "));
  if (!gnss.getTimeValid() || !gnss.getDateValid()) {
    if (print) DEBUG_PRINTLN(F("not valid"));
    status = false;
  } else {
    if (print) {
      DEBUG_PRINTLN(F("valid"));
      int year   = gnss.getYear();
      int month  = gnss.getMonth();
      int day    = gnss.getDay();
      int hour   = gnss.getHour();
      int minute = gnss.getMinute();

      DEBUG_PRINT(F("Current time: "));
      DEBUG_PRINT(year);
      DEBUG_PRINT(F("-"));
      DEBUG_PRINT(month);
      DEBUG_PRINT(F("-"));
      DEBUG_PRINT(day);
      DEBUG_PRINT(F("-"));
      DEBUG_PRINT(hour);
      DEBUG_PRINT(F(":"));
      DEBUG_PRINTLN(minute);
    }
    status = true;
  }

  return status;
}

void TerminateGnss() {
  gnss.end();
}

// Gets positional data in Latitude in degrees * 10^-7
long GnssGetLat() {
  return gnss.getLatitude();
}

// Gets positional data in Longitude in degrees * 10^-7
long GnssGetLong() {
  return gnss.getLongitude();
}

// Returns heading in degrees * 10^-5
long GnssGetHeading() {
  return gnss.getHeading();
}

// Query module and prints Lat, Long, Alt, Acc
void QueryGnss() {
  DEBUG_PRINT(F("GNSS: "));

  if (GnssStatus()) {
    long latitude = gnss.getLatitude();
    DEBUG_PRINT(F("Lat: "));
    DEBUG_PRINT(latitude);
    DEBUG_PRINT(F(" (deg * 10^-7)"));

    long longitude = gnss.getLongitude();
    DEBUG_PRINT(F("\tLong: "));
    DEBUG_PRINT(longitude);
    DEBUG_PRINT(F(" (deg * 10^-7)"));

    long altitude = gnss.getAltitude();
    DEBUG_PRINT(F("\tAlt: "));
    DEBUG_PRINT(altitude);
    DEBUG_PRINT(F(" (mm)"));

    long accuracy = gnss.getPositionAccuracy();
    DEBUG_PRINT(F("\t3D Positional Accuracy: "));
    DEBUG_PRINT(accuracy);
    DEBUG_PRINT(F(" (mm)"));

    long heading = GnssGetHeading();
    DEBUG_PRINT(F("\tCurrent Heading: "));
    DEBUG_PRINT(heading);
    DEBUG_PRINTLN(F(" (deg)"));

  } else {
    DEBUG_PRINTLN("ERROR");
  }
}

// Returns current signal status
bool GnssSignal() {
  if (!GetStatus(MODULE_GNSS)) {
    return false;
  }

  return (gnss.getTimeValid() && gnss.getDateValid() && gnss.getGnssFixOk());
}

// returns distance in meters between two positions, both specified
// as signed decimal-degrees latitude and longitude. Uses great-circle
// distance computation for hypothetical sphere of radius 6372795 meters.
// Because Earth is no exact sphere, rounding errors may be up to 0.5%.
// Courtesy of Maarten Lamers
double DistanceBetween(double lat1, double long1, double lat2, double long2) {
  double delta  = radians(long1 - long2);
  double sdlong = sin(delta);
  double cdlong = cos(delta);
  lat1          = radians(lat1);
  lat2          = radians(lat2);
  double slat1  = sin(lat1);
  double clat1  = cos(lat1);
  double slat2  = sin(lat2);
  double clat2  = cos(lat2);
  delta         = (clat1 * slat2) - (slat1 * clat2 * cdlong);
  delta         = sq(delta);
  delta += sq(clat2 * sdlong);
  delta        = sqrt(delta);
  double denom = (slat1 * slat2) + (clat1 * clat2 * cdlong);
  delta        = atan2(delta, denom);
  return delta * 6372795;
}

// returns course in degrees (North=0, West=270) from position 1 to position 2,
// both specified as signed decimal-degrees latitude and longitude.
// Because Earth is no exact sphere, calculated course may be off by a tiny fraction.
// Courtesy of Maarten Lamers
double CourseTo(double lat1, double long1, double lat2, double long2) {
  double dlon = radians(long2 - long1);
  lat1        = radians(lat1);
  lat2        = radians(lat2);
  double a1   = sin(dlon) * cos(lat2);
  double a2   = sin(lat1) * cos(lat2) * cos(dlon);
  a2          = cos(lat1) * sin(lat2) - a2;
  a2          = atan2(a1, a2);
  if (a2 < 0.0) {
    a2 += TWO_PI;
  }
  return degrees(a2);
}

// Returns Distance between two sets of coordinates
double DistanceBetweenLong(long lat1, long long1, long lat2, long long2) {
  return DistanceBetween(CoordLong2Double(lat1), CoordLong2Double(long1), CoordLong2Double(lat2), CoordLong2Double(long2));
}

// Returns Distance between two sets of coordinates
double CourseToLong(long lat1, long long1, long lat2, long long2) {
  return CourseTo(CoordLong2Double(lat1), CoordLong2Double(long1), CoordLong2Double(lat2), CoordLong2Double(long2));
}
