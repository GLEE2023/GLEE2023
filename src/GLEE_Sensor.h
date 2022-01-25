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
The Sensor class is used a parent class in order to reduce redundancy among,
universal funnctions required for I2C communication and LunaSat performance
*/

#ifndef GLEE_SENSOR_H
#define GLEE_SENSOR_H

#include <Wire.h>
#include <string.h>
#include <Arduino.h>

#define SENSOR_GRAVITY (9.80665F)
#define SENSOR_CONV_K_to_C (273.15F)


typedef struct{
    uint8_t id;
    uint8_t address;
    String name; // Might want to stray away from the string in this struct for better packing
} sensor_info_t;

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

typedef struct{
    int16_t x;
    int16_t y;
    int16_t z;
} sensor_int16_vec_t;


class Sensor {
    public:
        bool isConnected(void); //Using the wire interface we check to see if we can communicate with the sensor
        void whoAmI(void); //Returns info about the sensor!
        bool sensorDebug; //Stores sensor debug
        void setDebugMode(bool); //Set Debug Mode
        uint8_t readByte(uint8_t registerAddress); //Read Bytes
        void readBytes(uint8_t registerAddress, uint8_t len, uint8_t *data); //Read Bytes of specified Length
        void writeByte(uint8_t registerAddress, uint8_t writeData); //Write Bytes
        void writeBits(uint8_t registerAddress, uint8_t startBit, uint8_t length, uint8_t data); //UNFINISHED: Writes Bits with Specified Length
        sensor_info_t info; //Initialize Info variable 

    private:
        bool debug; //Debug Variable
};
#endif
