#include "MPU6000.h"

MPU6000 accelerometer(1, false); // Sets sensor ID to 1 and debugging to false
sensor_float_vec_t acc; // Saves acceleration readings in a vector structure
sensor_float_vec_t accMPS;

float xzAcceleration;
float yzAcceleration;
float xyAcceleration;

float roll; // XZ Angle
float pitch; // YZ Angle
float yaw; // XY Angle

//float avgX = 0.0;
//float avgY = 0.0;
//float avgZ = 0.0;

//int calibrationSize = 20;

void setup(){
    Serial.begin(9600); // Sets baud rate to 9600 for serial transmission 
                        // and starts serial communication
    accelerometer.begin(); // Begins transmission to the I2C slave device
    accelerometer.initialize(); // Set-up for MPU 
    accelerometer.setAccelRange(MPU6000_RANGE_2_G); // Sets range of acccelrometer 
                                                    // Range options: 2_G, 4_G, 8_G, 16_G
    /*
    // Calibration (LunaSat flat on table)
    sensor_float_vec_t calibrationPoints[calibrationSize];    
    for(int i = 0; i < calibrationSize; i++){
      calibrationPoints[i] = accelerometer.getSample();
    }

    // Get sums of axes measurements
    for(int j = 0; j < calibrationSize; j++){
      avgX = avgX + calibrationPoints[j].x;
      avgY = avgY + calibrationPoints[j].y;
      avgZ = avgZ + calibrationPoints[j].z;
    }

    // Find average axes measurements
    avgX = avgX/calibrationSize;
    avgY = avgY/calibrationSize;
    avgZ = avgZ/calibrationSize;
    */

};

void loop(){
    acc = accelerometer.getSample(); // Gets and saves 3-axis acceleration reading (G)
    accMPS = accelerometer.getMPSAccel(acc);
    // Step 1: Simplify LunaSat to 3 axes and consider 3 sets of 2 axes - X&Z, Y&Z, X&Y
    // Step 2: For each set of axes: Use F = mgcos(x) and F = mgsin(x) using the two relevant accelerations to find the angle x

    // Find the net acceleration for each pair of axes
    xzAcceleration = sqrt(pow(accMPS.x,2) + pow(accMPS.z,2));
    yzAcceleration = sqrt(pow(accMPS.y,2) + pow(accMPS.z,2));
    xyAcceleration = sqrt(pow(accMPS.x,2) + pow(accMPS.y,2));

    // Use a = gsin(x) to find angle "along direction of motion" (pretend lunasat is moving in direction of net acceleration) 
    roll = asin(xzAcceleration/-MPU_ONE_G); //[Math may not be correct?]
    pitch = asin(yzAcceleration/-MPU_ONE_G);
    yaw = asin(xyAcceleration/-MPU_ONE_G);

    // Print out results
    Serial.print(F("XZ Angle (Roll): "));
    Serial.println(roll);
    Serial.print(F("YZ Angle (Pitch): "));
    Serial.println(pitch);
    Serial.print(F("XY Angle (Yaw): "));
    Serial.println(yaw);

    delay(500); // Waits half a second
};
