#include <GLEE_Radio.h> // Include Radio Library

// Initialize Radio Object
LunaRadio Rad;

///////////////////////////////////////////////
String id = "11"; // CHANGE TO YOUR TEAM NUMBER
///////////////////////////////////////////////

String message = "Hello from team"; // You can also change this

// Concatinated full message string
String fullMessage = String(id + " " + message + " " + id);

char buff[40]; // Buffer for passing of message to transciever
char *p = &buff[0]; // Pointer (address) to character array buffer

// Indicator LED (Red) connected to pin 4
int LED = 4;

void setup() {
	Serial.begin(9600); // Begin serial communications at 9600 baud rate

	Rad.initialize_radio(915.0,7,250.0,12,8); // Initialize Radio (with default parameters)

	// Convert full formated message to character array stored in buffer (at address p)
	fullMessage.toCharArray(buff, fullMessage.length()+1);

	// Set LED Pin to output
	pinMode(LED, OUTPUT);
}

void loop() {
	// Call the transmit data method and provide it the pointer to your message
	Rad.transmit_data(p);

	// Blink LED on transmission 
	digitalWrite(LED, HIGH);
	delay(500);
	digitalWrite(LED, LOW);

	delay(3000); // Send the transmission once every ~3 seconds
}