
#include "MLX90395.h"

MLX90395 magnetometer = MLX90395(1,false);

mlx_sample_t sample;
float strength;

void setup (){
    Serial.begin(9600);

    magnetometer.begin_I2C();

    // Set gain
    magnetometer.setGain(gainMultipliers[16]);

    // Set resolution
    magnetometer.setResolution(MLX90395_RES_19, MLX90395_RES_19, MLX90395_RES_16);
    // Set oversampling
    magnetometer.setOSR(MLX90395_OSR_2);

    // Set digital filtering
    magnetometer.setFilter(MLX90395_FILTER_6);

};

void loop (){   

    sample = magnetometer.getSample();
    strength = sqrt(pow(sample.magnetic.x,2) + pow(sample.magnetic.y,2) + pow(sample.magnetic.z,2));
    // Print out magnetic field measurements for each axis 
    Serial.println("Magnetic Field Axes Measurements");
    Serial.print("X: "); Serial.print(sample.magnetic.x,4); Serial.println(" uT");
    Serial.print("Y: "); Serial.print(sample.magnetic.y,4); Serial.println(" uT");
    Serial.print("Z: "); Serial.print(sample.magnetic.z,4); Serial.println(" uT");
    Serial.println();
    // Print out strength of magnetic field
    Serial.println("Magnetic Field Strength (Magnitude)"); 
    Serial.print(strength,4); Serial.println(" uT");
    Serial.println();
   
    delay(1000); // Take samples every one second
};
