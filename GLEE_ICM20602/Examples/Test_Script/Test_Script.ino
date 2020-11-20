#include <Arduino.h>
#include "GLEE_Sensor.h"
#include "ICM20602.h"

// Instantiate CIM20602 class with id of one and debugging set to true
ICM20602 accelerometer = ICM20602();
sensor_uint16_vec_t rawAcc;

// Observation configuration
int sampleRate; // Sample rate in samples per second (Hz)

// Dynamic Variable 
sensor_uint16_vec_t acceleration;  // Variable to store an observed temperature
float startTime;    // Variables for timing offset calculations
float endTime;

bool b;

void setup(){
    
    accelerometer.initialize();
    b = accelerometer.begin();
    
    sampleRate = 1;
    Serial.begin(9600);
    
}

void loop(){
    //b = accelerometer.begin();
    
    //acceleration = accelerometer.getRawAccel();
    
    Serial.println(0);
    delay(5000);    
}