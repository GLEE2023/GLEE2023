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
        LunaSat(uint8_t idNumber,string LunaSatName);      // LunaSat Constructors
        void getData();
        void setMode();

    private:
        string myName;
        uint8_t myID;
        void initalize(void);


};

