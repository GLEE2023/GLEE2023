#include <Wire.h>

//this utilizes an anonamous namespace... Not exactly sure why this is needed, but the compiler gets mad if it isn't used
namespace {
  /**
  * Parameters: I2C address, Register Address, Bytes to read, buffer to read to
  * Returns: None
  * This function reads data from a specified I2c address, and specific register
  **/
  void readBytes(uint8_t I2CsensorAddress, uint8_t registerAddress, uint8_t nBytes, uint8_t * data){
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
  void writeByte (uint8_t I2CsensorAddress, uint8_t registerAddress, uint8_t writeData){
    Wire.beginTransmission(I2CsensorAddress);               // begin communication with the sensor
    Wire.write(registerAddress);                                // point to address to be written to
    Wire.write(writeData);                                      // write data to adress specificed above
    Wire.endTransmission();                                     // end communication
  }
}
