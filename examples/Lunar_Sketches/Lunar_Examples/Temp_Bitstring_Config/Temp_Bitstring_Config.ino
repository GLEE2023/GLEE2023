/*
Bitstring to define config of Temperature Sensor
Bits from LSB to MSB:
(bits): description
5-6: mode
  modes:
    00: Continuous conversion (CC)
    01: Shutdown (SD)
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
0-1: Averaging
    00: No averaging
    01: 8 Averaged conversions
    10: 32 averaged conversions
    11: 64 averaged conversions
*/

#include <Wire.h>
#include "Lunar_I2C.h"

#define TMP_CONFIG_STRING 0b0111011

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


    //put most sig byte in 0th spot beacuase that is how its written to TMP
    uint8_t buffer[2];
    buffer[1] =  ((uint8_t)TMP_CONFIG_STRING << 5);
    buffer[0] =  ((uint8_t)(TMP_CONFIG_STRING>>3));
    Serial.println("Buffer: ");
    Serial.println(buffer[0],HEX);
    Serial.println(buffer[1],HEX);


    Lunar_I2C::writeBytes(TMP117_TEMP_I2C,TMP117_CONFIG_REG,2,buffer);
    //Serial.println(result, BIN);
    // Wire.beginTransmission(TMP117_TEMP_I2C);
    // Wire.write(TMP117_CONFIG_REG);
    // Wire.write((uint8_t)(result >> 8));
    // Wire.write((uint8_t)result);
    // Wire.endTransmission();

    //Serial.println("setup done");
    uint8_t newBuffer[2];
    Lunar_I2C::readBytes(TMP117_TEMP_I2C,TMP117_CONFIG_REG,2,newBuffer);
    Serial.println("Read from buffer: ");
    Serial.println(newBuffer[0],HEX);
    Serial.println(newBuffer[1],HEX);
}

void loop(){

    uint8_t buffer[2];
    Lunar_I2C::readBytes(TMP117_TEMP_I2C, TMP117_TEMP_REG, 2, &buffer[0]);
    float temp = (float)(buffer[0] << 8 | buffer[1]) * TMP117_RESOLUTION;

    Serial.println(String(temp));
    //delay(500);
}
