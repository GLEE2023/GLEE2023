#include <Arduino.h>
#include <LunaSat.h>

LunaSat lunaSat(0,false);

TMP117 tmp117(1,true); 
ICM20602 icm20602(2,true);
AK09940 ak09940(3,true);

void setup(){
    lunaSat.begin(9600);
}

void loop(){
    lunaSat_sample_t testSample = lunaSat.getSample();
    lunaSat.tmp117.getTemperatureC();
    delay(2000);
}