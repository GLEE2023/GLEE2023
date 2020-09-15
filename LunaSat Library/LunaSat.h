/*

This Header file is a part of the LunaSat library. This library will provide LunaSats
with the classes and functions necessary to initalize and communicate with both LunaSats
and their sensors.


Author: John Walker Johnson
Contributors:
Date Created: September 10th 2020
Date Revised:September 11th 2020

*/

/*
The LunaSat class is used to create each LunaSat as its own unique object.
Within this class, there are functions used to initalize the lunaSat, query its
sensors, and communicate with the LunaSat
*/

#include <Arduino.h>

class LunaSat {

    public:
        LunaSat(string LunaSatName);      // LunaSat Constructors
        void getData();
    private:
        string myName;




};

/*
The Sensor class is used a parent class in order to reduce redundant,
universal funnctions required for I2C communication and LunaSat performance


*/

class Sensor {
    public:
        bool isConnected(void);
        void whoAmI(void);
        void setMode(uint8_t sensorModeRegister, uint8_t sensorModeData);
        

    protected:
        string sensorName;
        uint8_t sensorAddress;
        bool sensorDebug;
        uint8_t readByte(uint8_t registerAddress);
        void writeByte(uint8_t registerAddress, uint8_t writeData);
        




}
