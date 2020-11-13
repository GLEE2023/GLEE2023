/*
This Header file is a part of the LunaSat library. This library will provide LunaSats
with the classes and functions necessary to initalize and communicate with both LunaSats
and their sensors.
Author: John Walker Johnson
Contributors:
Date Created: September 10th 2020
Date Revised: October 7th 2020
*/


/*
The Sensor class is used a parent class in order to reduce redundant,
universal funnctions required for I2C communication and LunaSat performance
*/

#ifndef GLEE_SENSOR_H
#define GLEE_SENSOR_H

#include <Wire.h>
#include <string.h>
#include <Arduino.h>


class Sensor {
    public:
        bool isConnected(void);             //Using the wire interface we check to see if we can communicate with the sensor
        void whoAmI(void);                  //Returns info about the sensor!
        void setDebugMode(bool);
        String sensorName;
        uint8_t sensorAddress;
        bool sensorDebug;
        uint8_t readByte(uint8_t registerAddress);
        void writeByte(uint8_t registerAddress, uint8_t writeData);
};


typedef struct{
    float x;
    float y;
    float z;
} sensor_float_vec_t;

typedef struct{
    uint16_t x;
    uint16_t y;
    uint16_t z;
} sensor_uint16_vec_t;


#endif