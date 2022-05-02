/*
  Debug functions

  Custom Debug functions to execute by sending one of two commands via Serial
    <ME23> or <MD23>
*/

// Code in this function is executed by sending <ME23>
bool DebugInitialize() {
  // PowerSet(HIGH);

  return true;
}

// Code in this function is executed by sending <MD23>
void DebugTerminate() {
  // PowerSet(LOW);
}