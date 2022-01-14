# GLEE Sensor Library

This is a library houses standard functionality for sensors used by GLEE. Common types for sensor configuration and data enables efficent control when working with different kinds of sensors. The sensor parent class is currently utilized in the following child directories.

## Children (Sensor) Libraries: ##
* GLEE_TMP117 - Temperature Sensor 
* GLEE_MLX90393 - Magnetometer
* GLEE_MPU6000 - IMU
* GLEE_TPIS1385 - IR Temperature Sensor
* GLEE_CAP - Capacitive Sensor

# Structure # 
## Data Types
* Info 
    ```c++ 
    sensor_info_t // {uint8_t id, uint8_t address, String name}
    ```
    - Id: Identification number
    - Address: I2C address
    - Name: String representation of sesnor name
    
* Vectors
    ```c++
    sensor_float_vec_t // {float x, float y, float z}
    sensor_uint16_vec_t // {uint16_t x, uint16_t y, uint16_t z}
    sensor_int16_vec_t // {int16_t x, int16_t y, int16_t z}
    ```
    - X, Y, Z packaging for common usecases

## Physics Constants ##
```C++
SENSOR_GRAVITY (9.80665F) // 9.80665 m/s^2
SENSOR_CONV_K_to_C (273.15F) // Conversion factor deg C = deg K - 273.15
```
