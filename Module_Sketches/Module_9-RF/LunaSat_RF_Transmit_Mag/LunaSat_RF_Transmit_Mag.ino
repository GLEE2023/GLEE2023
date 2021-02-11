#include <Arduino.h>
#include "AK09940.h"
#include <GLEE_Radio.h>

//only x-axis threshold implemented, but can add y and z if needed

LunaRadio Rad;
AK09940 ak09940 = AK09940(0,false); // Initalization Params: Sensor ID = 0, Debugging = False

char MSG[16]; //for converting magnetometer values
String msg;

void setup(){
    Serial.begin(9600);

    Rad.initialize_radio(915.0, 17, 250.0, 12, 8); //basic radio initialization

    //Setup for AK09940
    ak09940.setDebugMode(true);
    ak09940.ak09940WAI(); //check proper initialization
    ak09940.setDriveMode(LOW_NOISE_1);
    ak09940.setMeasurementMode(POWER_DOWN); //not currently taking measurements
}

void loop(){
    int32_t xMagThreshold = 50; //user-defined threshold

    Serial.print("Current Measurement Mode of Magnetometer: ");
    Serial.println(ak09940.getMeasurementMode()); //displays current measurement mode

    if (ak09940.getMeasurementMode() == POWER_DOWN){ //makes sure it's currently powered down
        Serial.print("Current Measurement Mode: ");
        Serial.println("Transitioning from POWER_DOWN to SINGLE_MEASURE");
        ak09940.setMeasurementMode(SINGLE_MEASURE); //changed to single measurement mode
        ak09940.getRawData();
        ak09940.getCalculatedData();
        int32_t calcXMag = ak09940.calculatedData.xMag; //saves the current calculated x-mag

        if(calcXMag >= xMagThreshold){ //checks if the current x-mag is greater than or equal to threshold
            msg = String(calcXMag); //converts x-mag to char* to be trasnmitted
            msg.toCharArray(MSG, 8);
            Rad.transmit_data(MSG); //then radio transmits data
        }
    }
    
    delay(1000);  
}
