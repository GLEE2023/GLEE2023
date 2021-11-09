#include "MPU6000.h"

MPU6000 accelerometer(1, false); // Sets sensor ID to 1 and debugging to false
sensor_float_vec_t acc; // Saves acceleration readings in a vector structure

void setup(){
    Serial.begin(9600); // Sets baud rate to 9600 for serial transmission and starts serial communication
    accelerometer.begin(); // Begins transmission to the I2C slave device
    accelerometer.initialize(); // Set-up for MPU 
    accelerometer.setAccelRange(MPU6000_RANGE_2_G); // Sets range of accelerometer Range options: 2_G, 4_G, 8_G, 16_G
}

void loop(){
    acc = accelerometer.getSample(); // Gets and saves 3-axis acceleration reading (G)

    Serial.print("Acceleration in Gs, X-Axis: "); Serial.print(acc.x, 8); // Prints out 3-axis acceleration (G)
    Serial.print(" Y-Axis: "); Serial.print(acc.y, 8);
    Serial.print(" Z-Axis: "); Serial.print(acc.z, 8);
    Serial.println();
    
    delay(100); // Waits 100ms between readings
}