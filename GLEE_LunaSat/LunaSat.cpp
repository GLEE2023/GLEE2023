//First Draft LunaSat
#include <Arduino.h>
#include "LunaSat.h"


//Constructor: Initializes lunasat and subsequent sensors
LunaSat::LunaSat(int _id, bool _debug){                             
    // Set lunaSat info
    info.id = _id;
    
    // Initialize sensors
    //TMP117 tmp117(); 
    //ICM20602 icm20602();
    //AK09940 ak09940();

    // Set private variables
    debug = _debug;

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
    }
    
    //sample.temp = tmp117.getTemperatureC();
    //sample.acc = icm20602.getData();
    //sample.mag = ak09940.getData();
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
