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
unsigned long classBWindows[6] = {-1,-1,-1,-1,-1,-1}; // This array will hold the start times of additional windows that the LunaSat
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
	if((millis() > windowOneStart && millis() < windowOneEnd) || (millis() > windowTwoStart && millis() < windowTwoEnd)){
		String output = Rad.receive_data_string();
		String header = output.substring(0,3);
    output = output.substring(3);
		// Check Transmission ID, ignore unless reciever and transmitter IDs match
		if(output && header=="SRX"){
			// Output the results 
			Serial.print("Message: "); Serial.println(output);
			int firstloc = output.indexOf('|');
			int secondloc = output.indexOf('|',firstloc);
      classBWindows[0] = stoul(output.substring(firstloc,secondloc).c_str());
      Serial.println(String(classBWindows[0]));
			for(int i = 1; i < 6; i++){
        firstloc = secondloc;
        secondloc = output.indexOf('|',firstloc);
				classBWindows[i] = stol(output.substring(firstloc,secondloc).c_str());
        Serial.println(String(classBWindows[i]));
			}
			
		}
	} else if ((millis() > classBWindows[0]*1000 && millis() < (classBWindows[0]*1000)+1000)) {
    String output = Rad.receive_data_string();
    String header = output.substring(0,3);
    output = output.substring(3);
		if(output){
			// Output the results 
			Serial.println("Inside window 0.");
			Serial.print("Message: "); Serial.println(output);		
		}
	} else if ((millis() > classBWindows[1]*1000 && millis() < (classBWindows[1]*1000)+1000)) {
    String output = Rad.receive_data_string();
    String header = output.substring(0,3);
    output = output.substring(3);
		if(output){
			// Output the results 
			Serial.println("Inside window 0.");
			Serial.print("Message: "); Serial.println(output);		
		}
	} else if ((millis() > classBWindows[2]*1000 && millis() < (classBWindows[2]*1000)+1000)) {
    String output = Rad.receive_data_string();
    String header = output.substring(0,3);
    output = output.substring(3);
		if(output){
			// Output the results 
			Serial.println("Inside window 0.");
			Serial.print("Message: "); Serial.println(output);		
		}
	} else if ((millis() > classBWindows[3]*1000 && millis() < (classBWindows[3]*1000)+1000)) {
    String output = Rad.receive_data_string();
    String header = output.substring(0,3);
    output = output.substring(3);
		if(output){
			// Output the results 
			Serial.println("Inside window 0.");
			Serial.print("Message: "); Serial.println(output);		
		}
	} else if ((millis() > classBWindows[4]*1000 && millis() < (classBWindows[4]*1000)+1000)) {
    String output = Rad.receive_data_string();
    String header = output.substring(0,3);
    output = output.substring(3);
		if(output){
			// Output the results 
			Serial.println("Inside window 0.");
			Serial.print("Message: "); Serial.println(output);		
		}
	} else if ((millis() > classBWindows[5]*1000 && millis() < (classBWindows[5]*1000)+1000)) {
    String output = Rad.receive_data_string();
    String header = output.substring(0,3);
    output = output.substring(3);
		if(output){
			// Output the results 
			Serial.println("Inside window 0.");
			Serial.print("Message: "); Serial.println(output);		
		}
	} else {
		Serial.println("Outside receive window.");
	}
}