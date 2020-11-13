#include <Arduino.h>
#include "GLEE_Sensor.h"
#include "ICM20602.h"

ICM20602 accelerometer = ICM20602();

void setup(){
    
    bool begin_ = accelerometer.begin();
    accelerometer.initialize();

    sensor_uint16_vec_t rawAcc_ = accelerometer.getRawAccel();
}

void loop(){

}