// Include dependencies (libraries)
#include <Arduino.h>
#include "GLEE_Sensor.h"
#include "Capacitive.h"
Capacitive cap(1,true);
int rawData = 0;

void setup() {
  Serial.begin(9600); // open serial port, set the baud rate to 9600 bps
  Serial.println("init");
}

void loop(){
    rawData = analogRead(A0);
    
    Serial.println(rawData); //Print raw data
    delay(500);
};
