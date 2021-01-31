#include <Arduino.h>
#include "AK09940.h"
#include "GLEE_Sensor.h"
#include <GLEE_Radio.h>

//only x-axis right now

LunaRadio Rad;
AK09940 ak09940 = AK09940();

void setup(){
    Serial.begin(9600);

    Rad.initialize_radio(915.0, 17, 250.0, 12, 8);

    ak09940.setDebugMode(true);
    ak09940.ak09940WAI();
    ak09940.setDriveMode(LOW_NOISE_1);
    ak09940.setMeasurementMode(POWER_DOWN);
}

void loop(){
    int32_t xMagThreshold = 50;

    Serial.print("Current Measurement Mode of Magnetometer: ");
    Serial.println(ak09940.getMeasurementMode());

    if (ak09940.getMeasurementMode() == POWER_DOWN){
        Serial.print("Current Measurement Mode: ");
        Serial.println("Transitioning from POWER_DOWN to SINGLE_MEASURE");
        ak09940.setMeasurementMode(SINGLE_MEASURE);
        ak09940.getRawData();
        ak09940.getCalculatedData();

        if(ak09940.calculatedData.xMag >= xMagThreshold){
            Rad.transmit_data("Hello World!");
        }
    }
    
    delay(1000);  
}