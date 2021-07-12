#include "MLX90393.h"

MLX90393 magnetometer = MLX90393(1,false);

AK_Sample_t sample;

void setup (){
    Serial.begin(9600);
    magnetometer.begin_I2C();
    magnetometer.setGain(MLX90393_GAIN_2_5X);
    magnetometer.setResolution(MLX90393_X, MLX90393_RES_19);
    magnetometer.setResolution(MLX90393_Y, MLX90393_RES_19);
    magnetometer.setResolution(MLX90393_Z, MLX90393_RES_16);

    // Set oversampling
    magnetometer.setOversampling(MLX90393_OSR_2);

    // Set digital filtering
    magnetometer.setFilter(MLX90393_FILTER_6);

    //No apparent equivalency for setOpMode()
};

void loop (){   
    float x, y, z;

    sample = magnetometer.getSample();

    if (magnetometer.readData(&x, &y, &z)) {
        Serial.println("Direct Measurements");
        Serial.print("X: "); Serial.print(x, 4); Serial.println(" uT");
        Serial.print("Y: "); Serial.print(y, 4); Serial.println(" uT");
        Serial.print("Z: "); Serial.print(z, 4); Serial.println(" uT");
        Serial.print(F("Magnetic Field Magnitude: ")); 
        Serial.println(sample.strength);
        Serial.println("Sample Measurements");
        Serial.print("X: "); Serial.print(sample.magnetic.x, 4); Serial.println(" uT");
        Serial.print("Y: "); Serial.print(sample.magnetic.y, 4); Serial.println(" uT");
        Serial.print("Z: "); Serial.print(sample.magnetic.z, 4); Serial.println(" uT");
    } else {
        Serial.println("Unable to read XYZ data from the sensor.");
    }
    delay(1000); // Take samples every one sec
};