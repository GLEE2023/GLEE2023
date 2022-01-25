//First Draft LunaSat
#ifndef LUNASAT_H
#define LUNATSAT_H

#include <Arduino.h>
#include <Wire.h>
#include <String.h>

#include <GLEE_Radio.h> // Library containing pertiant radio functionality

// Include Sensor Libraries

#include "TMP117.h"     // Temperature Sensor Library
#include "TPIS1385.h"   // Thermopile Library
#include "CAP.h"        // Capacitive Sesnor Library
#include "MLX90393.h"   // Magnetometer Library
#include "MPU6000.h"    // Accelerometer Library

// #include "AK09940.h"   // 5.0 Magnetometer Library
// #include "ICM20602.h"  // 5.0 IMU Library

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
    int conf[6];
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
    double TMPtemperature;
    sensor_float_vec_t acceleration;
    mlx_sample_t mag;
    TPsample_t TPTemperature;
    int cap;

} lunaSat_sample_t;

class LunaSat{
    public:
        // Constructor
        // The LunaSat constructor takes one argument
        // Debug which is set to false by defualt. 
        // This controlls whether debug messages are 
        // printed to serial

        LunaSat(int _id, int _conf[6], bool _debug = true);

        

        // Sensor Initializations
        // Methods
        void begin(int baudRate);
        lunaSat_sample_t getSample();
        void dispSample(lunaSat_sample_t sample);
        void transmitSample(lunaSat_sample_t sample);
        lunaSat_info_t info;
        void blink(int _pin, int _delay);

        //Display Acceleration
        void dispAccel(sensor_float_vec_t accel);
    
        //Transmit Data
        void transmit_data(char *buff);
    
    private:
        // Radio class instance
        LunaRadio rad;
    
        // Sensor class instances
        TMP117 *tmp117 = new TMP117(0);
        MPU6000 *mpu6000 = new MPU6000(1);
        MLX90393 *mlx90393 = new MLX90393(2);
        TPIS1385 *tpis1385 = new TPIS1385(3);
        CAP *cap = new CAP(4);

        // 5.0 Sensors
        // ICM20602 *icm20602 = new ICM20602(1,true,false);
        // AK09940 *ak09940 = new AK09940(2);

        bool debug;
};
#endif




