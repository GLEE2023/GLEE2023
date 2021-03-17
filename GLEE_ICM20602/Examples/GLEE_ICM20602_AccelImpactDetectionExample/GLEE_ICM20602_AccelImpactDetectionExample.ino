#include <Arduino.h>
#include "GLEE_Sensor.h"
#include "ICM20602.h"

float startTime;    
float endTime;
float staticAcceleration = 0;
float dynamicAcceleration;
float impactThreshold = 0.001; // Defines impact threshold to be 0.001 m/s^2
int LED = 4;

ICM20602 accelerometer(1, false);
sensor_float_vec_t accelMPS;

void setup(){
    Serial.begin(9600);

    accelerometer.begin();

    accelerometer.initialize();

    accelerometer.setScale(1);

    pinMode(LED, OUTPUT);
};

void loop(){

    for(int i = 0; i < 3; i++){ // Calibration period of 3 seconds
        startTime = millis();

        accelMPS = accelerometer.getMPSAccel();
        Serial.print("MPS, X-Axis: ");
        Serial.println(accelMPS.x, 8);
        Serial.print("MPS, Y-Axis: ");
        Serial.println(accelMPS.y, 8);
        Serial.print("MPS, Z-Axis: ");
        Serial.println(accelMPS.z, 8);
        Serial.println();
    
        staticAcceleration = staticAcceleration + sqrt(pow(accelMPS.x,2) + pow(accelMPS.y,2) + pow(accelMPS.z,2));

        endTime = millis();
        delay(1000 - (endTime - startTime));
    };

    staticAcceleration = staticAcceleration/3;

    startTime = millis();

    accelMPS = accelerometer.getMPSAccel();
    Serial.print("MPS, X-Axis: ");
    Serial.println(accelMPS.x, 8);
    Serial.print("MPS, Y-Axis: ");
    Serial.println(accelMPS.y, 8);
    Serial.print("MPS, Z-Axis: ");
    Serial.println(accelMPS.z, 8);
    Serial.println();

    dynamicAcceleration = sqrt(pow(accelMPS.x,2) + pow(accelMPS.y,2) + pow(accelMPS.z,2));

    if(dynamicAcceleration >= impactThreshold){
         Serial.print("Impact detected");

         digitalWrite(LED, HIGH);
         delay(100);
         digitalWrite(LED, LOW);
    };
    
    endTime = millis();
    delay(100 - (endTime - startTime)); // get measurements every 0.1 second
};
