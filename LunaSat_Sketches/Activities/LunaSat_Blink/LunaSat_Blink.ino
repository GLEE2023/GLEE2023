// GLEE Blink Sketch

int LED1 = 4; // LED 1 is attached to I/O pin 4
int LED2 = 5; // LED 2 is attached to I/O pin 4

void setup() {
  // Initialize LED pins as outputs
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
}

// Countiously running loop
void loop() {
  digitalWrite(LED1, HIGH);         // Turn on LED1 by outputting High Voltage to that pin
  digitalWrite(LED2, HIGH);         // Turn on LED2 by outputting High Voltage to that pin
  delay(1000);                      // wait for a second
  digitalWrite(LED1, LOW);          // Turn off LED1 by outputting High Voltage to that pin
  digitalWrite(LED2, LOW);          // Turn off LED2 by outputting High Voltage to that pin
  delay(1000);                      // wait for a second
}
