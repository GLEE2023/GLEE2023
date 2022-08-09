/*
From LSB to MSB:
-TMP117 (19-25)
24-25 = mode, CC:00 (or 11), shutdown:01, single shot:10
21-23 = conversion bits, see convTimes table in data sheet, from 15ms to 16s.
19-20 = averaging bits, see convTimes table, from 1 avg to 64 avg.

-MPU6000 (4-18)
16-18 = digital low pass setting, may or may not be removed. from BW of
6-15 = sample rate divisor, number between 0-255
4-7 = mode, see MPU6000_Lunar.cpp

-Mag (0-2)
3   = on or off, on:1, off:0
2   = cc or single shot, single: 1, continuous:0
0-1 = cc freq, (00:10Hz, 10: 20Hz, 01:100Hz, 11:1KHz)
*/

#include <Arduino.h>
#include <Wire.h>
#include "MPU6000_Lunar.h"
#include "TMP117_Lunar.h"
#include "BM1422AGMV_Lunar.h"

// #define DBUG true
//   ----------bit numbers:54321098765432109876543210
// #define MODE_1_CONFIG 0b01000000000000000000000000 //everything put to sleep

// MPU6000 only mode configs
// #define MODE_1_CONFIG 0b01000000000000000000010000 //acc only, LPWU 1.25hz
// #define MODE_1_CONFIG 0b01000000000000000000100000 //acc only, LPWU 5hz
// #define MODE_1_CONFIG 0b01000000000000000001000000 //acc only, LPWU 20hz
// #define MODE_1_CONFIG 0b01000000000000000001010000 //acc only, cont conv, dlpf:0, srd:0
// #define MODE_1_CONFIG 0b01000000000000000001100000 //gyro only
// #define MODE_1_CONFIG 0b01000000000000000001110000 //acc & gyro

// TMP only mode configs
//   ----------bit numbers:54321098765432109876543210
#define MODE_1_CONFIG 0b00000000000000000000000000 //cc, cc=15.5ms, 1avg
// #define MODE_1_CONFIG 0b00001000000000000000000000 //cc, cc=125ms, 1avg
// #define MODE_1_CONFIG 0b00010000000000000000000000 //cc, cc=250ms, 1avg
// #define MODE_1_CONFIG 0b00011000000000000000000000 //cc, cc=500ms, 1avg
// #define MODE_1_CONFIG 0b00100000000000000000000000 //cc, cc=1s, 1avg
// #define MODE_1_CONFIG 0b00101000000000000000000000 //cc, cc=4s, 1avg
// #define MODE_1_CONFIG 0b00110000000000000000000000 //cc, cc=8s, 1avg
// #define MODE_1_CONFIG 0b00111000000000000000000000 //cc, cc=16s, 1avg
// #define MODE_1_CONFIG 0b00000010000000000000000000 //cc, cc=15.5ms, 8avg
// #define MODE_1_CONFIG 0b00000100000000000000000000 //cc, cc=15.5ms, 32avg
// #define MODE_1_CONFIG 0b00000110000000000000000000 //cc, cc=15.5ms, 64avg

// BM1422AGMV only modes
// #define MODE_1_CONFIG 0b01000000000000000000001000 //continuous 10Hz
// #define MODE_1_CONFIG 0b01000000000000000000001010 //continuous 20Hz
// #define MODE_1_CONFIG 0b01000000000000000000001001 //continuous 100Hz
// #define MODE_1_CONFIG 0b01000000000000000000001011 //continuous 1kHz

// #define MODE_1_CONFIG 0b00001110000000000001011011 //max power usage

#define MODE_2_CONFIG 10952

#define MODE_1_DURATION 100
#define MODE_2_DURATION 200



void setup(){
  //setup
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
  Serial.print("Config string passed into setConfig (hex): ");
  Serial.println(config,HEX);

  // setTMP((config<<0) & 0xFF);
  MPU6000_Lunar::setConfig((config>>4) & 0x7FF);
  TMP117_Lunar::setConfig((config>>19) & 0x3F);
  BM1422AGMV_Lunar::setConfig(config & 0xF);
  // setTPIS((config<<23) & 0x1);
  // setCAP()
}
