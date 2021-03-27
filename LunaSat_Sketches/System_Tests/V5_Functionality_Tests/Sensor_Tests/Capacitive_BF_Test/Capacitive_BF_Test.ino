// Include dependencies (libraries)
#include "Capacitive.h"

Capacitive cap(1);

int rawData = 0;

void setup() {
  Serial.begin(9600); // open serial port, set the baud rate to 9600 bps
  cap.begin();
}

void loop(){
    rawData = cap.getRawData();
    
    Serial.println(rawData); //Print raw data
    delay(500);
};
