#include <Wire.h>
#include "Lunar_I2C.h"

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
#define TMP117_RESOLUTION (double)0.0078125



void setup(){
  Serial.begin(9600);
  Wire.begin();
}

void loop(){

  long timestamp = micros();

  uint8_t buffer[2];
  readBytes(TMP117_TEMP_I2C, TMP117_TEMP_REG, 2, &buffer[0]);

  float temp = (float)(buffer[0] << 8 | buffer[1]) * TMP117_RESOLUTION;

  timestamp = micros()-timestamp;
  Serial.println(String(timestamp)+" "+String(temp));
}
