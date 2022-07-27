/*
From LSB to MSB:
-TMP117 (20-25)
INSERT_NUM = mode, 0b10 for one shot, 0b11 for continuous conversion
INSERT_NUM = conversion bits, see convTimes table.
INSERT_NUM = averaging bits, see convTimes table.

-MPU6000 (5-19)
5-8 = mode, options are in generateBitstrings file.
9-16 = sample rate divisor, number between 0-255
17-19 = digital low pass setting, may or may not be removed.

-Therm (4)
22 = on or off

-Cap (3)
23 = on or off

-Mag (0-2)
 = mode, represents frequency, one more bit needed.
*/

#include <Arduino.h>
#include <Wire.h>
#include "MPU6000_Lunar.h"

#define MODE_1_CONFIG 0x54

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
  MPU6000_Lunar::readAccData(buffer);
  float acc_z = (float)(buffer[4] << 8 |buffer[5]) / 16340.0 * -1;
  Serial.println(acc_z);
}

void setConfig(long config){
  Serial.print("Config string passed into setConfig: ");
  Serial.println(config,HEX);
  //do maksing to get config for each sensor, currently trash!!!!
  // uint8_t temp_config   = (config<<0) & 0xFF;
  // uint16_t acc_config   = (config<<8) & 0x7FF;
  // uint8_t therm_config  = (config<<23) & 0x1;
  // uint8_t cap_config    = (config<<24) & 0x1; //unused
  // uint8_t mag_config    = (config<<25) & 0x2;

  // setTMP((config<<0) & 0xFF);
  MPU6000_Lunar::setMPU((config>>4) & 0x7FF);
  // setTPIS((config<<23) & 0x1);
  // setCAP()
  // setBM14()
}
