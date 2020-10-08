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

#include <Wire.h>


class Sensor {
    public:
        bool isConnected(void);             //Using the wire interface we check to see if we can communicate with the sensor
        void whoAmI(void);                  //Returns info about the sensor!
        

    protected:
        string sensorName;
        uint8_t sensorAddress;
        bool sensorDebug;
        uint8_t readByte(uint8_t registerAddress);
        void writeByte(uint8_t registerAddress, uint8_t writeData);
        
}
