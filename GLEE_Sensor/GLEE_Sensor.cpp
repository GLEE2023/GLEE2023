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

#include "GLEE_Sensor.h"

bool Sensor::isConnected(void)
{
    uint8_t statusByte;
    Wire.beginTransmission(Sensor::info.address);
    statusByte = Wire.endTransmission();
    
    if (statusByte==0){
        if (Sensor::sensorDebug){
            Serial.print("Sucessful Connection with: ");
            Serial.print(Sensor::info.name);
            Serial.print(" at address: ");
            Serial.println(Sensor::info.address,HEX);
        }
        return true;
    }

    else {
        if (Sensor::sensorDebug){
            Serial.print("ERROR: UNABLE TO CONNECT TO ");
            Serial.println(Sensor::info.name);
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
    Serial.println(Sensor::info.name);
    Serial.print("My sensor I2C address is");
    Serial.println(Sensor::info.address,HEX);
}

void Sensor::setDebugMode(bool debug){
    Sensor::sensorDebug = debug;
}

uint8_t Sensor::readByte(uint8_t registerAddress){
    uint8_t readByte;                                       //byte to store data that is read
    Wire.beginTransmission(Sensor::info.address);           //begins comms with sensor specified
    Wire.write(registerAddress);                            //identifies register for data to be read from
    Wire.endTransmission();                                 //end transmission
    Wire.requestFrom(Sensor::info.address, uint8_t (1) );   //request 1 byte from the sensor address
    readByte = Wire.read();                                 //read data and store in the readByte variable
    return readByte;                                        //return the read data byte
}

void Sensor::writeByte (uint8_t registerAddress, uint8_t writeData){
    Wire.beginTransmission(Sensor::info.address);               //begin communication with the sensor 
    Wire.write(registerAddress);                                //point to address to be written to
    Wire.write(writeData);                                      //write data to adress specificed above
    Wire.endTransmission();                                     //end communication
}