#include "BM1422AGMV_Lunar.h"

/*
-Mag (0-2)
3   = on or off, 0 is off
2   = continuous or single mode
0-1 = continuous mode freq
*/

void BM1422AGMV_Lunar::setConfig(uint8_t config){
  /*
  contr1 register
  7 - power (1 on, 0 off);
  6 - out bit (0: 12 bit, 1: 14 bit )
  5 - rst: logic reset control
  3-4 ODR: measurment output rate (00:10Hz, 10: 20Hz, 01:100Hz, 11:1KHz) odd numbering, but it is correct
  1 - FS1: 0: Continuous, 1: single mode
  */

  #ifdef BM1422AGMV_DEBUG
    Serial.print(F("BM14 CONFIG STR: 0x")); Serial.print(config,HEX); Serial.print(F(", CONTR1 val (hex):")); Serial.println(((config & 0x8)<<4) | (0x40) | ((config & 0x3)<<3) | ((config & 0x4)<<1),HEX);
  #endif

  //see page 16 in the BM1422AGMV data sheet for steps to set mode
  //step 1
  Lunar_I2C::writeByte(BM1422AGMV_DEVICE_ADDRESS_0E,BM1422AGMV_CNTL1,((config & 0x8)<<4) | (0x40) | ((config & 0x3)<<3) | ((config & 0x4)<<1));
  Lunar_I2C::writeByte(BM1422AGMV_DEVICE_ADDRESS_0E,BM1422AGMV_CNTL4,0x00);
  Lunar_I2C::writeByte(BM1422AGMV_DEVICE_ADDRESS_0E,BM1422AGMV_CNTL4+1,0x00);
  //step2
  Lunar_I2C::writeByte(BM1422AGMV_DEVICE_ADDRESS_0E,BM1422AGMV_CNTL2,0x0C);
  //step 3 is to write offset values, not sure this is necessary

  //step4
  Lunar_I2C::writeByte(BM1422AGMV_DEVICE_ADDRESS_0E,BM1422AGMV_CNTL3,BM1422AGMV_CNTL3_FORCE); //starts measure

}

void BM1422AGMV_Lunar::getData(uint8_t* buffer){
  Lunar_I2C::writeByte(BM1422AGMV_DEVICE_ADDRESS_0E,BM1422AGMV_CNTL3, 0x04);
  // delay(10);
  Lunar_I2C::readBytes(BM1422AGMV_DEVICE_ADDRESS_0E,BM1422AGMV_DATAX, 6, buffer);
}
