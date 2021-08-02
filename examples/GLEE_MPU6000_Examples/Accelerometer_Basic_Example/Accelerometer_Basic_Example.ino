#include "MPU6000.h"

MPU6000 accelerometer(1, false);
sensor_float_vec_t acc; 

void setup(){
    Serial.begin(9600);

    accelerometer.begin();
    accelerometer.initialize();
    accelerometer.setAccelScale(MPU6000_RANGE_2_G);
};

void loop(){
    acc = accelerometer.getSample(); // Gets 3 axis acceleration reading (G)

    Serial.print("Acceleration in Gs, X-Axis: "); Serial.print(acc.x, 8);
    Serial.print(" Y-Axis: "); Serial.print(acc.y, 8);
    Serial.print(" Z-Axis: "); Serial.print(acc.z, 8);
    Serial.println();
    
    delay(500);
};
