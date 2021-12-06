/* NOTE: The output of this sketch may be skewed if the LunaSat undergoes a force that isn't gravity */

#include "MPU6000.h"

MPU6000 accelerometer(1, false); // Sets sensor ID to 1 and debugging to false
sensor_float_vec_t acc; // Saves acceleration readings in a vector structure
sensor_float_vec_t accMPS;

float measuredGravity = -1.1*MPU_ONE_G;

float xzAcceleration;
float yzAcceleration;
float xyAcceleration;

float xAngle = 0.0;
float yAngle = 0.0;
float zAngle = 0.0;

float avgX = 0.0;
float avgY = 0.0;
float avgZ = 0.0;


int calibrationSize = 40;

void setup(){
    Serial.begin(9600); // Sets baud rate to 9600 for serial transmission 
                        // and starts serial communication
    accelerometer.begin(); // Begins transmission to the I2C slave device
    accelerometer.initialize(); // Set-up for MPU 
    accelerometer.setAccelRange(MPU6000_RANGE_2_G); // Sets range of acccelerometer 
                                                    // Range options: 2_G, 4_G, 8_G, 16_G

    // Calibration (LunaSat flat on table)
    sensor_float_vec_t calibrationPoints[calibrationSize];    
    for(int i = 0; i < calibrationSize; i++){
        calibrationPoints[i] = accelerometer.getSample();
    }

    // Get sums of axes measurements
    for(int j = 0; j < calibrationSize; j++){
        avgX = avgX + calibrationPoints[j].x;
        avgY = avgY + calibrationPoints[j].y;
        //avgZ = avgZ + calibrationPoints[j].z;
    }

    // Find average axes measurements
    avgX = avgX/calibrationSize;
    avgY = avgY/calibrationSize;
    //avgZ = avgZ/calibrationSize;
    Serial.println(avgX*9.81);

};


void loop(){
    
    acc = accelerometer.getSample(); // Gets and saves 3-axis acceleration reading (G)

    accMPS = accelerometer.getMPSAccel(acc); // Acceleration in meters per second squared

    // Math/logic mostly based on following code: https://www.hobbytronics.co.uk/accelerometer-info

    // Subtract baseline values from current sample

    accMPS.x = accMPS.x - avgX*9.81;
    accMPS.y = accMPS.y - avgY*9.81;
    accMPS.z = accMPS.z;
    
    // Find the net acceleration for each pair of axes
    yzAcceleration = sqrt(pow(accMPS.y,2) + pow(accMPS.z,2)); 
    xzAcceleration = sqrt(pow(accMPS.x,2) + pow(accMPS.z,2)); 
    xyAcceleration = sqrt(pow(accMPS.x,2) + pow(accMPS.y,2)); 

    // Calculate x angle
    xAngle = accMPS.x/yzAcceleration;
    xAngle = atan(xAngle)*(180/3.141);
    Serial.print("X Angle: ");
    Serial.print(xAngle);
    Serial.println("º");

    // Calculate y angle
    yAngle = accMPS.y/xzAcceleration;
    yAngle = atan(yAngle)*(180/3.141);
    Serial.print("Y Angle: ");
    Serial.print(yAngle);
    Serial.println("º");

    // Calculate z angle
    zAngle = accMPS.z/xyAcceleration;
    zAngle = atan(zAngle)*(180/3.141);
    Serial.print("Z Angle: ");
    Serial.print(zAngle);
    Serial.println("º");
    Serial.println();
    

    delay(1000); // Waits 1 second
};