/*
From LSB to MSB:
-TMP117 (20-25)
25-26 = mode
22-24 = conversion bits, see convTimes table.
21-22 = averaging bits, see convTimes table.

-MPU6000 (5-19)
18-20 = digital low pass setting, may or may not be removed.
8-17 = sample rate divisor, number between 0-255
6-9 = mode, options are in generateBitstrings file.

-Therm (4)
5 = on or off (doesn't do anything)

-Cap (3)
4 = on or off (doesn't actually do anything)

-Mag (0-2)
3   = on or off, 0 is off
2   = cc or single shot
0-1 = cc freq
*/

#include <Arduino.h>
#include <Wire.h>
#include "MPU6000_Lunar.h"
#include "TMP117_Lunar.h"
#include "BM1422AGMV_Lunar.h"

// #define DBUG true

#define MODE_1_CONFIG 0b010000000000000000010111000

#define MODE_2_CONFIG 10952

#define MODE_1_DURATION 100
#define MODE_2_DURATION 200



void setup(){
  //setup
  Wire.begin();
  Serial.begin(9600);

  uint8_t mode_num = 0;
  if (mode_num == 0){
    setConfig(MODE_1_CONFIG);
  }
}

void loop(){

  uint8_t buffer[6];

  //get data from accelerometer
  // MPU6000_Lunar::readAccData(buffer);
  // float acc_z = (float)(buffer[4] << 8 | buffer[5]) / 16340.0 * -1;

  //get data from temperature sensor
  // TMP117_Lunar::getData(buffer);
  // float temp = TMP117_Lunar::convertToTempC(buffer);

  Serial.println("Mag stuff:");
  BM1422AGMV_Lunar::getData(buffer);
  unsigned short val = (signed short)(buffer[1]<<8) | (buffer[0]);
  Serial.print("VAL: "); Serial.println(val);
  float mag_x = (float)val / 6;

  // Serial.print(temp); 
  // Serial.print(", ");
  Serial.print(mag_x);
  // Serial.print(", ");
  // Serial.println(acc_z);
}

void setConfig(long config){
  Serial.print("Config string passed into setConfig: ");
  Serial.println(config,HEX);

  // setTMP((config<<0) & 0xFF);
  MPU6000_Lunar::setConfig((config>>5) & 0x7FF);
  TMP117_Lunar::setConfig((config>>20) & 0x3F);
  BM1422AGMV_Lunar::setConfig(config & 0xF);
  // setTPIS((config<<23) & 0x1);
  // setCAP()
  // setBM14()
}
