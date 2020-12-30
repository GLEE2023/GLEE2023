// GLEE 2023 Module 5: Acceleration - Basic acceleration output via serial in Gs

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

    Serial.println("X-Axis (Gs), Y-Axis(Gs), Z-Axis(Gs)");
};

void loop(){
    startTime = millis();
    
    accelG = accelerometer.getGAccel(AFS_2G);
    
    Serial.print(accelG.x, 8);
    Serial.print(",");
    Serial.print(accelG.y, 8);
    Serial.print(",");
    Serial.println(accelG.z, 8);
    
    endTime = millis();
    delay(1000 - (endTime - startTime));
};