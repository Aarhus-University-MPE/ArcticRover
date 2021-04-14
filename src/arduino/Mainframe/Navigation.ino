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


bool Navigate(){
    bool navigation = true;

    if (!NavigationValidityPreChecks()) navigation = false;  

    // Update coordinates
    // distance < min accept -> increment waypoint
    // run validity check if waypoint updated

    // bearing calculations
}

bool NavigationValidityPreChecks(){
    bool navigationPreChecks = true;

    if(!gnssFixValid){
        DEBUG_PRINTLN("No valid GNSS fix!");
        navigationPreChecks = false;
    }
    else{
        // Check validity of coordinates (if not already checked)
        if(!validCoordinate){

            validCoordinate = CoordinateValiditiy();

            if(!validCoordinate) {
                navigationPreChecks = false;
            }
        }
    }

    return navigationPreChecks;
}

bool CoordinateValiditiy(){
    DEBUG_PRINTLN("Checking validity of coordinates ... ");
    bool validity = true;
    if(!CoordinateValidityFormat(lat_target, lon_target)){
        DEBUG_PRINT("Coordinate format NOT valid!")
    }
    return (CoordinateValidityFormat(lat_target, lon_target) && CoordinateValidityRange())
}

// Checks validity of coordinate format (-90 <= lat <= 90 && -180 <= lon <= 180)
bool CoordinateValidityFormat(double lat, double lon){
    DEBUG_PRINT("Checking format of coordinates... lat: " + lat + ", lon: " + lon + " ... ");
    bool validity = true;
    if (lat == 0 || lon == 0){
        validity = false;
        DEBUG_PRINT("coordinates must be non zero ... ");
    }

    if(lat < -90 || lat > 90){
        validity = false;
        DEBUG_PRINT("latitude range must be (-90 < lat < 90) ... ");
    }

    if(lon < -180 || lon > 180){
        validity = false;
        DEBUG_PRINT("longitude range must be (-180 < lon < 180) ... ");
    }
    
    if(validity) DEBUG_PRINTLN("Coordinates format valid!");
    else DEBUG_PRINTLN("Coordinates format NOT valid!");

    return validity;
}

// Checks if distance to coordinate is below maximum range (MAX_DISTANCE_VALID_WAYPOINT)
bool CoordinateValidityRange(){
    DEBUG_PRINT("Checking range between current and target coordinates... lat_current: " + lat_current + ", lon_current: " + lon_current + " ... "+ "lat_target: " + lat_target + ", lon_target: " + lon_target + " ... ");
    bool validity = (CoordinateDistance(lat_current,lon_current,lat_target,lon_target) <= MAX_DISTANCE_VALID_WAYPOINT)

    if(validity) DEBUG_PRINTLN("Coordinates within maximum range!");
    else DEBUG_PRINTLN("Range NOT within maximum range!");
    return validity;
}

/* 
Calculates distancebetween coordinate (lat_A, lon_A) and (lat_B, lon_B) [rad], using Pythagoras used on equi­rectangular projec­tion: https://www.movable-type.co.uk/scripts/latlong.html
- x = Δλ ⋅ cos φm
- y = Δφ
- d = R ⋅ √x² + y²
(φ/λ for lati­tude/longi­tude in radians)
*/
double CoordinateDistance(double lat_A, double lon_A, double lat_B, double lon_B){
    double x = (lon_B - lon_A) * cos((lat_A + lat_B) / 2); 
    double y = (lat_B - lat_A);

    return EARTH_RADIUS * sqrt(x * x + y * y);
}

/* 
Calculates bearing between coordinate (lat_A, lon_A) and (lat_B, lon_B) [rad], using Forward Azimuth https://www.movable-type.co.uk/scripts/latlong.html
- θ = atan2( sin Δλ ⋅ cos φ2 , cos φ1 ⋅ sin φ2 − sin φ1 ⋅ cos φ2 ⋅ cos Δλ )
(φ/λ for lati­tude/longi­tude in radians)
*/
double CoordinateBearing(double lat_A, double lon_A, double lat_B, double lon_B){
    double x = sin(lon_B - lon_A) * cos(lat_B);
    double y = cos(lat_A) * sin(lat_B) - sin(lat_A) * cos(lat_2) * cos(lon_B - lon_A);

    return atan2(x, y);
}