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
    
    accelG = accelerometer.getGAccel(AFS_2G);
    Serial.print("G, X-Axis");
    Serial.println(accelG.x, 8);
    Serial.print("G, Y-Axis");
    Serial.println(accelG.y, 8);
    Serial.print("G, Z-Axis");
    Serial.println(accelG.z, 8);
    Serial.println();
    
    endTime = millis();
    delay(1000 - (endTime - startTime));
};