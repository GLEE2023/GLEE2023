// Include dependencies (libraries)
#include "Cap.h"

Capacitive cap(1);

void setup() {
	Serial.begin(9600); // open serial port, set the baud rate to 9600 bps
	cap.begin();
}

void loop(){
	int rawData = cap.getRawData(); //Pin is Argument
	Serial.println(rawData); //Print raw data
};
