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
long GnssGetHeading(){
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
    DEBUG_PRINTLN(F(" (mm)"));
  } else {
    DEBUG_PRINTLN("ERROR");
  }
}

// Returns current signal status
bool GnssSignal(){
  if(!GetStatus(MODULE_GNSS)){
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

/*
  Calculates distancebetween coordinate (lat_A, lon_A) and (lat_B, lon_B) [rad], using Pythagoras used on equi­rectangular projec­tion: https://www.movable-type.co.uk/scripts/latlong.html
  - x = Δλ ⋅ cos φm
  - y = Δφ
  - d = R ⋅ √x² + y²
  (φ/λ for lati­tude/longi­tude in radians)
*/
long CoordinateDistance(long lat_A, long lon_A, long lat_B, long lon_B) {
  long x = (lon_B - lon_A) * cos((lat_A + lat_B) / 2);
  long y = (lat_B - lat_A);

  return EARTH_RADIUS * sqrt(x * x + y * y);
}

/*
  Calculates bearing between coordinate (lat_A, lon_A) and (lat_B, lon_B) [rad], using Forward Azimuth https://www.movable-type.co.uk/scripts/latlong.html
  - θ = atan2( sin Δλ ⋅ cos φ2 , cos φ1 ⋅ sin φ2 − sin φ1 ⋅ cos φ2 ⋅ cos Δλ )
  (φ/λ for lati­tude/longi­tude in radians)
*/
double CoordinateBearing(long lat_A, long lon_A, long lat_B, long lon_B) {
  double x = sin(lon_B - lon_A) * cos(lat_B);
  double y = cos(lat_A) * sin(lat_B) - sin(lat_A) * cos(lat_B) * cos(lon_B - lon_A);

  return atan2(x, y);
}

/*
  static void AveragingGnss()
  {
  static bool warned = false; // that we're waiting for a valid location

  if (fix.valid.location && fix.valid.date && fix.valid.time) {

    if (count == 0) {

      // Just save the first good fix
      first = fix;
      firstSecs = (clock_t) first.dateTime;
      count = 1;

    } else {

      // After the first fix, accumulate locations until we have
      //   a good average.  Then display the offset from the average.

      if (warned) {
        // We were waiting for the fix to be re-acquired.
        warned = false;
        DEBUG_PORT.println();
      }

      DEBUG_PORT.print( count );

      if (!doneAccumulating) {

        // Enough time?
        if (((clock_t)fix.dateTime - firstSecs) > 2 * SECONDS_PER_HOUR)
          doneAccumulating = true;
      }

      int32_t dLat, dLon;

      if (!doneAccumulating) {

        // Use deltas from the first location
        dLat = fix.location.lat() - first.location.lat();
        sumDLat += dLat;
        int32_t avgDLat = sumDLat / count;

        dLon = fix.location.lon() - first.location.lon();
        sumDLon += dLon;
        int32_t avgDLon = sumDLon / count;

        //  Then calculated the average location as the first location
        //     plus the averaged deltas.
        avgLoc.lat( first.location.lat() + avgDLat );
        avgLoc.lon( first.location.lon() + avgDLon );

        count++;
      }

      DEBUG_PORT.print( ',' );
      DEBUG_PORT.print( avgLoc.lat() );
      DEBUG_PORT.print( ',' );
      DEBUG_PORT.print( avgLoc.lon() );
      DEBUG_PORT.print( ',' );
      dLat = avgLoc.lat() - fix.location.lat();
      DEBUG_PORT.print( dLat );
      DEBUG_PORT.print( ',' );
      dLon = avgLoc.lon() - fix.location.lon();
      DEBUG_PORT.print( dLon );

      // Calculate the distance from the current fix to the average location
      float avgDistError = avgLoc.DistanceKm( fix.location );
      DEBUG_PORT.print( ',' );
      DEBUG_PORT.print( avgDistError * 100000.0 ); // cm

      // Calculate the bearing from the current fix to the average location.
      //   NOTE: other libraries will have trouble with this calculation,
      //   because these coordinates are *VERY* close together.  Naive
      //   floating-point calculations will not have enough significant
      //   digits.
      float avgBearingErr = fix.location.BearingTo( avgLoc );
      float bearing       = avgBearingErr * Location_t::DEG_PER_RAD;
      DEBUG_PORT.print( ',' );
      DEBUG_PORT.print( bearing, 6 );
      DEBUG_PORT.print( ',' );
      DEBUG_PORT.print( compassDir( bearing ) );

      // Calculate a point that is 10km away from the average location,
      //   at the error bearing
      Location_t tenKmAway( avgLoc );
      tenKmAway.OffsetBy( 10.0 / Location_t::EARTH_RADIUS_KM, avgBearingErr );
      DEBUG_PORT.print( ',' );
      DEBUG_PORT.print( tenKmAway.lat() );
      DEBUG_PORT.print( ',' );
      DEBUG_PORT.print( tenKmAway.lon() );

      // Calculate the bearing from the average location to that point.
      //    This should be very close to the avgBearingErr, and will
      //    reflect the calculation error.  This is because the
      //    current fix is *VERY* close to the average location.
      float tb = avgLoc.BearingToDegrees( tenKmAway );
      DEBUG_PORT.print( ',' );
      DEBUG_PORT.print( tb, 6 );

      DEBUG_PORT.println();
    }

  } else {
    if (!warned) {
      warned = true;
      DEBUG_PORT.print( F("Waiting for fix...") );
    } else {
      DEBUG_PORT.print( '.' );
    }
  }

  }


*/
