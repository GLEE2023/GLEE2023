// Include dependencies (libraries)
#include "CAP.h"

CAP cap(2,1);
int LED1 = 4;

void setup() {
	Serial.begin(9600); // open serial port, set the baud rate to 9600 bps
	cap.begin();
  pinMode(LED1, OUTPUT);
}

void loop(){
	int rawData = cap.getRawData(); 
  digitalWrite(LED1, HIGH);
	Serial.println(rawData); //Print raw data
  digitalWrite(LED1, LOW);
};
