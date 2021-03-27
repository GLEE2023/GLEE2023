#include <Arduino.h>
#include "GLEE_Sensor.h"
#include "ICM20602.h"

float startTime;    
float endTime;

ICM20602 gyroscope(2);

sensor_int16_vec_t angVelRaw;
sensor_float_vec_t angVelDPS; 

void setup(){
    Serial.begin(9600);

    gyroscope.begin();

    gyroscope.initialize();

    gyroscope.setGyroScale(GFS_250DPS);
};

void loop(){
    angVelRaw = gyroscope.getRawAngVel();
    angVelDPS = gyroscope.getDPSAngVel(angVelRaw);

    Serial.print("Angular Velocity in Gs, X-Axis: "); Serial.print(angVelDPS.x, 8);
    Serial.print(" Y-Axis: "); Serial.print(angVelDPS.y, 8);
    Serial.print(" Z-Axis: "); Serial.print(angVelDPS.z, 8);
    Serial.println();
    
    delay(1000);
};