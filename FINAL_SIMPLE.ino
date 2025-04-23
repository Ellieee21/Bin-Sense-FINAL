#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Stepper.h>
#include <Servo.h>

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Servo
Servo servo;
const int servoPin = 7;

// Stepper
const int stepsPerRevolution = 2048;
Stepper stepper(stepsPerRevolution, 8, 10, 9, 11); // 28BYJ-48 Stepper with ULN2003

// Sensors
const int raindropPin = 5;
const int proximityPin = 6;

// Timing
unsigned long lastActionTime = 0;
const unsigned long cooldownPeriod = 7000; // 7 seconds

void setup() {
  pinMode(raindropPin, INPUT);
  pinMode(proximityPin, INPUT);
  servo.attach(servoPin);
  stepper.setSpeed(10); // adjust for smoother rotation

  lcd.init();
  lcd.backlight();
  standbyDisplay();
  Serial.begin(9600);
}

void loop() {
  int raindropState = digitalRead(raindropPin);
  int proximityState = digitalRead(proximityPin);
  bool isMetalDetected = (Serial.readStringUntil('\n') == "metal detected: 1");

  if (millis() - lastActionTime < cooldownPeriod) return;

  if (raindropState == HIGH) {
    displayWetWaste();
    rotateAndOpen();
    countdown(10);
    standbyDisplay();
    lastActionTime = millis();
  } else if (isMetalDetected) {
    displayMetalWaste();
    rotateAndOpen();
    delay(2000);
    standbyDisplay();
    lastActionTime = millis();
  } else {
    // DRY WASTE default detection
    displayDryWaste();
    rotateAndOpen();
    countdown(7);
    standbyDisplay();
    lastActionTime = millis();
  }
}

void standbyDisplay() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  BIN-SENSE READY");
  lcd.setCursor(1, 1);
  lcd.print(" TO SEGREGATE!");
}

void displayWetWaste() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("     WET WASTE");
  lcd.setCursor(0, 1);
  lcd.print("Moisture: HIGH");
}

void displayDryWaste() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("    DRY WASTE");
}

void displayMetalWaste() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("   METAL WASTE");
}

void rotateAndOpen() {
  stepper.step(stepsPerRevolution / 3); // 120 degrees
  delay(500);
  servo.write(0);  // open left
  delay(1500);
  servo.write(90); // close
}

void countdown(int seconds) {
  for (int i = seconds; i > 0; i--) {
    lcd.setCursor(0, 1);
    lcd.print("Next in: ");
    lcd.print(i);
    lcd.print("s ");
    delay(1000);
  }
}
