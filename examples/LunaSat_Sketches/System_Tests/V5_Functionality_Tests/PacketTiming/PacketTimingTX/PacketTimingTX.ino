#include <GLEE_Radio.h> // Include Radio Library

// Initialize Radio Object
LunaRadio Rad;

String message = "Pong"; 

String fullMessage = String(message);

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
  // Check if RF successfully recieved tranmsission using the recieve_data_string() function
	// Store Results in a string variable
	String output = Rad.receive_data_string();

	// Check Transmission ID, ignore unless reciever and transmitter IDs match
	if(output){
		// Output the results 
		Serial.print("Message: "); Serial.println(output);
    Rad.transmit_data(p);
	}
}