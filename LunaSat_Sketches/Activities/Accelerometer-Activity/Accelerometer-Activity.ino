#include <Arduino.h>
#include "ICM20602.h" //Might want to change this to caps for consistancy

ICM20602 accelSensor = ICM20602();

void setup (){
  accelSensor.initialize();
};

void loop (){   
    delay(1000);
};