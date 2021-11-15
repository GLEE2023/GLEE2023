// Include dependencies (libraries)
#include "VT03.h"

VT03 radfet(2,1);


void setup() {
  Serial.begin(9600); // open serial port, set the baud rate to 9600 bps
  radfet.begin();
}

void loop(){
  int rawData = radfet.getRawData(); 
  Serial.println(rawData); // print raw data
};
