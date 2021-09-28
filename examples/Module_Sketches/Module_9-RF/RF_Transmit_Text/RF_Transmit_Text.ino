#include <GLEE_Radio.h> // Include Radio Library

// Initialize Radio Object
LunaRadio Rad;

void setup() {
	Serial.begin(9600); // Begin serial communications at 9600 baud rate
	Rad.initialize_radio(); // Initialize Radio (with default parameters)
}

void loop() {
	// Call the transmit data method and provide it the string you wish to transmit
	Rad.transmit_data("Hello World!"); 
	delay(1000); // Send the transmission once every ~1 sec
}
