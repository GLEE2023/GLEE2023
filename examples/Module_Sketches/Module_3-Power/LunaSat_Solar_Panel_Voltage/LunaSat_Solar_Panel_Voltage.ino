// LunaSat Solar Panel Voltage Reader (modified arduino analog example)
// Note: P8 Jumper must be connected.

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int solarCellReading = analogRead(A1);
  
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 3.3V):
  float voltage = solarCellReading * (3.3 / 1023.0);
  
  // print out the value you read:
  Serial.println(voltage);
}
