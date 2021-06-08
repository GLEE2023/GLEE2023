// Include dependencies (libraries)
#include "CAP.h"

CAP cap(2,1);

void setup() {
	Serial.begin(9600); // open serial port, set the baud rate to 9600 bps
	cap.begin();
}

void loop(){
	int rawData = cap.getRawData(); 
	Serial.println(rawData); //Print raw data
};
