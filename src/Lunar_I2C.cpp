#include "Lunar_I2C.h"
/**
* Parameters: I2C address, Register Address, Bytes to read, buffer to read to
* Returns: None
* This function reads data from a specified I2c address, and specific register
**/
void Lunar_I2C::readBytes(uint8_t I2CsensorAddress, uint8_t registerAddress, uint8_t nBytes, uint8_t * data){
  Wire.beginTransmission(I2CsensorAddress);           // begins forming transmission to sensor
  Wire.write(registerAddress);                     // Add register address to transmission
  Wire.endTransmission(false);
  Wire.requestFrom(I2CsensorAddress, nBytes, true);         // Request and listen for response
  // Record response, wire will be available until nBytes are read
  int i = 0;

  #ifdef I2C_DEBUG
    Serial.print(F("Lunar_I2C::readBytes reading ")); Serial.print(nBytes); Serial.println(F(" bytes of data (hex): "));
  #endif

  while(Wire.available()){
    #ifdef I2C_DEBUG //for debugging
      Serial.println(data[i],HEX);
    #endif

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
  #ifdef I2C_DEBUG
    Serial.print(F("Lunar_I2C::writeBytes() writing (hex): "));
    Serial.println(writeData);
  #endif

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

  #ifdef I2C_DEBUG //debugging
    Serial.print(F("Lunar_I2C::writeBytes writiing ")); Serial.print(nBytes); Serial.println(F(" bytes of data (hex): "));
  #endif

  for(int i=0; i<nBytes; i++){
    #ifdef I2C_DEBUG //for debugging
      Serial.print(F("Byte num: "));Serial.print(i); Serial.print(": "); Serial.println(data[i],HEX);
    #endif

    Wire.write(data[i]);                                      // write data to adress specificed above
  }
  Wire.endTransmission();                                     // end communication
}
