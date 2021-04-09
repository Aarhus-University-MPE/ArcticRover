/*
GeoRover Navigation Stack

Mads Rosenhøj Jepepsen
Aarhus University 
2021
*/

/*  
Algorithm:

Check validity of current waypoint (min distance)

Determine current orientation
- previous sampled waypoints? Distance between waypoints larger than min determination?
    - Calculate orientation
- Previous sampled waypoints not far enough between, set current direction to forward (~1 meter to determine orientation).


Figure out direction to current waypoint

 
*/