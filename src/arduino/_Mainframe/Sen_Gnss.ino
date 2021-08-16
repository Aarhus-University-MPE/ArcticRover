/*
  GeoRover GNSS functionalities, utilizing the ZED-F9P
  (SparkFun GPS-RTK-SMA Breakout https://www.sparkfun.com/products/16481)

  Primary library used:
    https://github.com/sparkfun/SparkFun_u-blox_GNSS_Arduino_Library

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021
*/

#include <SparkFun_u-blox_GNSS_Arduino_Library.h>

SFE_UBLOX_GNSS myGNSS;

long lastTimeGNSS = 0; // Local timer, limits I2C traffic to u-blox module.

bool InitializeGnss() {
  bool status = myGNSS.begin();
  if(status)
  {
    myGNSS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
    myGNSS.setVal(UBLOX_CFG_RATE_MEAS, GNSS_QUERY_UPDATE_FREQUENCY); //Set measurement rate to 1000ms (1Hz update rate)
  }
  SetStatus(MODULE_GNSS, status);
  return status;
}

bool GnssStatus(){

  return myGNSS.isConnected();
}

void TerminateGnss(){
  myGNSS.end();
}


// Gets positional data in Latitude in degrees * 10^-7
long GnssGetLat() {
  return myGNSS.getLatitude();
}

// Gets positional data in Longitude in degrees * 10^-7
long GnssGetLong() {
  return myGNSS.getLongitude();
}

// Query module and prints Lat, Long, Alt, Acc
void QueryGnss() {
  long latitude = myGNSS.getLatitude();
  DEBUG_PRINT(F("Lat: "));
  DEBUG_PRINT(latitude);
  DEBUG_PRINT(F(" (degrees * 10^-7)"));

  long longitude = myGNSS.getLongitude();
  DEBUG_PRINT(F(" Long: "));
  DEBUG_PRINT(longitude);
  DEBUG_PRINT(F(" (degrees * 10^-7)"));

  long altitude = myGNSS.getAltitude();
  DEBUG_PRINT(F(" Alt: "));
  DEBUG_PRINT(altitude);
  DEBUG_PRINT(F(" (mm)"));

  long accuracy = myGNSS.getPositionAccuracy();
  DEBUG_PRINT(F(" 3D Positional Accuracy: "));
  DEBUG_PRINT(accuracy);
  DEBUG_PRINTLN(F(" (mm)"));
}

double DistanceBetween(double lat1, double long1, double lat2, double long2)
{
  // returns distance in meters between two positions, both specified
  // as signed decimal-degrees latitude and longitude. Uses great-circle
  // distance computation for hypothetical sphere of radius 6372795 meters.
  // Because Earth is no exact sphere, rounding errors may be up to 0.5%.
  // Courtesy of Maarten Lamers
  double delta = radians(long1 - long2);
  double sdlong = sin(delta);
  double cdlong = cos(delta);
  lat1 = radians(lat1);
  lat2 = radians(lat2);
  double slat1 = sin(lat1);
  double clat1 = cos(lat1);
  double slat2 = sin(lat2);
  double clat2 = cos(lat2);
  delta = (clat1 * slat2) - (slat1 * clat2 * cdlong);
  delta = sq(delta);
  delta += sq(clat2 * sdlong);
  delta = sqrt(delta);
  double denom = (slat1 * slat2) + (clat1 * clat2 * cdlong);
  delta = atan2(delta, denom);
  return delta * 6372795;
}

double CourseTo(double lat1, double long1, double lat2, double long2)
{
  // returns course in degrees (North=0, West=270) from position 1 to position 2,
  // both specified as signed decimal-degrees latitude and longitude.
  // Because Earth is no exact sphere, calculated course may be off by a tiny fraction.
  // Courtesy of Maarten Lamers
  double dlon = radians(long2 - long1);
  lat1 = radians(lat1);
  lat2 = radians(lat2);
  double a1 = sin(dlon) * cos(lat2);
  double a2 = sin(lat1) * cos(lat2) * cos(dlon);
  a2 = cos(lat1) * sin(lat2) - a2;
  a2 = atan2(a1, a2);
  if (a2 < 0.0)
  {
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
double CoordinateDistance(double lat_A, double lon_A, double lat_B, double lon_B) {
  double x = (lon_B - lon_A) * cos((lat_A + lat_B) / 2);
  double y = (lat_B - lat_A);

  return EARTH_RADIUS * sqrt(x * x + y * y);
}

/*
  Calculates bearing between coordinate (lat_A, lon_A) and (lat_B, lon_B) [rad], using Forward Azimuth https://www.movable-type.co.uk/scripts/latlong.html
  - θ = atan2( sin Δλ ⋅ cos φ2 , cos φ1 ⋅ sin φ2 − sin φ1 ⋅ cos φ2 ⋅ cos Δλ )
  (φ/λ for lati­tude/longi­tude in radians)
*/
double CoordinateBearing(double lat_A, double lon_A, double lat_B, double lon_B) {
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
