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
    Serial.print(F("Data Ready Pin Showing: ")); Serial.println(magnetometer.dataReady());

    sample = magnetometer.getSample();

    Serial.print(F("Magnetometer Temp: ")); Serial.println(sample.temp,5);
    Serial.print(F("Mag X (nT): ")); Serial.println(sample.magnetic.x,5);
    Serial.print(F("Mag Y (nT): ")); Serial.println(sample.magnetic.y,5);
    Serial.print(F("Mag Z (nT): ")); Serial.println(sample.magnetic.z,5);
    Serial.print(F("Magnetic Field Magnitude: ")); Serial.println(sample.strength);

    // if (dataReadyPin == 1){
    //     rawData = magnetometer.getRawData();
    //     Serial.print(F("Raw Mag X: ")); Serial.println(rawData.x,BIN);
    //     Serial.print(F("Raw Mag Y: ")); Serial.println(rawData.y,BIN);
    //     Serial.print(F("Raw Mag Z: ")); Serial.println(rawData.z,BIN);
    //     Serial.print(F("Raw Temp: ")); Serial.println(rawData.temp,BIN);

    //     temperature = magnetometer.getTemperature(rawData.temp);
    //     magnetic = magnetometer.getMagnetic(rawData);

    //     magStrength = magnetometer.getMagFieldStrength(magnetic);

    //     Serial.print(F("Converted Temp: ")); Serial.println(temperature,5);
    //     Serial.print(F("Converted X Mag: ")); Serial.println(magnetic.x,5);
    //     Serial.print(F("Converted Y Mag: ")); Serial.println(magnetic.y,5);
    //     Serial.print(F("Converted Z Mag: ")); Serial.println(magnetic.z,5);


    //     Serial.print(F("Magnetic Field Magnitude: ")); Serial.println(magStrength);

    //     magnetometer.setOpMode(true, LOW_POWER_1, SINGLE_MEASURE);
    //     delay(50);
    // }


    delay(1000);
};
