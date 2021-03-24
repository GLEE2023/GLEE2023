// GLEE 2023 Module 5: Acceleration - Basic acceleration output via serial in Gs

#include <Arduino.h>
#include "ICM20602.h"

float startTime;    
float endTime;

ICM20602 accelerometer(2,false); // Initialize acclerometer with ID of 2
 
sensor_int16_vec_t accelRaw;
// sensor_float_vec_t accelMPS; 
sensor_float_vec_t accelG; 

void setup(){
    // Begin Serial Communications
    Serial.begin(9600);

    // Begin communications with and initialize the accelerometer
    accelerometer.begin();

    accelerometer.initialize();

    accelerometer.setScale(AFS_2G);
    
    // Print Header row for line seperated data
    Serial.println("X-Axis (Gs), Y-Axis(Gs), Z-Axis(Gs)");
};

void loop(){
    // Loop for printing acceleration reading every second
  
    startTime = millis(); // Record start time for adjustment for processing time

    accelRaw = accelerometer.getRawAccel();
    accelG = accelerometer.getGAccel(accelRaw);
        
    
    Serial.print(accelG.x, 8);
    Serial.print(",");
    Serial.print(accelG.y, 8);
    Serial.print(",");
    Serial.println(accelG.z, 8);
    
    endTime = millis();
    
    delay(1000 - (endTime - startTime)); // Delay of 1000s between each readings
};
