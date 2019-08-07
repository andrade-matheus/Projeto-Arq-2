
void setup() {
  pinMode(33, OUTPUT);
}

void loop() {
  digitalWrite(33, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(33, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
