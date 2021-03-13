#include "GLEE_IR_Thermopile.h"


Thermopile IRsensor;

void setup(){
    Serial.begin(9600);
    IRsensor.getCalibration();
}

void loop(){
    IRsensor.readADC();
    Serial.print(IRsensor.getSensorTemperature());
    Serial.println(IRsensor.getObjectTemperature());
    delay(1000);
}