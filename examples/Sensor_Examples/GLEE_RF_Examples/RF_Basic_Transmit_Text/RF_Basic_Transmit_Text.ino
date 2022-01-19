#include <GLEE_Radio.h>

LunaRadio Rad;


void setup() {
  Serial.begin(9600); // Set the data rate to 9600 bits per second

  // Initialize the radio settings by using the initialize_radio function
  // Argument 1: Set frequency to 915
  // Argument 2: Set output power to 17
  // Argument 3: Set Bandwidth to 250
  // Argument 4: Set spreading factor to 12
  // Argument 5: Set coding rate to 8
  Rad.initialize_radio(915.0,17,250.0,12,8);
}

void loop() {
  Rad.transmit_data("Hello World!"); // Transmit data
  delay(1000); // 1000ms (1s) delay
}
