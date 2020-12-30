#include <Arduino.h>
#include <GLEE_Radio.h>
#include <LunaSat.h>

LunaSat lunaSat(0, false); 
lunaSat_sample_t sample;
LunaRadio Rad;
char MSG[16];
String msg;

void setup() {
    lunaSat.begin(9600);
    Rad.initialize_radio();
}

void loop() {
    sample = lunaSat.getSample();
    msg = String(sample.temperature);
    msg.toCharArray(MSG,16);
    Rad.transmit_data(MSG);
    delay(1000);  
}
