/*
Bitstring to define config of Temperature Sensor
Bits from LSB to MSB:
(bits): description
0-1: mode
  modes:
    01: Shutdown (SD)
    00: Continuous conversion (CC)
    10: Continuous conversion (CC), Same as 00 (reads back = 00)
    11: One-shot conversion (OS)
2-4: Conversion Time
    000
    001
    010
    011
    100
    101
    110
    111
5-6: Averaging
    00: No averaging
    01: 8 Averaged conversions
    10: 32 averaged conversions
    11: 64 averaged conversions
*/

#include <Wire.h>
#include "Lunar_I2C.h"

#define TMP_CONFIG_STRING 0b0010100

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
    delay(1000);

    uint16_t first = 0b0010;
    uint16_t second = ((uint16_t)TMP_CONFIG_STRING);
    uint16_t result = ((0b0010 << 12) | (TMP_CONFIG_STRING << 5));

    //Serial.println(result, BIN);
    Wire.beginTransmission(TMP117_TEMP_I2C);
    Wire.write(TMP117_CONFIG_REG);
    Wire.write((uint8_t)(result >> 8));
    Wire.write((uint8_t)result);
    Wire.endTransmission();

    //Serial.println("setup done");
}

void loop(){

    uint8_t buffer[2];
    Lunar_I2C::readBytes(TMP117_TEMP_I2C, TMP117_TEMP_REG, 2, &buffer[0]);
    float temp = (float)(buffer[0] << 8 | buffer[1]) * TMP117_RESOLUTION;

    Serial.println(String(temp));
    //delay(500);
}
