#include <Servo.h>

Servo servo;

void setup() {
  servo.attach(7); // Servo signal connected to pin D7
}

void loop() {
  servo.write(0);   // Move to 0 degrees
  delay(1000);
  servo.write(90);  // Move to 90 degrees (center)
  delay(1000);
  servo.write(180); // Move to 180 degrees (right)
  delay(1000);
}