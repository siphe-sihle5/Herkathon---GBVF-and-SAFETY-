// ==========================================
// 1. PIN CONFIGURATIONS
// ==========================================
const int soundSensorPin = 4; // Sound Sensor OUT / DO connected to Pin 4
const int soundLedPin = 5;    // External LED connected to Pin 5 (Clap toggled)
const int pulseSensorPin = A1;// Pulse Sensor data line connected to Analog Pin A1
const int pulseLedPin = 13;   // Built-in yellow LED on Arduino board (Blinks on heart beat)

// ==========================================
// 2. SOUND TRACKING VARIABLES
// ==========================================
int soundLedState = LOW;       
unsigned long lastClapTime = 0; 
unsigned long debounceDelay = 200; // Time in ms to ignore extra echo sound vibrations

// ==========================================
// 3. PULSE TRACKING VARIABLES
// ==========================================
unsigned long lastSampleTime = 0;
unsigned long lastBeatTime = 0;
int thresh = 525;              // Trigger threshold for counting a heartbeat
boolean pulse_signal = false;
int heart_rate = 0;

void setup() {
  // Synchronized to fast 115200 speed so both sensors stream without lag
  Serial.begin(115200); 
  
  // Initialize Sound System Pins
  pinMode(soundSensorPin, INPUT);
  pinMode(soundLedPin, OUTPUT);
  digitalWrite(soundLedPin, soundLedState); 
  
  // Initialize Pulse System Pins
  pinMode(pulseLedPin, OUTPUT);
  digitalWrite(pulseLedPin, LOW);

  Serial.println("==============================================");
  Serial.println("🏆 UBUNTU PANIC MESH - UNIFIED SYSTEM READY 🏆");
  Serial.println("==============================================");
  Serial.println("-> Clap Toggle Monitor Active on Pin 4/5.");
  Serial.println("-> Pulse Monitor Active on Pin A1/13.");
  Serial.println("Place your finger gently on the heart monitor...");
}

void loop() {
  unsigned long currentMillis = millis();

  // =========================================================================
  // TASK A: SOUND SENSOR TRACKING (Runs instantly whenever sound is triggered)
  // =========================================================================
  int soundVal = digitalRead(soundSensorPin);

  // Check if a sound is detected AND enough time has passed since the last clap
  if (soundVal == LOW && (currentMillis - lastClapTime > debounceDelay)) {
    lastClapTime = currentMillis; // Update the timestamp of this valid clap
    soundLedState = !soundLedState; // Toggle state: Invert it (HIGH->LOW or LOW->HIGH)
    digitalWrite(soundLedPin, soundLedState); // Apply to the physical pin 5 LED
    
    Serial.print("🔊 [ACOUSTIC SYSTEM] Clap detected! Alert LED state is now: ");
    Serial.println(soundLedState == HIGH ? "ON" : "OFF");
  }

  // =========================================================================
  // TASK B: PULSE TRACKING (Executes precisely every 2 milliseconds)
  // =========================================================================
  if (currentMillis - lastSampleTime >= 2) {
    lastSampleTime = currentMillis;
    
    int analog_data = analogRead(pulseSensorPin); // Read raw wave form from A1

    // Detect if the pulse wave crosses above your threshold level
    if (analog_data > thresh && pulse_signal == false) {
      pulse_signal = true;
      digitalWrite(pulseLedPin, HIGH); // Blink board LED 13 on beat
      
      unsigned long time_between_beats = currentMillis - lastBeatTime;
      lastBeatTime = currentMillis;

      // Filter out rapid high-frequency sensor noise
      if (time_between_beats > 300) { 
        heart_rate = 60000 / time_between_beats; // Convert time to Beats Per Minute
        
        Serial.print("❤️ [HEALTH MONITOR] Pulse Detected! Current BPM: ");
        Serial.println(heart_rate);
      }
    }

    // Reset the tracking flag once the pulse wave drops back down below baseline
    if (analog_data < thresh && pulse_signal == true) {
      digitalWrite(pulseLedPin, LOW); // Turn off board LED 13
      pulse_signal = false;
    }
  }
}
