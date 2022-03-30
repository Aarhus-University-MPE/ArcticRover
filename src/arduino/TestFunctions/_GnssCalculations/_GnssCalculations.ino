/*
  GeoRover GNSS calculation test

  Mads Rosenhøj Jeppesen
  Aarhus University
  2022
*/

const long lat1Const = -5615939;
long lon1 = 1021605;

long lat2 = -5615882;
long lon2 = 1021729;

double scale = 1.0d / 100000.0d;

const long incrementValue = 1;
void setup() {
  Serial.begin(115200);

  for (int i = 0; i < 101; i++) {
    long lat1 = lat1Const + i * incrementValue;
    double distance = DistanceBetween(coordLong2Double(lat1), coordLong2Double(lon1), coordLong2Double(lat2), coordLong2Double(lon2));
    Serial.print(i);
    Serial.print("\t");
    Serial.print(lat1);
    Serial.print("\t");
    Serial.println(distance);
  }
}

void loop() {
}

double coordLong2Double(long longValue) {
  return ((double)longValue) * scale;
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
