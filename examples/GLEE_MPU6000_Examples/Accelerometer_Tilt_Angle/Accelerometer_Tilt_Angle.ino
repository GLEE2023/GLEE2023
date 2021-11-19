#include "MPU6000.h"

MPU6000 accelerometer(1, false); // Sets sensor ID to 1 and debugging to false
sensor_float_vec_t acc; // Saves acceleration readings in a vector structure
float angleInXDirection;
float angleInYDirection;
float angleInZDirection;
void setup(){
    Serial.begin(9600); // Sets baud rate to 9600 for serial transmission 
                        // and starts serial communication
    accelerometer.begin(); // Begins transmission to the I2C slave device
    accelerometer.initialize(); // Set-up for MPU 
    accelerometer.setAccelRange(MPU6000_RANGE_2_G); // Sets range of acccelrometer 
                                                    // Range options: 2_G, 4_G, 8_G, 16_G
};

void loop(){
    acc = accelerometer.getSample(); // Gets and saves 3-axis acceleration reading (G)

    // Step 1: Simplify LunaSat to 3 axes and consider 1 axis at a time.
    // Step 2: For each axis: Consider the maximum and minimum magnitude of acceleration,
    // and also consider the convetions for positve and negative acceleration. Determine what
    // will be 0º for the axis.
    // Step 3: Establish a 360º range of angles and map it to the maximum and minimum accelerations.
    // Step 4: Print out the individual angles.
    angleInXDirection = 0.0;
    angleInYDirection = 0.0;
    angleInZDirection = 0.0;
    
    delay(500); // Waits half a second
};
