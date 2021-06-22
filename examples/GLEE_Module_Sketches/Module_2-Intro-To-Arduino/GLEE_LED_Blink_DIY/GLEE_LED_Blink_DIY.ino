// Blink LED
// This script will blink the two LEDs of the LunaSat

// Initialize variables representing the pins for the LEDs
int LED1 = 4;
int LED2 = 5;

void setup() {
  // Initialize LED pins as outputs
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
}

void loop() {
  // Turn on LEDs
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);

  Serial.println("LED 1 and LED 2 are ON"); // prints LED on message to serial monitor
  
  delay(1000); // Wait for 1s (1000ms)

  // Turn off LEDs
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);

  Serial.println("LED 1 and LED 2 are OFF"); // prints LED off message to serial monitor

  delay(1000); // Wait for another second before restarting loop

}
