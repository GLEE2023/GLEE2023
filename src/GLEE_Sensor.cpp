/**
 * Sensor Library Function Definitions
 * All Functions are common functions shared among I2C sensors
**/

#include "GLEE_Sensor.h"

/**
 * Function: Sensor.isConnected
 * inputs: none
 * outputs: boolean value.
 * returns true if we can communicate with sensor
 * returns false if we cannot communicate with sensor or any other error occurs
**/
bool Sensor::isConnected(void){
    uint8_t statusByte;
    Wire.beginTransmission(Sensor::info.address);
    statusByte = Wire.endTransmission();
    
    if (statusByte==0){
        // if (Sensor::sensorDebug){
        //     Serial.print(F"Sucessful Connection with: ");
        //     Serial.print(Sensor::info.name);
        //     Serial.print(F" at address: ");
        //     Serial.println(Sensor::info.address,HEX);
        // }
        return true;
    }

    else {
        if (Sensor::sensorDebug){
            // Serial.print("ERROR: UNABLE TO CONNECT TO ");
            // Serial.println(Sensor::info.name);
		    // Serial.print("Status Byte = ");
		    // Serial.println(statusByte);    
        }
        return false;
    }
}

/**
 * Function: Sensor Who Am I 
 * inputs: none
 * outputs: None. currently will print sensor details to serial monitor
**/
void Sensor::whoAmI(void){
    Serial.print(F("Name: "));
    Serial.println(Sensor::info.name);
    Serial.print(F("I2C address is"));
    Serial.println(Sensor::info.address,HEX);
}

/**
 * Parameters: Debug Bool
 * Returns: None
 * This function stores debug mode bool locally
**/
void Sensor::setDebugMode(bool _debug){
    Sensor::sensorDebug = _debug;
}

// I2C Helper Functions 

/**
 * Parameters: Register Address
 * Returns: Read data byte
 * This function returns read data byte
**/
uint8_t Sensor::readByte(uint8_t registerAddress){
    uint8_t readByte;                                       // byte to store data that is read
    Wire.beginTransmission(Sensor::info.address);           // begins comms with sensor specified
    Wire.write(registerAddress);                            // identifies register for data to be read from
    Wire.endTransmission();                                 // end transmission
    Wire.requestFrom(Sensor::info.address, uint8_t (1) );   // request 1 byte from the sensor address
    readByte = Wire.read();                                 // read data and store in the readByte variable
    return readByte;                                        // return the read data byte
}

/**
 * Parameters: Register Address, nBytes, and data
 * Returns: Read None
 * This function stores read bytes in data array
**/
void Sensor::readBytes(uint8_t registerAddress, uint8_t nBytes, uint8_t * data){
    Wire.beginTransmission(Sensor::info.address);           // begins forming transmission to sensor
    Wire.write(registerAddress);                            // Add register address to transmission
    Wire.endTransmission();                                 
    Wire.requestFrom(Sensor::info.address, nBytes);         // Request and listen for response
    // Record response, wire will be available until nBytes are read
    int i = 0;
    while(Wire.available()){
        data[i] = Wire.read();
        i++;
    }
}

/**
 * Parameters: Register Address, Write Data
 * Returns: None
 * This function writes data to specified address
**/
void Sensor::writeByte (uint8_t registerAddress, uint8_t writeData){
    Wire.beginTransmission(Sensor::info.address);               // begin communication with the sensor 
    Wire.write(registerAddress);                                // point to address to be written to
    Wire.write(writeData);                                      // write data to adress specificed above
    Wire.endTransmission();                                     // end communication
}

/**
 * Parameters: Register Address, Starting Bit, Length, Data
 * Returns: None
 * This function  writes data byte for specified length
**/
void Sensor::writeBits(uint8_t registerAddress, uint8_t startBit, uint8_t length, uint8_t data){
    uint8_t buff; 
    if (readByte(registerAddress) != 0){
        uint8_t mask = ((1 << length) - 1) << (startBit - length + 1);
        data <<= (startBit - length + 1); 
        data &= mask;
        buff &= ~(mask);
        buff |= data;
        writeByte(registerAddress, buff);
    } else {
	//TODO
    }
}
