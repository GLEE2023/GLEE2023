
#include <Arduino.h>
#include "TMP117.h" //Might want to change this to caps for consistancy

TMP117 tempSensor = TMP117();

void setup (){
  tempSensor.reset();
  tempSensor.dataReady();
};

void loop (){   
    delay(1000);
};
