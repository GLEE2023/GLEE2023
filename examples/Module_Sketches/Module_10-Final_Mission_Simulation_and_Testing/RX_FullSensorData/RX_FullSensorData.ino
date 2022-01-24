#include <GLEE_Radio.h>

//Initialize RF Object
LunaRadio Rad;

char buff[100];
String output;

///////////////////////////////////////////////
String id = "11"; // CHANGE TO YOUR TEAM NUMBER
///////////////////////////////////////////////

// Indicator LED (Blue) connected to pin 5
int LED = 5;

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
}

void loop() {
	// Check if RF successfully recieved tranmsission using the recieve_data_string() function
	// Store Results in a string variable
	//String output = Rad.receive_data_string();

  Rad.readData(buff, 100);
  output = "";
  for(int i = 0; i < 100; i++){
    output = output + buff[i];
  }

	String message_id = output.substring(0,2); // ID is contained in the first two characters of message
	String message_content = output.substring(3); // Message content comes after ID

	// Check Transmission ID, ignore unless reciever and transmitter IDs match
	if(output && message_id == id){
		// Output the results 

    // Parse message and output correctly here


    
		//Serial.print("Message: "); Serial.println(message_content);

		// Print recieved signal strength indicator
		Serial.print("Return Signal Strength Indicator: "); Serial.println(Rad.getRSSI());

		// When a valid message is recieved, blink the LED for half a second
		digitalWrite(LED, HIGH);
		delay(500);
		digitalWrite(LED, LOW);
	}
}