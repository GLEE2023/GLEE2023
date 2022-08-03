#include "TMP117_Lunar.h"

void TMP117_Lunar::setConfig(uint8_t config){
  // CONFIG_PRINT_STATEMENT
  #ifdef TMP_DEBUG
    Serial.print(F("TMP CONFIG STR: 0x")); Serial.println(config,HEX);
  #endif

  uint8_t buffer[2];
  buffer[1] =  ((uint8_t)config<<5);
  buffer[0] =  ((uint8_t)(config>>3));

  Lunar_I2C::writeBytes(TMP117_TEMP_I2C, TMP117_CONFIG_REG, 2, buffer);
}

void TMP117_Lunar::getData(uint8_t* buffer){
  Lunar_I2C::readBytes(TMP117_TEMP_I2C, TMP117_TEMP_REG, 2, buffer);
}

float TMP117_Lunar::convertToTempC(uint8_t* buffer, float* temperature){
  *temperature =  (float)(buffer[0] << 8 | buffer[1]) * TMP117_RESOLUTION;
}
