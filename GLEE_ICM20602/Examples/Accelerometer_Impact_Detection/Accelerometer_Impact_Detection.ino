#include "ICM20602.h"

float startTime;    
float endTime;
float staticAcceleration = 0;
float dynamicAcceleration;
float accelDif;
float impactThreshold = 0.1; // Defines impact threshold to be 0.001 m/s^2
int LED = 4;

ICM20602 accelerometer(1, false);

sensor_float_vec_t accelMPS;
sensor_float_vec_t accelG;
sensor_int16_vec_t accelRaw;

void setup(){
    Serial.begin(9600);

    accelerometer.begin();

    accelerometer.initialize();

    accelerometer.setAccelScale(0); // Scale: AFS_2G - 0

    pinMode(LED, OUTPUT);
    
    delay(50); // Allow time for sensor initialization to complete

    for(int i = 0; i < 3; i++){ // Calibration period of 3 seconds
        startTime = millis();

        accelRaw = accelerometer.getRawAccel();
        accelG = accelerometer.getGAccel(accelRaw);
        accelMPS = accelerometer.getMPSAccel(accelG);
        
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
    Serial.print(F("Calculated Static Acceleration: ")); Serial.println(staticAcceleration);
};

void loop(){

    //startTime = millis();

    accelRaw = accelerometer.getRawAccel();
    accelG = accelerometer.getGAccel(accelRaw);
    accelMPS = accelerometer.getMPSAccel(accelG);

    Serial.print("MPS, X-Axis: ");
    Serial.println(accelMPS.x, 8);
    Serial.print("MPS, Y-Axis: ");
    Serial.println(accelMPS.y, 8);
    Serial.print("MPS, Z-Axis: ");
    Serial.println(accelMPS.z, 8);
    Serial.println();

    dynamicAcceleration = sqrt(pow(accelMPS.x,2) + pow(accelMPS.y,2) + pow(accelMPS.z,2));

    accelDif = abs(dynamicAcceleration - staticAcceleration);

    Serial.print(F("Calculated Static Acceleration: ")); Serial.println(accelDif);

    if( accelDif >= impactThreshold){
        Serial.println("Impact detected");

        digitalWrite(LED, HIGH);
        delay(500);
        digitalWrite(LED, LOW);
    };
    
    //endTime = millis();
    delay(100); // get measurements every 0.1 second
};