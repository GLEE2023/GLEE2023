# GLEE_ICM20602
This library is intended for GLEE's Accelerometer Sensor, the ICM20602. 
Paired with GLEE's LunaSat object class, it can be used to monitor, configure and control 
the ICM20602. GLEE_ICM20602 supports the basic functionality of the accelerometer with reading 
and saving the three-axis acceleration in different units depending on one of four sensing 
accuracies. However, the gyroscope is currently disabled as the GLEE team currently does not 
have a need for it.

## Baseline Configuration Registers
| Register Name | Register Value (Hex) | Comments  |
|---|---|---|
| ICM20602_slave_addr | 0x69 | I2C Slave address|
| ICM20602_PWR_MGMT_1 | 0x6B | Power management 1, see "Configuration Details below" |
| ICM20602_PWR_MGMT_2 | 0x6C | Power management 2 , see "Configuration Details below" |
| ICM20602_CONFIG | 0x1A | Configuration, FIFO mode and more |
| ICM20602_GYRO_CONFIG | 0x1B | Gyroscope Configuration |
| ICM20602_ACCEL_CONFIG | 0x1C | Accelerometer Configuration |

## Methods 
| Method Name | return type | args | Comments |
|---|---|---|---|
| begin | bool | none | Returns true or false based on whether communication was successful with the sensor |
| initialize | void | none | Sets the clock to auto, disables the gyroscope, and sets the configuration registrations |
| getSensitivity | float | enum Ascale scaleA | Returns the sensitivity scale factor depending on the current sensing accuracy scale |
| getRawAccel | sensor_uint16_vec_t | none | Converts the raw acceleration in LSB/G to the acceleration in G's |
| getMPSAccel | sensor_float_vec_t | none | Converts the raw accelerations in LSB/G to meters per second squared |
| getGAccel | sensor_float_vec_t | enum Ascale scaleA | converts the raw acceleration in LSB/G to the acceleration in G's |

## Configuration Details

| Configuration Name | Register Name | Function | Setting |
|---|---|---|---|
| Default PWR MGMT 1 Configuration | ICM20602_PWR_MGMT_1 | DEVICE_RESET, SLEEP, CYCLE, GYRO_STANDBY, TEMP_DIS, CLKSEL | 0 0 0 0 0 001 |
| Default PWR MGMT 2 Configuration | ICM20602_PWR_MGMT_2 | Reserved, Reserved, STBY_XA, STBY_YA, STBY_ZA, STBY_XG, STBY_YG, STBY_Z | 0 0 1 1 1 0 0 0 |
| Accel Full Scale Select | ACCEL_FS_SEL[1:0] | ±2g, ±4g, ±8g, ±16g | 00, 01, 10, 11 |




