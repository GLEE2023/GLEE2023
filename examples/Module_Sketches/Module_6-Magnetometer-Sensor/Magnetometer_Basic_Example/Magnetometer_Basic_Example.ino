#include "MLX90393.h"

MLX90393 magnetometer = MLX90393(1,false); // Magnetometer set up, debug is false

mlx_sample_t sample; // Saves sample reading

void setup (){
    Serial.begin(9600);

    magnetometer.begin_I2C();

    // Set gain
    magnetometer.setGain(MLX90393_GAIN_2_5X);

    // Set resolution
    magnetometer.setResolution(MLX90393_X, MLX90393_RES_19);
    magnetometer.setResolution(MLX90393_Y, MLX90393_RES_19);
    magnetometer.setResolution(MLX90393_Z, MLX90393_RES_16);

    // Set oversampling
    magnetometer.setOversampling(MLX90393_OSR_2);

    // Set digital filtering
    magnetometer.setFilter(MLX90393_FILTER_6);

};

void loop (){   

    sample = magnetometer.getSample(); // Get sample measurements

    // Print out magnetic field measurements for each axis 
    Serial.println("Magnetic Field Axes Measurements");
    Serial.print("X: "); Serial.print(sample.magnetic.x,4); Serial.println(" uT");
    Serial.print("Y: "); Serial.print(sample.magnetic.y,4); Serial.println(" uT");
    Serial.print("Z: "); Serial.print(sample.magnetic.z,4); Serial.println(" uT");
    Serial.println();
    // Print out strength of magnetic field
    Serial.println("Magnetic Field Strength (Magnitude)"); 
    Serial.print(sample.strength,4); Serial.println(" uT");
    Serial.println();
   
    delay(1000); // Take samples every one second
};
