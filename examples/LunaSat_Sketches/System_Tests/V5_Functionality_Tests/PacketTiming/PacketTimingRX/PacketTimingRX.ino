#include <GLEE_Radio.h>

//Initialize RF Object
LunaRadio Rad;

// Indicator LED (Blue) connected to pin 5
int LED = 5;

unsigned long windowOneStart = 0;
unsigned long windowOneEnd = 0;
unsigned long windowTwoStart = 0;
unsigned long windowTwoEnd = 0;
unsigned long windowLength = 1000;
unsigned long tmp = 0;
unsigned long classBWindows[6] = {1,10,20,30,40,50}; // This array will hold the start times of additional windows that the LunaSat
                                // can receive packets during (in minutes). These windows fall after the first two windows 
                                // following transmission. These windows would ideally be set by the gateway but they are
                                // hardcoded for now.

String fullMessage = "Ping";
char buff[40]; // Buffer for passing of message to transciever
char *p = &buff[0]; // Pointer (address) to character array buffer

void setup() {
	//Set the data rate to 9600 bits per second and begin serial communication
	Serial.begin(9600);

	//Initialize the radio settings by using the initialize_radio function
	// Argument 1: Set frequency to 915
	// Argument 2: Set output power to 17
	// Argument 3: Set Bandwidth to 250
	// Argument 4: Set spreading factor to 12
	// Argument 5: Set coding rate to 8
	Rad.initialize_radio(915.0,17,250.0,12,8);

	// Set indicator LED Pin mode to output
	pinMode(LED, OUTPUT);
  tmp = millis();
  fullMessage.toCharArray(buff, fullMessage.length()+1);
  Rad.transmit_data(p);
  windowOneStart = tmp + 0; // Replace with appropriate constant
  windowTwoStart = tmp + 1000; // Replace with appropriate constant
  windowOneEnd = windowOneStart + windowLength; // Replace with length of window
  windowTwoEnd = windowTwoStart + windowLength; // Replace with length of window
}

void loop() {
	// Check if RF successfully recieved tranmsission using the recieve_data_string() function
	// Store Results in a string variable
	String output = Rad.receive_data_string();

	// Check Transmission ID, ignore unless reciever and transmitter IDs match
	if(output && (millis() > windowOneStart && millis() < windowOneEnd) || (millis() > windowTwoStart && millis() < windowTwoEnd)){
		// Output the results 
		Serial.print("Message: "); Serial.println(output);


	}
}