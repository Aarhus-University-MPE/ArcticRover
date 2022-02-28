// Sets pinmode of all pins and writes initial values for outputs
void InitAllPins() {
  // Heartbeat
  pinMode(PI_PRIMARY_HRTBEAT, INPUT_PULLUP);
  pinMode(PO_PRIMARY_HRTBEAT, OUTPUT);
  pinMode(PO_PRIMARY_RST, OUTPUT);

  digitalWrite(PO_PRIMARY_HRTBEAT, false);
  digitalWrite(PO_PRIMARY_RST, false);

  // Assign heartbeat interrupt
  attachInterrupt(PI_INT_HRTBEAT, HeartBeatInInterrupt, FALLING);
}