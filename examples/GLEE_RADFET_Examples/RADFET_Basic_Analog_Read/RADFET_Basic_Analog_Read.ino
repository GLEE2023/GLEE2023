// Include dependencies (libraries)
#include "VT03.h"

VT03 radfet(2,1);
int LED1 = 4; //LED1 is connected to pin 4

void setup() {
  Serial.begin(9600); // open serial port, set the baud rate to 9600 bps
  radfet.begin();
  pinMode(LED1, OUTPUT); //have LED1 (pin 4) output info
}

void loop(){
  int rawData = radfet.getRawData(); // get the raw data
  digitalWrite(LED1, HIGH); // set voltage to high
  Serial.println(rawData); // print raw data
  digitalWrite(LED1, LOW); //set voltage to low
};
