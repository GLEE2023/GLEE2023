/*

This cpp file is a part of the LunaSat library. This file provides the functions necessary
for the utilization the LunaSat library and vital for the operation of the LunaSat.
This project was created for the Great Lunar Expedition for Everyone (GLEE), a distributed
lunar science project by the Colorado Space Grant Consortium.


Author: John Walker Johnson
Contributors:
Date Created: September 10th 2020
Date Revised:September 11th 2020

*/

#include "LunaSat.h"

LunaSat::LunaSat(string lunaSatName){   //Constructor: Names LunaSat and Creates Object
    LunaSat.myName=lunaSatName;

}

LunaSat::getData(){
                                //this function will query and store sensor data
}


/*
Sensor Library Function Definitions
All Functions are common functions shared among I2C sensors
*/

/*
Function: Sensor.isConnected
inputs: none
outputs: boolean value.
returns true if we can communicate with sensor
returns false if we cannot communicate with sensor or any other error occurs
*/

bool Sensor::isConnected(void){
    uint8_t statusByte
    Wire.beginTransmission(Sensor.sensorAddress);
    statusByte = Wire.endTransmission();
    
    if (statusByte==0){
        if (Sensor.sensorDebug){
            Serial.print("Sucessful Connection with: ");
            Serial.print(Sensor.sensorName);
            Serial.print(" at address: ");
            Serial.println(Sensor.sensorAddress,HEX);
        }
        return true;
    }

    else {
        if (Sensor.sensorDebug){
            Serial.print("ERROR: UNABLE TO CONNECT TO ");
            Serial.println(Sensor.sensorName);
		    Serial.print("Status Byte = ");
		    Serial.println(statusByte);    
        }
        return false;
    }
}

/*
Function: Sensor.whoAmI
inputs: none
outputs: None. currently will print sensor details to serial monitor
*/

void Sensor::whoAmI(void){
    Serial.print("My sensor name is: ");
    Serial.println(Sensor.sensorName);
    Serial.print("My sensor I2C address is");
    Serial.println(Sensor.sensorAddress,HEX);
}

void Sensor::setMode(uint8_t sensorModeRegister, uint8_t sensorModeData){
    Sensor.writeByte(sensorModeRegister,sensorModeData);
}

uint8_t Sensor::readByte(uint8_t registerAddress){
    uint8_t readByte;                                       //byte to store data that is read
    Wire.beginTransmission(Sensor.sensorAddress);           //begins comms with sensor specified
    Wire.write(registerAddress);                            //identifies register for data to be read from
    Wire.endTransmission();                                 //end transmission
    Wire.requestFrom(Sensor.sensorAddress,1);               //request 1 byte from the sensor address
    readByte = Wire.read();                                 //read data and store in the readByte variable
    return readByte;                                        //return the read data byte
}

void Sensor::writeByte (uint8_t registerAddress, uint8_t writeData){
    Wire.beginTransmission(Sensor.sensorAddress);               //begin communication with the sensor 
    Wire.write(registerAddress);                                //point to address to be written to
    Wire.write(writeData);                                      //write data to adress specificed above
    Wire.endTransmission();                                     //end communication
}

