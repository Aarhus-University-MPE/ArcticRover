// Sets pinmode of all pins and writes initial values for outputs
void InitAllPins() {

  // Heartbeat
  pinMode(PI_PRIMARY_HRTBEAT, INPUT_PULLUP);
  pinMode(PO_PRIMARY_HRTBEAT, OUTPUT);

  // Reset
  digitalWrite(PO_PRIMARY_RST, true);
  pinMode(PO_PRIMARY_RST, OUTPUT);

  digitalWrite(PO_PRIMARY_HRTBEAT, true);
  digitalWrite(PO_PRIMARY_RST, true);

  // Assign heartbeat interrupt
  attachInterrupt(PI_INT_HRTBEAT, HeartBeatInInterrupt, FALLING);

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}