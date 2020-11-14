#include <Arduino.h>
#include <LunaSat.h>

LunaSat lunaSat(0,true);

void setup(){
    lunaSat.begin(9600);
}

void loop(){
    lunaSat_sample_t testSample = lunaSat.getSample();
    lunaSat.dispSample(testSample);
    delay(2000);
}