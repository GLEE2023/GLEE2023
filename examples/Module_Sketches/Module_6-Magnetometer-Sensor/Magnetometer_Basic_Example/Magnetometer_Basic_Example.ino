#include "MLX90395.h"

MLX90395 magnetometer = MLX90395(1,false); // Set-up for the magnetometer with debugging set to false

mlx_sample_t sample;


void setup (){
    Serial.begin(9600);
    if (!magnetometer.begin_I2C()) {          // hardware I2C mode, can pass in address & alt Wire
        Serial.println("No sensor found ... check your wiring?");
        while (1) { delay(10); }
    }


    // see 18.3.1 in the mlx90395 datasheet for info on default values.
    // The following enumerators are defined in MLX90395.h.

    // Set gain
    magnetometer.setGain(8); //default value is 8, can range from 0-15

    // Set resolution
    // See section 18.6 Sensitivity in the MLX90395 datasheet for explanation.
    // These values are equivalent to 1 and are the default
    magnetometer.setResolution(MLX90395_RES_17,MLX90395_RES_17,MLX90395_RES_17);


    // Set oversampling. See 18.4 in the datasheet
    magnetometer.setOSR(MLX90395_OSR_4); //not default. default is MLX90395_OSR_1 and is equivalent to 0

    // Set digital filtering. (Built in averaging)
    magnetometer.setFilter(MLX90395_FILTER_8); //default. MLX90395_FILTER_1 is equivalent to 0



    //Setup done
    Serial.println("Begin printing data:");
    Serial.println("X[uT],Y[uT],Z[uT],Magnitude[uT],Agnle[deg]");
};

void loop (){
    // Get a sample
    sample = magnetometer.getSample();

    // get strength by square rooting the sum of squares of each element
    float strength = sqrt(pow(sample.magnetic.x,2) + pow(sample.magnetic.y,2) + pow(sample.magnetic.z,2));

    //get angle using atan2(). Returns value from -pi to pi
    float angle = atan2(sample.magnetic.y,sample.magnetic.x)*180/3.14;

    // Print out magnetic field measurements for each axis
    Serial.print(sample.magnetic.x,4); Serial.print(", \t");
    Serial.print(sample.magnetic.y,4); Serial.print(", \t");
    Serial.print(sample.magnetic.z,4); Serial.print(", \t");
    Serial.print(strength,4); Serial.print(", \t");
    Serial.print(angle,4); Serial.println("");

    delay(100); // Take samples approx 10Hz
};
