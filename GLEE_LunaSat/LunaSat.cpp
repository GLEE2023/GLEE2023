// LunaSat Class contains core lunasat functionality such as transmission and sensing
#include <Arduino.h>
#include <Wire.h>
#include "LunaSat.h"


//Constructor: Initializes lunasat and subsequent sensors
LunaSat::LunaSat(int _id, bool _debug){                             
    // Set lunaSat info
    info.id = _id;
    
    // Initialize sensors

    // Set private variables
    debug = _debug;

    tmp117 = new TMP117(1,true);
    icm20602 = new ICM20602(2,true);
    ak09940 = new AK09940(3,true);

    //TODO: Handle passing modes/configurations as arguments into sensor constructors
    //TODO: LunaSat class should have has its own begin function which begins transmission with other sensors)
}

void LunaSat::begin(int baudRate){
    // The begin function starts sierial communications at the given baud rate.
    Serial.begin(baudRate);
    if (debug){
        Serial.println("LunaSat has begun serial communications");
    }
}

lunaSat_sample_t LunaSat::getSample(void){
    lunaSat_sample_t sample;
    
    if(debug){
        sample.timeStamp = 5;

        sample.temperature = 22;
        
        sample.acceleration.x = 1;
        sample.acceleration.y = 2;
        sample.acceleration.z = 3; 
    
        sample.magnetic.x = 1;
        sample.magnetic.y = 2;
        sample.magnetic.z = 3; 
    }else{
        sample.timeStamp = millis();
        sample.temperature = tmp117->getTemperatureC();
        sample.acceleration = icm20602->getGAccel(AFS_2G);
        sample.magnetic = ak09940->getrawData();
    }
    
    return sample;
}

void LunaSat::dispSample(lunaSat_sample_t sample){
    Serial.print(sample.timeStamp);
    Serial.print(',');
    Serial.print(sample.temperature);
    Serial.print(',');
    Serial.print(sample.acceleration.x);
    Serial.print(',');
    Serial.print(sample.acceleration.y);
    Serial.print(',');
    Serial.print(sample.acceleration.z);
    Serial.print(',');
    Serial.print(sample.magnetic.x);
    Serial.print(',');
    Serial.print(sample.magnetic.y);
    Serial.print(',');
    Serial.print(sample.magnetic.z);
}
