// Include Capacitive Sensor Library Dependency
#include "CAP.h"

// Initialize Cap sensor with Id of 2 and debugging enabled (2,1)
CAP cap(2,1);

void setup() {
	Serial.begin(9600); // Open serial port, set the baud rate to 9600 bits per second
	cap.begin(); // Call begin method of cap library for secondary initialization
}

void loop(){
	int rawData = cap.getRawData(); // Retrieve raw analog cap sensor reading and store in initialized variable
	Serial.println(rawData); // Print raw data
	delay(100); // Wait 100ms between samples (no offset adjustment)
};
