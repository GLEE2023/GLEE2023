// Include dependencies (libraries)
#include <Arduino.h>
#include "GLEE_Sensor.h"
#include "Capacitive.h"
Capacitive cap(1,true);
void setup() {
  Serial.begin(9600); // open serial port, set the baud rate to 9600 bps
}

void loop(){
    int rawData = cap.getRawData(A1); //Pin is Argument
    Serial.print(rawData); //Print raw data
};
