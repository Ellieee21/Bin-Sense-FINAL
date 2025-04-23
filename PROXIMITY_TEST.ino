int sensorPin = 6;
int ledPin = 13;

void setup() {
  pinMode(sensorPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int sensorValue = digitalRead(sensorPin);
  if (sensorValue == LOW) {
    Serial.println("🔩 Metal detected!");
    digitalWrite(ledPin, HIGH);
  } else {
    Serial.println("No metal.");
    digitalWrite(ledPin, LOW);
  }
  delay(500);
}