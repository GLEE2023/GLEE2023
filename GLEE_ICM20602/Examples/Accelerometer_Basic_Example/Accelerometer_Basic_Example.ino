#include <Arduino.h>
#include "GLEE_Sensor.h"
#include "ICM20602.h"

float startTime;    
float endTime;

ICM20602 accelerometer(1, false);
sensor_int16_vec_t accelRaw;
sensor_float_vec_t accelMPS;
sensor_float_vec_t accelG; 

void setup(){
    Serial.begin(9600);

    accelerometer.begin();

    accelerometer.initialize();

    accelerometer.setScale(AFS_2G);
};

void loop(){
    startTime = millis();
    
    accelRaw = accelerometer.getRawAccel();
    accelG = accelerometer.getGAccel(accelRaw);
    accelMPS = accelerometer.getMPSAccel(accelG);

    Serial.print("Raw (Binary), X-Axis: ");
    Serial.println(accelRaw.x, BIN);
    Serial.print("Raw, (Binary): ");
    Serial.println(accelRaw.y, BIN);
    Serial.print("Raw, (Binary): ");
    Serial.println(accelRaw.z, BIN);
    Serial.println();
    
    Serial.print("Raw, X-Axis: ");
    Serial.println(accelRaw.x, DEC);
    Serial.print("Raw, Y-Axis: ");
    Serial.println(accelRaw.y, DEC);
    Serial.print("Raw, Z-Axis: ");
    Serial.println(accelRaw.z, DEC);
    Serial.println();

    Serial.print("G, X-Axis: ");
    Serial.println(accelG.x, 8);
    Serial.print("G, Y-Axis: ");
    Serial.println(accelG.y, 8);
    Serial.print("G, Z-Axis: ");
    Serial.println(accelG.z, 8);
    Serial.println();

    Serial.print("MPS, X-Axis: ");
    Serial.println(accelMPS.x, 8);
    Serial.print("MPS, Y-Axis: ");
    Serial.println(accelMPS.y, 8);
    Serial.print("MPS, Z-Axis: ");
    Serial.println(accelMPS.z, 8);
    Serial.println();
    
    endTime = millis();
    delay(1000 - (endTime - startTime));
};
