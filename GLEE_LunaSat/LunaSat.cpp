// LunaSat Class contains core lunasat functionality such as transmission and sensing
#include <Arduino.h>
#include <Wire.h>
#include "LunaSat.h"


/**
 * Constructor
 * Parameters: the sensor ID as an integer, array containg active functionality,
 * and the debug mode as a booelan
 * Returns: none
 * This is the constructor for the Capacitive class. 
 * Initializes lunsat and subsequent sensors.
**/
LunaSat::LunaSat(int _id, int _conf[5], bool _debug){                             
    // Set lunaSat info
    info.id = _id;
    
    for(int i = 0; i < 5; i++){ // Might need to replace this with more optimal refrencing
        info.conf[i] = _conf[i];
    }

    // Initialize sensors

    // Set private variables
    debug = _debug;

    if (info.conf[0]){
        tmp117 = new TMP117(1, _debug);
    }

    if (info.conf[1]){
        icm20602 = new ICM20602(2, _debug);
    }
    
    if (info.conf[2]){
        ak09940 = new AK09940(3, _debug);
    }

    if (info.conf[3]){
        tpis1385 = new Thermopile(4, _debug);
    }

    if (info.conf[4]==1){
        Rad = new LunaRadio();
    }
    
    Serial.println(F("Done initalization"));

    // Set indicator LED pin modes
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);

    //TODO: Handle passing modes/configurations as arguments into sensor constructors
    //TODO: LunaSat class should have has its own begin function which begins transmission with other sensors)
}

/**
 * Parameters: int baudRate
 * Returns: if serial communications have began.
 * This function starts serial communication at 
 * given baud rate then sensor begins initialization.
**/
void LunaSat::begin(int baudRate){
    // The begin function starts serial communications at the given baud rate.
    Serial.begin(baudRate);
    if (debug){
        Serial.println("LunaSat has begun serial communications");
    }

    // Sensor specific begins and initializations
    if (info.conf[1]){
        icm20602->begin();
        icm20602->initialize();
        Serial.println(F("Accel Initialized"));
    }

    if (info.conf[2]){
        ak09940->setDebugMode(true);
        ak09940->ak09940WAI();
        ak09940->setDriveMode(LOW_NOISE_1);
        ak09940->setMeasurementMode(POWER_DOWN);
    }

    if (info.conf[4]==1) { 
        Rad->initialize_radio();
        Serial.println(F("Radio Initialized"));
        delay(50);
    }

    // TODO: Implement sensor begin outside of constructor classes implement
    // tmp begin
    // mag begin 
}

/**
 * Parameters: none
 * Returns: sample
 * This function gets the sample through testing 
 * functionality otherwise set acceleration and magnetic.
**/
lunaSat_sample_t LunaSat::getSample(void){
    lunaSat_sample_t sample;
    if (debug){
        // Debug test functonality passes appropriatley type values for testing library interfacing

        sample.timeStamp = millis();
        sample.temperature = tmp117->getTemperatureC_fuzzed();
        sample.acceleration = icm20602->getGAccel_fuzzed();
        sample.magnetic = ak09940->getRawData_fuzzed();
    }
    else {
        sample.timeStamp = millis();

        // Handle Temperature Sensor Sample base on configuration
        if (info.conf[0] == 1){
            sample.temperature = tmp117->getTemperatureC();
        }else{
            sample.temperature = 0;
        }
        
        // Hangle Acceleration Sample based on configuration
        if (info.conf[1] == 1){
            sample.acceleration = icm20602->getGAccel();
        } else {
            sample.acceleration.x = 0;
            sample.acceleration.y = 0;
            sample.acceleration.z = 0;
        }
        
        // Handle Magnetic Sample based on configuration
        if (info.conf[2] == 1){
            // Get standard single magnetometer measurement. 
            // TODO: Generalize with respect to magnetometer mode
            if(ak09940->getMeasurementMode() == POWER_DOWN){
                ak09940->setMeasurementMode(SINGLE_MEASURE);
                ak09940->updateRawData();
                ak09940->updateCalculatedData();
            }else if(ak09940->getMeasurementMode() == SINGLE_MEASURE){
                ak09940->updateRawData();
                ak09940->updateCalculatedData();
            }

            sample.magnetic = ak09940->getCalculatedData();
        } else {
            sample.magnetic.x = 0;
            sample.magnetic.y = 0;
            sample.magnetic.z = 0;
        }

        // Handle Thermopile Sample based on configuration
        if (info.conf[3] == 1){
            sample.ObjTemperature = tpis1385->getObjectTemperature();
        }
    }
    return sample;
}

/**
 * Parameters: lunaSat_sample_t sample
 * Returns: display of sample
 * This function displays the sample including
 * temperature, acceleration, and magnetic.
**/
void LunaSat::dispSample(lunaSat_sample_t sample){
    Serial.print(sample.timeStamp);
    Serial.print(',');
    if (info.conf[0]==1){
        Serial.print(sample.temperature);
        Serial.print(',');
    }
    if (info.conf[1]==1){
        Serial.print(sample.acceleration.x);
        Serial.print(',');
        Serial.print(sample.acceleration.y);
        Serial.print(',');
        Serial.print(sample.acceleration.z);
        Serial.print(',');
    }
    if (info.conf[2]==1){
        Serial.print(sample.magnetic.x);
        Serial.print(',');
        Serial.print(sample.magnetic.y);
        Serial.print(',');
        Serial.print(sample.magnetic.z);
    }
    Serial.println("");
}

/**
 * Parameters: int _LED and int _delay
 * Returns: none
 * This function to make the lunasat blink
 * given delays between high and low. 
**/
void LunaSat::blink(int _LED, int _delay){
    int pin;
    
    if (_LED == 1){
        pin = LED1;
    }else if (_LED == 2){
        pin = LED2;
    }else{
        // Invalid Pin
    }

    digitalWrite(pin, HIGH);
    delay(_delay);
    digitalWrite(pin, LOW);
    delay(_delay);
}

void LunaSat::transmitSample(lunaSat_sample_t sample){
    Rad->transmit_data((char*)&sample);
}