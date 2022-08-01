/*
From LSB to MSB:
-TMP117 (20-25)
24-25 = mode
21-23 = conversion bits, see convTimes table.
20-21 = averaging bits, see convTimes table.

-MPU6000 (5-19)
17-19 = digital low pass setting, may or may not be removed.
9-16 = sample rate divisor, number between 0-255
5-8 = mode, options are in generateBitstrings file.

-Therm (4)
4 = on or off (doesn't do anything)

-Cap (3)
3 = on or off (doesn't actually do anything)

-Mag (0-2)
0 = on or off, 0 is off
1-2 = mode, represents frequency, one more bit needed.
*/

#include <Arduino.h>
#include <Wire.h>
#include "MPU6000_Lunar.h"
#include "TMP117_Lunar.h"

// #define DBUG true

#define MODE_1_CONFIG 0b01000000000000000001011100

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
  MPU6000_Lunar::readAccData(buffer);
  float acc_z = (float)(buffer[4] << 8 |buffer[5]) / 16340.0 * -1;

  //get data from temperature sensor
  TMP117_Lunar::getData(buffer);
  float temp = TMP117_Lunar::convertToTempC(buffer);

  Serial.print(temp); Serial.print(", ");
  Serial.println(acc_z);
}

void setConfig(long config){
  Serial.print("Config string passed into setConfig: ");
  Serial.println(config,HEX);

  // setTMP((config<<0) & 0xFF);
  MPU6000_Lunar::setConfig((config>>4) & 0x7FF);
  TMP117_Lunar::setConfig((config>>19) & 0x3F);
  // setTPIS((config<<23) & 0x1);
  // setCAP()
  // setBM14()
}
