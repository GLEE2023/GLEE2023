#include "MPU6000.h"

float startTime; // When a measurement has started being taken    
float endTime; // When a measurement has finished being taken
float staticAcceleration = 0; // Acceleration when not moving 
float dynamicAcceleration; // Acceleration during an impact
float accelDif; // Change in acceleration
float impactThreshold = 0.35; // Defines impact threshold (how sensitive the impact sensor is)
int LED = 4;

MPU6000 accelerometer(1, false);

sensor_float_vec_t acc; 

sensor_float_vec_t accelMPS; // Acceleration in meters per second
sensor_float_vec_t accelG; // Acceleration in G's
sensor_int16_vec_t accelRaw; // Acceleration in LSB/G, raw output

void setup(){
    Serial.begin(9600); // Sets baud rate to 9600 for serial transmission 
                        // and starts serial 
    accelerometer.begin(); // Begins transmission to the I2C slave device

    accelerometer.initialize(); // Set-up for MPU 

    accelerometer.setAccelRange(MPU6000_RANGE_2_G); // Sets range of acccelerometer 
                                                    // Range options: 2_G, 4_G, 8_G, 16_G

    pinMode(LED, OUTPUT); // Sets LED
    
    delay(50); // Allow time for sensor initialization to finish

    for(int i = 0; i < 3; i++){ // Calibration period of 3 seconds
        startTime = millis(); // Keeps track of start time before getting measurements

        accelRaw = accelerometer.getRawAcc(); // Gets and saves raw acceleration 
        accelG = accelerometer.getGAccel(accelRaw); // Gets and saves acceleration in G's
        accelMPS = accelerometer.getMPSAccel(accelG); // Gets and saves acceleration in MPS
        
        Serial.print("MPS, X-Axis: "); // Prints out 3-axis acceleration (MPS)
        Serial.println(accelMPS.x, 8);
        Serial.print("MPS, Y-Axis: ");
        Serial.println(accelMPS.y, 8);
        Serial.print("MPS, Z-Axis: ");
        Serial.println(accelMPS.z, 8);
        Serial.println();
    
        staticAcceleration = staticAcceleration + sqrt(pow(accelMPS.x,2) + pow(accelMPS.y,2) + pow(accelMPS.z,2)); // Calculates magnitude of 
                                                                                                                   // current acceleration
        endTime = millis(); // Keeps track of end time after getting measurements 
        delay(1000 - (endTime - startTime)); // Change of time factored into a one second delay
    };
    
    staticAcceleration = staticAcceleration/3; // Calculates and saves average of the sum of the magnitude of current acceleration from above
    Serial.print(F("Calculated Static Acceleration: ")); Serial.println(staticAcceleration); // Prints out static acceleration
};

void loop(){

    startTime = millis(); // Keeps track of start time before getting measurements

    accelRaw = accelerometer.getRawAcc(); // Gets and saves raw acceleration
    accelG = accelerometer.getGAccel(accelRaw); // Gets and saves acceleration in G's
    accelMPS = accelerometer.getMPSAccel(accelG); // Gets and saves acceleration in MPS

    Serial.print("MPS, X-Axis: "); // Prints out 3-axis acceleration (MPS)
    Serial.println(accelMPS.x, 8);
    Serial.print("MPS, Y-Axis: ");
    Serial.println(accelMPS.y, 8);
    Serial.print("MPS, Z-Axis: ");
    Serial.println(accelMPS.z, 8);
    Serial.println();


    dynamicAcceleration = sqrt(pow(accelMPS.x,2) + pow(accelMPS.y,2) + pow(accelMPS.z,2)); // Calculates magnitude of current acceleration

    accelDif = abs(dynamicAcceleration - staticAcceleration); // Subtracts and saves static acceleration from dynamic accelration

    Serial.print(F("Calculated Difference in Acceleration: ")); Serial.println(accelDif); // Print out difference in acceleration

    if(accelDif >= impactThreshold){ // If the difference in acceleration is greater than or equal to the threshold set above
        Serial.println("Impact detected"); // Notify impact has been detected
        digitalWrite(LED, HIGH); // LEDs blink
        delay(500);
        digitalWrite(LED, LOW);
    };
    
    endTime = millis(); // Keeps track of end time after getting measurements 
    delay(100); // Gets measurements every 0.1 seconds (100 milliseconds)
};
