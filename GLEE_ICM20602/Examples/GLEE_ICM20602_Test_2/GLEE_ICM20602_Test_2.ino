// Include dependencies (libraries)
#include <Arduino.h>
#include "GLEE_Sensor.h"
#include "ICM20602.h"

// Instantiate ICM20602 sensor class with id of one and debugging set to false
ICM20602 accelerometer(1,false);

// Observation configuration
int sampleRate;

// Dynamic Variable 
sensor_float_vec_t accG, accMPS;
sensor_uint16_vec_t accRaw;  

float startTime;    
float endTime;

void setup(){
    // Set sample rate to 1 sample per second (1Hz)
    sampleRate = 1;
    Serial.begin(9600);
    // TODO: Document Default Configuration in detail
    
    // Begin communications with the accelerometer 
    accelerometer.begin();
    // Initialize the sensor with default GLEE configuration (gyro disabled, low power mode)
    accelerometer.initialize();
};

void loop(){
    startTime = millis();
    accRaw = accelerometer.getRawAccel();
    accMPS = accelerometer.getMPSAccel();
    accG = accelerometer.getGAccel(AFS_2G);

    Serial.print(accRaw.x, 8);
    Serial.print(", ");
    Serial.print(accRaw.y, 8);
    Serial.print(", ");
    Serial.println(accRaw.z, 8);
    
    Serial.print(accG.x, 8);
    Serial.print(", ");
    Serial.print(accG.y, 8);
    Serial.print(", ");
    Serial.println(accG.z, 8);
    
    Serial.print(accMPS.x, 8);
    Serial.print(", ");
    Serial.print(accMPS.y, 8);
    Serial.print(", ");
    Serial.println(accMPS.z, 8);
    
    endTime = millis();
    delay(sampleRate * 1000 - (endTime - startTime));
};
