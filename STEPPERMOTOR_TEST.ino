#include <Stepper.h>

const int stepsPerRevolution = 2048; // 28BYJ-48 full revolution
Stepper stepper(stepsPerRevolution, 8, 10, 9, 11);

void setup() {
  stepper.setSpeed(10); // RPM
}

void loop() {
  stepper.step(stepsPerRevolution / 3); // Rotate 120 degrees clockwise
  delay(2000);
  stepper.step(-stepsPerRevolution / 3); // Rotate 120 degrees counterclockwise
  delay(2000);
}