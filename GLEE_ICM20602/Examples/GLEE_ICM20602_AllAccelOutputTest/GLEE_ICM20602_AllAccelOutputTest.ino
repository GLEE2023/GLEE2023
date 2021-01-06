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
    Serial.print("Raw, X-Axis: ");
    Serial.println(accelRaw.x, 8);
    Serial.print("Raw, Y-Axis: ");
    Serial.println(accelRaw.y, 8);
    Serial.print("Raw, Z-Axis: ");
    Serial.println(accelRaw.z, 8);
    Serial.println();

    accelMPS = accelerometer.getMPSAccel();
    Serial.print("MPS, X-Axis: ");
    Serial.println(accelMPS.x, 8);
    Serial.print("MPS, Y-Axis: ");
    Serial.println(accelMPS.y, 8);
    Serial.print("MPS, Z-Axis: ");
    Serial.println(accelMPS.z, 8);
    Serial.println();

    accelG = accelerometer.getGAccel(AFS_2G);
    Serial.print("G, X-Axis: ");
    Serial.println(accelG.x, 8);
    Serial.print("G, Y-Axis: ");
    Serial.println(accelG.y, 8);
    Serial.print("G, Z-Axis: ");
    Serial.println(accelG.z, 8);
    Serial.println();
    
    endTime = millis();
    delay(1000 - (endTime - startTime));
};