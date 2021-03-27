#include "ICM20602.h"

ICM20602 accelerometer(1);
sensor_int16_vec_t accelRaw;
sensor_float_vec_t accelG; 

void setup(){
    Serial.begin(9600);

    accelerometer.begin();

    accelerometer.initialize();

    accelerometer.setScale(AFS_2G);
};

void loop(){
    accelRaw = accelerometer.getRawAccel();
    accelG = accelerometer.getGAccel(accelRaw);

    Serial.print("Acceleration in Gs, X-Axis: "); Serial.print(accelG.x, 8);
    Serial.print(" Y-Axis: "); Serial.print(accelG.y, 8);
    Serial.print(" Z-Axis: "); Serial.print(accelG.z, 8);
    Serial.println();
    
    delay(1000);
};
