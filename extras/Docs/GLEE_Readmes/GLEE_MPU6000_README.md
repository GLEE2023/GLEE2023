# GLEE - LunaSat Inertial Measurement Unit Library (Accelerometer and Gyroscope) - GLEE_MPU6000
This library is intended for GLEE's Accelerometer Sensor, the MPU6000. 
Paired with GLEE's LunaSat object class, it can be used to monitor, configure and control 
the MPU6000. GLEE_MPU6000 supports the basic functionality of the accelerometer and the gyroscope with reading 
and saving the three-axis acceleration and angular velocity in different units depending on one of four sensing 
accuracies. 

![ICM20602](/Docs/Images/ICM20602_close_up.jpg)

## Accelerometer Datasheet
[InvenSense MPU6000](https://product.tdk.com/system/files/dam/doc/product/sensor/mortion-inertial/imu/data_sheet/mpu-6000-datasheet1.pdf)

## Examples
# Accelerometer- outputting the acceleration in G's for the x, y, and z axes
```C++
#include "MPU6000.h"

MPU6000 accelerometer(1, false); // Sets sensor ID to 1 and debugging to false
sensor_float_vec_t acc; // Saves acceleration readings in a vector structure

void setup(){
    Serial.begin(9600); // Sets baud rate to 9600 for serial transmission 
                        // and starts serial communication
    accelerometer.begin(); // Begins transmission to the I2C slave device
    accelerometer.initialize(); // Set-up for MPU 
    accelerometer.setAccelRange(MPU6000_RANGE_2_G); // Sets range of acccelrometer 
                                                    // Range options: 2_G, 4_G, 8_G, 16_G
};

void loop(){
    acc = accelerometer.getSample(); // Gets and saves 3-axis acceleration reading (G)

    Serial.print("Acceleration in Gs, X-Axis: "); Serial.print(acc.x, 8); // Prints out 3-axis acceleration (G)
    Serial.print(" Y-Axis: "); Serial.print(acc.y, 8);
    Serial.print(" Z-Axis: "); Serial.print(acc.z, 8);
    Serial.println();
    
    delay(500); // Waits half a second
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
| disableGyro | void | bool disableGyro | write configuration of gyroscope depending on whether the gyroscope is getting disabled or enabled
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
