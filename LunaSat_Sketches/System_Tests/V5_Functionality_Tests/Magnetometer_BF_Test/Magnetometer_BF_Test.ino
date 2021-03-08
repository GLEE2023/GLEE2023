#include <Arduino.h>
#include "GLEE_Sensor.h"
#include "AK09940.h"

AK09940 ak09940 = AK09940(0, false);

void setup(){
    Serial.begin(9600);
    ak09940.setDebugMode(true);
    ak09940.ak09940WAI();
    ak09940.setDriveMode(LOW_NOISE_1);
    ak09940.setMeasurementMode(POWER_DOWN);
};

void loop(){   
    Serial.print("Current Measurement Mode: ");
    Serial.println(ak09940.getMeasurementMode());
    if (ak09940.getMeasurementMode() == POWER_DOWN){
        Serial.print("Current Measurement Mode: ");
        Serial.println("Transitioning from POWER_DOWN to SINGLE_MEASURE");        
        ak09940.setMeasurementMode(SINGLE_MEASURE);
        ak09940.getRawData();
        ak09940.getCalculatedData();
        Serial.println("===== START DATA =====");
        Serial.print("X-Axis Magnetic Field (nT): ");
        Serial.println(ak09940.calculatedData.xMag);
        Serial.print("Y-Axis Magnetic Field (nT): ");
        Serial.println(ak09940.calculatedData.yMag);
        Serial.print("Z-Axis Magnetic Field (nT): ");
        Serial.println(ak09940.calculatedData.zMag);
        Serial.print("On Board Temperature Sensor (C): ");
        Serial.println(ak09940.calculatedData.temperature);
        Serial.println("===== END DATA =====");
        Serial.println();
    }
    delay(1000);
};
