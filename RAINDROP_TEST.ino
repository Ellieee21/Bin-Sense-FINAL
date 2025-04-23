const int rainSensorPin = 5; // DO pin of rain sensor connected to D5

void setup() {
  Serial.begin(9600);        // Initialize serial communication
  pinMode(rainSensorPin, INPUT); // Set D5 as input
  Serial.println("Raindrop Sensor (DO pin) Test Starting...");
}

void loop() {
  int rainStatus = digitalRead(rainSensorPin); // Read from D5

  if (rainStatus == HIGH) {
    Serial.println("Rain detected! 🌧️");
  } else {
    Serial.println("No rain. ☀️");
  }

  delay(1000); // Wait for 1 second
}