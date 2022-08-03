/*
From LSB to MSB:
-TMP117 (19-25)
24-25 = mode
21-23 = conversion bits, see convTimes table.
19-20 = averaging bits, see convTimes table.

-MPU6000 (4-18)
16-18 = digital low pass setting, may or may not be removed.
6-15 = sample rate divisor, number between 0-255
4-7 = mode, options are in generateBitstrings file.

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

#define MODE_1_CONFIG 0b010000000000000000010111011

#define MODE_2_CONFIG 10952

#define MODE_1_DURATION 100
#define MODE_2_DURATION 200



void setup(){
  //setup
  Wire.begin();
  Serial.begin(9600);

  setConfig(MODE_1_CONFIG);
}

void loop(){
  //used to time how long the loop takes to run
  unsigned long timestamp = micros();

  //this serves as the array that will store the data accessed over I2C for all the sensors
  uint8_t buffer[6];

  //these arrays will store the data that is calculated from the bits stored in the buffer
  float magData[3];
  float accData[3];
  float temperature;

  //each sensor has its own namespace with functions associated with it to get the data and convert from bytes in the buffer to actual data

  //get data from accelerometer
  MPU6000_Lunar::readAccData(buffer);
  MPU6000_Lunar::convertAccToFloat(buffer, accData);

  //get data from temperature sensor
  TMP117_Lunar::getData(buffer);
  TMP117_Lunar::convertToTempC(buffer, &temperature);

  // get data from magnetometer
  BM1422AGMV_Lunar::getData(buffer);
  BM1422AGMV_Lunar::convertToFloats(buffer, magData);

  timestamp = micros()-timestamp;

  //print data
  Serial.print(timestamp); Serial.print(", "); Serial.print(temperature); Serial.print(", "); Serial.print(magData[0]); Serial.print(", "); Serial.print(magData[1]); Serial.print(", "); Serial.print(magData[2]); Serial.print(", "); Serial.print(accData[0]); Serial.print(", "); Serial.print(accData[1]); Serial.print(", "); Serial.print(accData[2]); Serial.println();
}

void setConfig(long config){
  Serial.print("Config string passed into setConfig: ");
  Serial.println(config,HEX);

  // setTMP((config<<0) & 0xFF);
  MPU6000_Lunar::setConfig((config>>4) & 0x7FF);
  TMP117_Lunar::setConfig((config>>19) & 0x3F);
  BM1422AGMV_Lunar::setConfig(config & 0xF);
  // setTPIS((config<<23) & 0x1);
  // setCAP()
}
