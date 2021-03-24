#include "AK09940.h"

AK09940 magnetometer = AK09940(0,false);

AK_Sample_t sample;

void setup (){
    Serial.begin(9600);
    delay(3000);
    Serial.println("initializing");
    magnetometer.begin();
    magnetometer.readWAI();
    magnetometer.setOpMode(true, LOW_POWER_1, POWER_DOWN);
};

void loop (){   
    Serial.print(F("Data Ready: ")); Serial.println(magnetometer.dataReady());

    sample = magnetometer.getSample();

    Serial.print(F("Magnetometer Temp: ")); Serial.println(sample.temp,5);
    Serial.print(F("Mag X (nT): ")); Serial.println(sample.magnetic.x,5);
    Serial.print(F("Mag Y (nT): ")); Serial.println(sample.magnetic.y,5);
    Serial.print(F("Mag Z (nT): ")); Serial.println(sample.magnetic.z,5);
    Serial.print(F("Magnetic Field Magnitude: ")); Serial.println(sample.strength);

    delay(1000);
};
