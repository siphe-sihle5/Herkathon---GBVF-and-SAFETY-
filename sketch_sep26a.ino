// Define the physical pins on your breadboard
const int sensorPin = 4; // Sound Sensor OUT / DO connected to Pin 4
const int ledPin = 5;    // LED connected to Pin 5

// Variables to keep track of the LED and sensor states
int ledState = LOW;       // Keeps track of whether the LED is currently ON or OFF
unsigned long lastClapTime = 0; 
unsigned long debounceDelay = 200; // Time in milliseconds to ignore extra echo sounds

void setup() {
  Serial.begin(115200);
  pinMode(sensorPin, INPUT);
  pinMode(ledPin, OUTPUT);
  
  digitalWrite(ledPin, ledState); // Ensure LED matches initial state (OFF)
  Serial.println("Clap Toggle System Initialized...");
}

void loop() {
  // Read the sound sensor (Most sensors go LOW when they hear a sound)
  int sensorVal = digitalRead(sensorPin);

  // Check if a sound is detected AND enough time has passed since the last clap
  if (sensorVal == LOW && (millis() - lastClapTime > debounceDelay)) {
    
    // Update the timestamp of this valid clap
    lastClapTime = millis();
    
    // Toggle the LED state: If it was HIGH, make it LOW. If it was LOW, make it HIGH.
    ledState = !ledState; 
    
    // Apply the new state to the physical LED pin
    digitalWrite(ledPin, ledState);
    
    // Print the update to your computer screen for debugging
    Serial.print("Clap detected! LED is now: ");
    if (ledState == HIGH) {
      Serial.println("ON");
    } else {
      Serial.println("OFF");
    }
  }
}
