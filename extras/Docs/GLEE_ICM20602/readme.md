# GLEE - LunaSat 5.0 Inertial Measurement Unit Library (Accelerometer and Gyroscope) - GLEE_ICM20602
This library is intended for GLEE's Accelerometer Sensor, the ICM20602. 
Paired with GLEE's LunaSat object class, it can be used to monitor, configure and control 
the ICM20602. GLEE_ICM20602 supports the basic functionality of the accelerometer and the gyroscope with reading 
and saving the three-axis acceleration and angular velocity in different units depending on one of four sensing 
accuracies. 

![ICM20602](/Docs/Images/ICM20602_close_up.jpg)

## Accelerometer Datasheet
[InvenSense ICM20602](https://invensense.tdk.com/wp-content/uploads/2016/10/DS-000176-ICM-20602-v1.0.pdf)

## Examples
# Accelerometer- outputting the acceleration in G's for the x, y, and z axes
```C++
#include "ICM20602.h"

ICM20602 accelerometer(1);
sensor_int16_vec_t accelRaw;
sensor_float_vec_t accelG; 

void setup(){
    Serial.begin(9600);

    accelerometer.begin();

    accelerometer.initialize();

    accelerometer.setAccelScale(AFS_2G);
};

void loop(){
    accelRaw = accelerometer.getRawAccel();
    accelG = accelerometer.getGAccel(accelRaw);

    Serial.print("Acceleration in Gs, X-Axis: "); 
    Serial.print(accelG.x, 8);
    Serial.print(" Y-Axis: "); 
    Serial.print(accelG.y, 8);
    Serial.print(" Z-Axis: "); 
    Serial.print(accelG.z, 8);
    Serial.println();
    
    delay(1000);
};
```
# Gyroscope- outputting the angular velocity in degrees per second for the x, y, and z axes
```C++
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

    Serial.print("Angular Velocity in DPS, X-Axis: "); Serial.print(angVelDPS.x, 8);
    Serial.print(" Y-Axis: "); Serial.print(angVelDPS.y, 8);
    Serial.print(" Z-Axis: "); Serial.print(angVelDPS.z, 8);
    Serial.println();
    
    delay(1000);
};
```

## Baseline Configuration Registers
| Register Name | Register Value (Hex) | Comments  |
|---|---|---|
| ICM20602_slave_addr | 0x69 | I2C Slave address|
| ICM20602_PWR_MGMT_1 | 0x6B | Power management 1, see "Configuration Details" below |
| ICM20602_PWR_MGMT_2 | 0x6C | Power management 2 , see "Configuration Details" below |
| ICM20602_CONFIG | 0x1A | Configuration, FIFO mode and more |
| ICM20602_GYRO_CONFIG | 0x1B | Gyroscope Configuration |
| ICM20602_ACCEL_CONFIG | 0x1C | Accelerometer Configuration |

## Configuration Details
| Configuration Name | Register Name | Function of Each Bit | Setting |
|---|---|---|---|
| Default PWR MGMT 1 Configuration | ICM20602_PWR_MGMT_1 | DEVICE_RESET, SLEEP, CYCLE, GYRO_STANDBY, TEMP_DIS, CLKSEL | 0 0 0 0 0 001 |
| Default PWR MGMT 2 Configuration | ICM20602_PWR_MGMT_2 | Reserved, Reserved, STBY_XA, STBY_YA, STBY_ZA, STBY_XG, STBY_YG, STBY_Z | 0 0 1 1 1 0 0 0 |
| Accel Full Scale Select | ACCEL_FS_SEL[1:0] | ±2g, ±4g, ±8g, ±16g | 00, 01, 10, 11 |

## Methods 
| Method Name | return type | args | Comments |
|---|---|---|---|
| begin | bool | none | Returns true or false based on whether communication was successful with the sensor |
| initialize | void | none | Sets the clock to auto and sets the configuration registrations |
| getRawAccel | sensor_uint16_vec_t | none | Gets and returns the current raw acceleration in LSB/G |
| getRawAngVel | sensor_int16_vec_t | none | Gets and returns the current raw angular velocity |
| getMPSAccel | sensor_float_vec_t | sensor_float_vec_t GAccel | Converts the raw acceleration in LSB/G to meters per second squared |
| getGAccel | sensor_float_vec_t | sensor_int16_vec_t rawAccel | Converts the raw acceleration in LSB/G to the acceleration in G's |
| getDPSAngVel | sensor_float_vec_t | sensor_int16_vec_t rawAngVel | Converts the raw angular velocity to degrees per second |
| updateRawAccel | none | sensor_int16_vec_t rawAccel | Saves the current raw acceleration into the private accelRaw struct |
| updateMPSAccel | none | sensor_float_vec_t MPSAccel | Saves the current meters per second acceleration into the private accelMPSstruct |
| updateGAccel | none | sensor_float_vec_t GAccel | Saves the current acceleration in G's into the private accelG struct |
| updateRawAngVel | none | sensor_int16_vec_t rawAngVel | Saves the current raw angular velocity into the private angVelRaw struct |
| updateDPSAngVel | none | sensor_float_vec_t DPSAngVel | Saves the current angular velocity in degrees per second into the private angVelDPS struct |
| setAccelScale | none | enum Ascale newScale | Sets the global variable 'currentAccelScale' to a new scale |
| setGyroScale | none | enum Gscale newScale | Sets the global variable 'currentGyroScale' to a new scale
| getAccelSensitivity | float | none | Returns the sensitivity scale factor depending on the current sensing accuracy scale given by the public variable 'currentAccelScale' |
| getGyroSensitivity | float | none | Returns the sensitivity scale factor depending on the current sensing accuracy scale given by the public variable 'currentGyroScale' |





