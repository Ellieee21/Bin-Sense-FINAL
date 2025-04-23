#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Stepper.h>
#include <Servo.h>

// LCD Setup
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27

// Stepper Motor Setup
const int stepsPerRevolution = 2048; // 28BYJ-48 full rotation
Stepper stepper(stepsPerRevolution, 8, 10, 9, 11);

// Servo Setup
Servo myServo;

// Sensor Pins
const int raindropPin = 5;  // Digital output from raindrop sensor
const int proximityPin = 6; // Metal detection via PC817 optocoupler

// Variables
bool isSegregating = false;
int dryCount = 0;

void setup() {
  pinMode(raindropPin, INPUT);
  pinMode(proximityPin, INPUT);

  lcd.init();
  lcd.backlight();
  myServo.attach(7);
  stepper.setSpeed(10); // Adjust speed if needed

  standbyDisplay();
}

void loop() {
  int rainState = digitalRead(raindropPin);
  int metalDetected = digitalRead(proximityPin);

  if (!isSegregating) {
    if (metalDetected == HIGH) {
      dryCount = 0; // Reset dry count
      segregate("METAL WASTE");
    } 
    else if (rainState == HIGH) {
      dryCount = 0; // Reset dry count
      int moistureLevel = analogRead(A0); // Optional moisture level
      segregateWetWaste(moistureLevel);
    } 
    else {
      dryCount++;
      if (dryCount <= 2) {
        segregate("DRY WASTE");
      } else {
        standbyDisplay();
        dryCount = 0;
      }
    }
  }
}

void segregate(const char* type) {
  isSegregating = true;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(centerText(type));

  rotateStepper(120);
  openServoLeft(); // Now includes auto-close after 5s

  countdown(7);

  standbyDisplay();
  isSegregating = false;
}

void segregateWetWaste(int moisture) {
  isSegregating = true;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WET WASTE");

  lcd.setCursor(0, 1);
  lcd.print("Moisture: ");
  lcd.print(moisture);

  rotateStepper(120);
  openServoLeft(); // Auto-close included

  countdown(7);

  standbyDisplay();
  isSegregating = false;
}

void standbyDisplay() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" BIN-SENSE READY");

  lcd.setCursor(0, 1);
  lcd.print(" TO SEGREGATE!");
}

void rotateStepper(int degree) {
  int steps = map(degree, 0, 360, 0, stepsPerRevolution);
  stepper.step(steps);
}

void openServoLeft() {
  myServo.write(0); // Open left — adjust if needed
  delay(500);
  delay(5000);       // Wait 5 seconds before auto-close
  closeServo();
}

void closeServo() {
  myServo.write(90); // Closed position — adjust if needed
  delay(500);
}

void countdown(int seconds) {
  for (int i = seconds; i > 0; i--) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Next in:");
    lcd.setCursor(0, 1);
    lcd.print(i);
    lcd.print(" seconds");
    delay(1000);
  }
}

String centerText(const char* text) {
  String str = String(text);
  int spaces = (16 - str.length()) / 2;
  String result = "";
  for (int i = 0; i < spaces; i++) result += " ";
  result += str;
  return result;
}