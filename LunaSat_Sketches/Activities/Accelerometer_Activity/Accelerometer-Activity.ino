// Include dependencies (libraries)
#include <Arduino.h>
#include "GLEE_Sensor.h"
#include "ICM20602.h"

// Instantiate ICM20602 sensor class with id of one and debugging set to true
ICM20602 accelerometer(1,true);

// Observation configuration
int sampleRate;

// Dynamic Variable 
sensor_float_vec_t acceleration;  
float startTime;    
float endTime;

void setup(){
    // Set sample rate to 1 sample per second (1Hz)
    sampleRate = 1;
    Serial.begin(9600);
    
    // Begin communications with the accelerometer 
    accelerometer.begin();

    // Initialize the sensor with default GLEE configuration (gyro disabled, low power mode)
    accelerometer.initialize();
};

void loop(){
    startTime = millis();
    
    // TODO: Check argument against configuration
    acceleration = accelerometer.getGAccel(AFS_2G);
    
    Serial.print(acceleration.x);
    Serial.print(',');
    Serial.print(acceleration.y);
    Serial.print(',');
    Serial.println(acceleration.z);

    endTime = millis();
    delay(sampleRate * 1000 - (endTime - startTime));
};