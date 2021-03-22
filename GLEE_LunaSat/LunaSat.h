//First Draft LunaSat
#ifndef LUNASAT_H
#define LUNATSAT_H

#include <Arduino.h>
#include <Wire.h>
#include <String.h>

#include <GLEE_Radio.h> // Library containing pertiant radio functionality

// Include Sensor Libraries
#include "AK09940.h"                // Magnetometer Library
#include "ICM20602.h"               // IMU Library
#include "TMP117.h"                 // Temperature Sensor Library
#include "TPIS1385.h"               // Thermopile Library

#define LED1 4 // Indicator LED Pin Constants defined here
#define LED2 5 

typedef struct{
    float x;
    float y;
    float z;
} vec_t;

// This type represents the structure of lunasat info
typedef struct{

    // Lunasat Info consists of 
        // ID - Identity of LunaSat
        // Name (5 character limit)
        // Conf - Array containing active functionality

    int id;
    String name;
    int conf[5];
    // TODO: Find optimal structure packing    
    
} lunaSat_info_t;

// This type represents the structure of lunasat observations
// *Very useful for packeting by rf
typedef struct{

    // This may also be expandaded to support 
    // Additional functionality added in future LS revs

    // The LunaSata sample type consists of
        // Time stamp, 
        // Temperature
        // magetic (x,y,z)
        // Acceleratio/ Magnetic Field (x,y,z)

    unsigned long timeStamp;
    double temperature;
    sensor_float_vec_t acceleration;
    sensor_float_vec_t magnetic;
    float ObjTemperature;

} lunaSat_sample_t;

class LunaSat{
    public:
        // Constructor
        // The LunaSat constructor takes one argument
        // Debug which is set to false by defualt. 
        // This controlls whether debug messages are 
        // printed to serial

        LunaSat(int _id, int _conf[5], bool _debug = false);    

        // Sensor Initializations
        // Methods
        void begin(int baudRate);                      
        lunaSat_sample_t getSample();
        void dispSample(lunaSat_sample_t sample);
        void transmitSample(lunaSat_sample_t sample);
        lunaSat_info_t info;
        void blink(int _pin, int _delay);
    
    private:
        // Radio class instance
        LunaRadio Rad;
    
        // Sensor class instances
        TMP117 tmp117;
        AK09940 ak09940;
        TPIS1385 tpis1385;
    
        bool debug;
    
};
#endif




