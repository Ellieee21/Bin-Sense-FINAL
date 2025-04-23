#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Stepper.h>
#include <Servo.h>

// I2C LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Stepper motor config (28BYJ-48)
const int stepsPerRevolution = 2048; // full rotation
Stepper stepper(stepsPerRevolution, 8, 10, 9, 11);

// Servo
Servo myServo;
const int servoPin = 7;

// Sensors
const int rainSensorPin = A0;
const int proxSensorPin = 6;

// Values
int moistureLevel = 0;
bool metalDetected = false;

void setup() {
  lcd.begin();
  lcd.backlight();

  pinMode(proxSensorPin, INPUT);
  myServo.attach(servoPin);

  showStandby();
}

void loop() {
  moistureLevel = analogRead(rainSensorPin);
  metalDetected = digitalRead(proxSensorPin);

  if (metalDetected == HIGH) {
    displayMessage("METAL WASTE", "");
    processWaste();
    returnToStandby();
  }
  else if (moistureLevel > 600) {  // Adjust threshold if needed
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WET WASTE");
    lcd.setCursor(0, 1);
    lcd.print("Moisture: " + String(moistureLevel));
    processWaste();
    countdown(10);
    returnToStandby();
  }
  else if (moistureLevel < 400) {
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("DRY WASTE");
    processWaste();
    returnToStandby();
  }
}

void processWaste() {
  rotateStepper(120);  // rotate 120 degrees
  openServo();         // open lid
  delay(2000);         // give time for waste to drop
  closeServo();
}

void rotateStepper(int degrees) {
  int steps = map(degrees, 0, 360, 0, stepsPerRevolution);
  stepper.setSpeed(10);
  stepper.step(steps);
}

void openServo() {
  myServo.write(0); // pakaliwa
  delay(500);
}

void closeServo() {
  myServo.write(90); // default closed
  delay(500);
}

void countdown(int seconds) {
  for (int i = seconds; i > 0; i--) {
    lcd.setCursor(0, 1);
    lcd.print("Reset in: " + String(i) + "s ");
    delay(1000);
  }
}

void returnToStandby() {
  lcd.clear();
  showStandby();
}

void showStandby() {
  lcd.setCursor(1, 0);
  lcd.print("BIN-SENSE READY");
  lcd.setCursor(2, 1);
  lcd.print("TO SEGREGATE!");
}

void displayMessage(String top, String bottom) {
  lcd.clear();
  lcd.setCursor((16 - top.length()) / 2, 0);
  lcd.print(top);
  lcd.setCursor((16 - bottom.length()) / 2, 1);
  lcd.print(bottom);
  delay(2000);
}
