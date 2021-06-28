#include "ICM20602.h"

ICM20602 accelerometer(1, false);
sensor_float_vec_t accelG; 

void setup(){
    Serial.begin(9600);

    accelerometer.begin();

    accelerometer.initialize();

    accelerometer.setAccelScale(AFS_2G);
};

void loop(){
    
    accelG = accelerometer.getSample();

    Serial.print(accelG.x); Serial.print(",");
    Serial.print(accelG.y); Serial.print(",");
    Serial.println(accelG.z);
    
    delay(100);
};
