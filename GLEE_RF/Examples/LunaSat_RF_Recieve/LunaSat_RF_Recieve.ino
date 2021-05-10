#include <GLEE_Radio.h>

//Initialize RF Object
LunaRadio Rad;

void setup() {
	//Set the data rate to 9600 bits pere second
	Serial.begin(9600);

	//Initialize the radio settings by using the initialize_radio function
	// Argument 1: Set frequency to 915
	// Argument 2: Set output power to 17
	// Argument 3: Set Bandwidth to 250
	// Argument 4: Set spreading factor to 12
	// Argument 5: Set coding rate to 8
	Rad.initialize_radio(915.0,17,250.0,12,8);
}

void loop() {
	//Check if RF successfully recieved tranmsission using the recieve_data_string() function
	//Store Results in a string variable
	String output = Rad.receive_data_string();

	//Output the results 
	Serial.print("Data Recieved: "); Serial.println(output);

	// Print recieved signal strength indicator
	Serial.print("Return Signal Strength Indicator: "); Serial.println(Rad.getRSSI());
}
