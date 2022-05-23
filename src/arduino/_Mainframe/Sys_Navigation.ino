/*
  GeoRover Navigation Stack

  Mads Rosenhøj Jeppesen
  Aarhus University
  2021
*/

/* Algorithm:
  1. Check coordinates valid
    1.1 Format correct?
    1.2 Distance < min distance?
        -> Valid gps fix? (time since last)
    else break
  2. Calculate current heading
    2.1 Distance between previous waypoints > Min Distance?
        -> Calculate using Forward Azimuth
    2.2 Else skip (Set heading: 0)
  3. Calculate target heading (Difference between current position and target waypoint)
    3.1 Is heading valid?
        -> Calculate using forward Azimuth
    3.2 Else skip (Set target heading: 0)
  4. Calculate change heading
    4.1 difference between current and target

*/

#include "GeneralFunctions.h"
#include "Math.h"

bool navigationPreCheck = false;
unsigned long millisLastNavUpdate;
float maxSpeedScale, autonomySpeedOverride = 0;

// Autonomous Navigation, update current position and heading, update target waypoint and update motor controls
bool Navigate() {
  if (millis() - millisLastNavUpdate < NAVIGATION_CYCLE_DT) {
    return true;
  }
  millisLastNavUpdate = millis();

  // Update pos, heading and target waypoint
  PathingProcess();

  return true;
}

// Increases top speed based on time since autonomy start to avoid high speed start up
float AutonomySpeedScale() {
  unsigned long timeSinceAutonomyStart = millis() - millisAutonomyStart;

  if (timeSinceAutonomyStart > TIME_UNTIL_AUTONOMY_MAX_SPEED) {
    // return 1.0f;
    return autonomySpeedOverride;
  }

  float timeScaledSpeed = timeSinceAutonomyStart * (1.0f - MIN_AUTONOMOUS_SPEED) / TIME_UNTIL_AUTONOMY_MAX_SPEED;
  // return min(1.0f, timeScaledSpeed);  // TODO: Remote control override
  return autonomySpeedOverride;
}

void AutonomySpeedUpdate(float speed) {
  autonomySpeedOverride = speed;
}

void NavigationPreCheckReset() {
  navigationPreCheck = false;
  ResetRouteStatus();

  millisLastNavUpdate = millis() - NAVIGATION_CYCLE_DT;
}

// Resets current waypoint index and restarts route
void ResetNavigation() {
  waypointIndex = 0;
  UpdateWaypoint();
}

//  Checks to run before starting autonomous navigation
bool NavigationPreCheck() {
  if (navigationPreCheck) return true;

  if (millis() - millisLastNavUpdate < NAVIGATION_CYCLE_DT) {
    return navigationPreCheck;
  }

  LedSetSignal(LED_SIGNAL_IDLE);

  millisLastNavUpdate = millis();
  navigationPreCheck  = true;

  bool routeValid = RouteTest();
  bool gnssValid  = GnssSignal();
  bool motorValid = MotorCycle();

  PreCheckStatus(routeValid);
  PreCheckStatus(gnssValid);
  PreCheckStatus(motorValid);

  LedSetSignal();
  return navigationPreCheck;
}

// Sets navigationPreCheck flag to false and signals
void PreCheckStatus(bool status) {
  if (!status) {
    navigationPreCheck = false;
    StatusHaltLed(LED_SIGNAL_ERROR_SHORT_HALT);
  } else {
    StatusHaltLed(LED_SIGNAL_OK_SHORT_HALT);
  }
}
