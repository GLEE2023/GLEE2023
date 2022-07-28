#include <Wire.h>
#include <Arduino.h>
//this utilizes an anonamous namespace... Not exactly sure why this is needed, but the compiler gets mad if it isn't used
namespace Lunar_I2C{
  void readBytes(uint8_t I2CsensorAddress, uint8_t registerAddress, uint8_t nBytes, uint8_t * data);

  void writeBytes(uint8_t I2CsensorAddress, uint8_t registerAddress, uint8_t nBytes, uint8_t * data);

  void writeByte(uint8_t I2CsensorAddress, uint8_t registerAddress, uint8_t writeData);
}
