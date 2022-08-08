#include <Wire.h>
#include "Lunar_I2C.h"

// #define TMP_DEBUG 

#ifdef TMP_DEBUG
  #define CONFIG_PRINT_STATEMENT Serial.print(F("TMP CONFIG STR: 0x")); Serial.println(config,HEX);
#else
  #define CONFIG_PRINT_STATEMENT //none
#endif


#define TMP117_TEMP_I2C 0x48    // TMP I2C Register
#define TMP117_TEMP_REG 0x00    // TMP Data Register
#define TMP117_CONFIG_REG 0x01
#define TMP117_HIGH_LIMIT_REG 0X02
#define TMP117_LOW_LIMIT_REG 0X03
#define TMP117_EEPROM_UL_REG 0X04
#define TMP117_EEPROM1_REG 0X05
#define TMP117_EEPROM2_REG 0X06
#define TMP117_TEMP_OFFSET_REG 0X07
#define TMP117_EEPROM3_REG 0X08
#define TMP117_DEVICE_ID 0X0F
#define TMP117_RESOLUTION (float)0.0078125


namespace TMP117_Lunar{
  void setConfig(uint8_t config);
  void getData(uint8_t* buffer);
  float convertToTempC(uint8_t* buffer, float* temperature);
}
