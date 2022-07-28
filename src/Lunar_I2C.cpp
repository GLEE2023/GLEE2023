#include "Lunar_I2C.h"
/**
* Parameters: I2C address, Register Address, Bytes to read, buffer to read to
* Returns: None
* This function reads data from a specified I2c address, and specific register
**/
void Lunar_I2C::readBytes(uint8_t I2CsensorAddress, uint8_t registerAddress, uint8_t nBytes, uint8_t * data){
  Wire.beginTransmission(I2CsensorAddress);           // begins forming transmission to sensor
  Wire.write(registerAddress);                     // Add register address to transmission
  Wire.endTransmission();
  Wire.requestFrom(I2CsensorAddress, nBytes);         // Request and listen for response
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
void Lunar_I2C::writeByte (uint8_t I2CsensorAddress, uint8_t registerAddress, uint8_t writeData){
  Wire.beginTransmission(I2CsensorAddress);               // begin communication with the sensor
  Wire.write(registerAddress);                                // point to address to be written to
  Wire.write(writeData);                                      // write data to adress specificed above
  Wire.endTransmission();                                     // end communication
}


/**
* Parameters: Register Address, Write Data
* Returns: None
* This function writes data to specified address
**/
void Lunar_I2C::writeBytes (uint8_t I2CsensorAddress, uint8_t registerAddress, uint8_t nBytes, uint8_t * data){
  Wire.beginTransmission(I2CsensorAddress);               // begin communication with the sensor
  Wire.write(registerAddress);                                // point to address to be written to
  for(int i=0; i<nBytes; i++){
    Serial.print("Writing to buffer: ");
    Serial.println(data[i],HEX);
    Wire.write(data[i]);                                      // write data to adress specificed above
  }
  Wire.endTransmission();                                     // end communication
}
