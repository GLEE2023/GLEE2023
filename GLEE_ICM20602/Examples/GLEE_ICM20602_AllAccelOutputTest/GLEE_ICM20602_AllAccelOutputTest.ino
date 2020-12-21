#include <Arduino.h>
#include "GLEE_Sensor.h"
#include "ICM20602.h"

float startTime;    
float endTime;

ICM20602 accelerometer(1, true);
sensor_uint16_vec_t accelRaw;
sensor_float_vec_t accelMPS;
sensor_float_vec_t accelG; 

void setup(){

    Serial.begin(9600);

    accelerometer.begin();

    accelerometer.initialize();
};

void loop(){
    startTime = millis();
    
    accelRaw = accelerometer.getRawAccel();
    Serial.println(accelRaw.x);
    accelMPS = accelerometer.getMPSAccel();
    Serial.println(accelMPS.x);
    accelG = accelerometer.getGAccel(AFS_2G);
    Serial.println(accelG.x);
    
    endTime = millis();
    delay(1000 - (endTime - startTime));
};