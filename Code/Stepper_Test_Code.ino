// Automatic Blinds Controller - Stepper Test
// NEMA 17, DRV8825, 1/16 microstepping, ESP32

#define STEP 26   // STEP pin
#define DIR 27    // DIR pin
#define EN 25     // ENABLE pin

void setup() {
  pinMode(STEP, OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(EN, OUTPUT);

  digitalWrite(EN, LOW);    // Enable driver
  digitalWrite(DIR, HIGH);  // Set direction
}

void loop() {
  int stepDelay = 750; // microseconds per step, matches #10 bead chain

  digitalWrite(STEP, HIGH);
  delayMicroseconds(stepDelay);
  digitalWrite(STEP, LOW);
  delayMicroseconds(stepDelay);
}