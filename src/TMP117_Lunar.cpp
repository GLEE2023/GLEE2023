#include "TMP117_Lunar.h"

/*
5-6 = mode, CC:00 (or 11), shutdown:01, single shot:10
2-4 = conversion bits, see convTimes table in data sheet, from 15ms to 16s.
0-1 = averaging bits, see convTimes table, from 1 avg to 64 avg.
*/

void TMP117_Lunar::setConfig(uint8_t config){

  uint8_t buffer[2];
  buffer[1] =  ((uint8_t)config<<5);
  buffer[0] =  ((uint8_t)(config>>3));

  Lunar_I2C::writeBytes(TMP117_TEMP_I2C, TMP117_CONFIG_REG, 2, buffer);

  // CONFIG_PRINT_STATEMENT
  #ifdef TMP_DEBUG
  Serial.print(F("TMP CONFIG STR: 0x")); Serial.println(config,HEX);

  uint8_t mode = ((config>>5) & 0x3);
  uint8_t conversion_bits = ((config>>2) & 0x7);
  uint8_t averaging_bits = ((config) & 0x3);

  switch (mode){
    case 0x0:
    Serial.print(F("TMP mode set to: ")); Serial.println(F("continuous conv"));
    break;
    case 0x1:
    Serial.print(F("TMP mode set to: ")); Serial.println(F("shutdown"));
    break;
    case 0x2:
    Serial.print(F("TMP mode set to: ")); Serial.println(F("single shot"));
    break;
    case 0x3:
    //0b00 is the same as 0b11 for the TMP
    Serial.print(F("TMP mode set to: ")); Serial.println(F("continuous conv"));
    break;
    default:
    Serial.print(F("TMP mode set to: ")); Serial.println(F("something went wrong"));
    break;
  };

  switch (conversion_bits){
    case 0x0:
    Serial.print(F("TMP conversion bits: ")); Serial.print(conversion_bits,BIN); Serial.println(F(" (15.5 ms)"));
    break;
    case 0x1:
    Serial.print(F("TMP conversion bits: ")); Serial.print(conversion_bits,BIN); Serial.println(F(" (125 ms)"));
    break;
    case 0x2:
    Serial.print(F("TMP conversion bits: ")); Serial.print(conversion_bits,BIN); Serial.println(F(" (250 ms)"));
    break;
    case 0x3:
    Serial.print(F("TMP conversion bits: ")); Serial.print(conversion_bits,BIN); Serial.println(F(" (500 ms)"));
    break;
    case 0x4:
    Serial.print(F("TMP conversion bits: ")); Serial.print(conversion_bits,BIN); Serial.println(F(" (1 s)"));
    break;
    case 0x5:
    Serial.print(F("TMP conversion bits: ")); Serial.print(conversion_bits,BIN); Serial.println(F(" (4 s)"));
    break;
    case 0x6:
    Serial.print(F("TMP conversion bits: ")); Serial.print(conversion_bits,BIN); Serial.println(F(" (8 s)"));
    break;
    case 0x7:
    Serial.print(F("TMP conversion bits: ")); Serial.print(conversion_bits,BIN); Serial.println(F(" (16 s)"));
    break;
  };

  switch (averaging_bits){
    case 0x0:
    Serial.print(F("TMP averaging_bits bits: ")); Serial.print(averaging_bits,BIN); Serial.println(F(" (0 averages (i.e. one data point))"));
    break;
    case 0x1:
    Serial.print(F("TMP averaging_bits bits: ")); Serial.print(averaging_bits,BIN); Serial.println(F(" (8 averages)"));
    break;
    case 0x2:
    Serial.print(F("TMP averaging_bits bits: ")); Serial.print(averaging_bits,BIN); Serial.println(F(" (32 averages)"));
    break;
    case 0x3:
    Serial.print(F("TMP averaging_bits bits: ")); Serial.print(averaging_bits,BIN); Serial.println(F(" (64 averages)"));
    break;
    default:
    Serial.print(F("TMP averaging_bits bits: ")); Serial.println(F(" something went wrong"));
    break;
  };
  #endif
}

void TMP117_Lunar::getData(uint8_t* buffer){
  Lunar_I2C::readBytes(TMP117_TEMP_I2C, TMP117_TEMP_REG, 2, buffer);
}

float TMP117_Lunar::convertToTempC(uint8_t* buffer, float* temperature){
  *temperature =  (float)(buffer[0] << 8 | buffer[1]) * TMP117_RESOLUTION;
}
