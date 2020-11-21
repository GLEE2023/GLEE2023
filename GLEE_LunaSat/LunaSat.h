//First Draft LunaSat
#ifndef LUNASAT_H
#define LUNATSAT_H

#include <Arduino.h>
#include <Wire.h>
#include <String.h>

//#include "AK09940.h"
//#include "ICM20602.h"
//#include "TMP117.h"


typedef struct {
    float x;
    float y;
    float z;
} vec_t;

typedef struct {
    // This type represents the structure of lunasat info

    // Lunasat Info consists of 
        // ID
        // Name (5 character limit)

    int id;
    String name;

    // TODO: Find optimal structure packing    
} lunaSat_info_t;


typedef struct {
    // This type represents the structure of lunasat observations
    // *Very useful for packeting by rf

    // This may also be expandaded to support 
    // additional functionality added in future LS revs

    // The LunaSata sample type consists of
        // Time stamp, 
        // Temperature
        // magetic (x,y,z)
        // Acceleratio/ Magnetic Field (x,y,z)

    unsigned long timeStamp;
    float temperature;
    vec_t acceleration;
    vec_t magnetic;
} lunaSat_sample_t;

class LunaSat {
    public:
        // Constructor
        // The LunaSat constructor takes one argument
        // Debug which is set to false by defualt. 
        // This controlls whether debug messages are 
        // printed to serial

        LunaSat(int _id, bool _debug = false);    

        // Methods
        void begin(int baudRate);                      
        lunaSat_sample_t getSample();
        void dispSample(lunaSat_sample_t sample);
        

        // Variables
        lunaSat_info_t info;

        // TMP117 tmp117;
        // AK09940 ak09940;
        // ICM20602 icm20602;

    private:
        bool debug;
};



#endif




