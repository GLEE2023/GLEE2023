#include "MLX90395.h"

MLX90395 magnetometer = MLX90395(1,false); // Set-up for the magnetometer with debugging set to false



void setup (){
    Serial.begin(9600);

    if (!magnetometer.begin_I2C()) {          // hardware I2C mode, can pass in address & alt Wire
        Serial.println("No sensor found ... check your wiring?");
        while (1) { delay(10); }
    }

    // Set gain
    //magnetometer.setGain(MLX90393_GAIN_2_5X); //this line may or may not work

    // Set resolution
    magnetometer.setResolution(MLX90395_RES_19,MLX90395_RES_19,MLX90395_RES_16);

    // Set oversampling
    magnetometer.setOSR(MLX90395_OSR_2);

    // Set digital filtering
    magnetometer.setFilter(MLX90395_FILTER_6);


    //Setup done
    Serial.println("Begin printing data:");
    Serial.println("X [uT],Y [uT],Z [uT],Magnitude [uT]");
};

void loop (){
    /* Get a new sensor event, normalized to uTesla */
    mlx_sample_t sample = magnetometer.getSample();

    // get strength by square rooting the sum of squares of each element
    float strength = sqrt(pow(sample.magnetic.x,2) + pow(sample.magnetic.y,2) + pow(sample.magnetic.z,2));

    // Print out magnetic field measurements for each axis
    Serial.print(sample.magnetic.x,4); Serial.print(","); Serial.print(sample.magnetic.y,4); Serial.print(",");
    Serial.print(sample.magnetic.z,4); Serial.print(","); Serial.print(strength,4); Serial.println("");

    delay(1000); // Take samples approx every one second
};
