#include <Arduino.h>
#include <LunaSat.h>

LunaSat lunaSat(0,false);



void setup(){
    lunaSat.begin(9600);
}

void loop(){
    
    delay(2000);
}
