#include "BM1422AGMV_Lunar.h"

/*
config string passed into setConfig
-Mag (0-2)
3   = on or off, 0 is off
2   = continuous:0 or single mode:1
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
    // ((config & 0x8)<<4) | (0x40) | ((config & 0x3)<<3) | ((config & 0x4)<<1)
    uint8_t on_or_off = ((config>>3) & 0x1);
    uint8_t cont_or_single = ((config>>2) & 0x1);
    uint8_t cont_freq = (config & 0x3);

    Serial.print(F("BM14 CONFIG STR: 0x")); Serial.print(config,HEX); Serial.print(F(", CONTR1 val (hex):")); Serial.println(((config & 0x8)<<4) | (0x40) | ((config & 0x3)<<3) | ((config & 0x4)<<1),HEX);


    //Is it set on or off
    switch (on_or_off){
      case 0x0:
        Serial.print(F("BM14 on or sleep: ")); Serial.println(F("SLEEP (The following BM14 prints don't matter)"));
        break;
      case 0x1:
        Serial.print(F("BM14 on or sleep: ")); Serial.println(F("ON"));
        break;
      default:
        Serial.print(F("BM14 on or sleep: ")); Serial.println(F("Something went wrong."));
        break;
    };

    // check if mag is set to single mode or continuous mode
    switch (cont_or_single){
      case 0x0:
        Serial.print(F("BM14 continuous or single: ")); Serial.println(F("continuous"));
        break;
      case 0x1:
        Serial.print(F("BM14 continuous or single: ")); Serial.println(F("single"));
        break;
      default:
        Serial.print(F("BM14 continuous or single: ")); Serial.println(F("sonthing went wrong"));
        break;
    };

    //print freq of continuous mode
    switch (cont_freq){
      case 0x0:
        Serial.print(F("BM14 confinuous freq:")); Serial.println(F("10Hz"));
        break;
      case 0x1:
        Serial.print(F("BM14 confinuous freq:")); Serial.println(F("100Hz"));
        break;
      case 0x2:
        Serial.print(F("BM14 confinuous freq:")); Serial.println(F("20Hz"));
        break;
      case 0x3:
        Serial.print(F("BM14 confinuous freq:")); Serial.println(F("1kHz"));
        break;
      default:
        Serial.print(F("BM14 confinuous freq:")); Serial.println(F("Somthing went wrong"));
        break;
    };
  #endif

  //see page 16 in the BM1422AGMV data sheet for steps to set mode
  //step 1 (the 0x40 sets it to 14bit output)
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

void BM1422AGMV_Lunar::convertToFloats(uint8_t* buffer, float* nums){
  nums[0] = (float)(((signed short)buffer[1] << 8) | (buffer[0])) / BM1422AGMV_14BIT_SENS;
  nums[1] = (float)(((signed short)buffer[3] << 8) | (buffer[2])) / BM1422AGMV_14BIT_SENS;
  nums[2] = (float)(((signed short)buffer[5] << 8) | (buffer[4])) / BM1422AGMV_14BIT_SENS;
}
