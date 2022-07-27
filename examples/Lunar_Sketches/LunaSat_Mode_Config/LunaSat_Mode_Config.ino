#include <Arduino.h>
#include <Wire.h>
#include "MPU6000_Lunar.h"

#define MODE_1_CONFIG 0x54

#define MODE_2_CONFIG 10952

#define MODE_1_DURATION 100
#define MODE_2_DURATION 200


void writeByte (uint8_t registerAddress, uint8_t writeData);
void readBytes (uint8_t registerAddress, uint8_t nBytes, uint8_t * data);

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

  float acc_z = (float)(buffer[4] << 8 |buffer[5]) / 1634.0 * -1;
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
  MPU6000_Lunar::setMPU((config>>6) & 0x7FF);
  // setTPIS((config<<23) & 0x1);
  // setCAP()
  // setBM14()
}

// void readBytes(uint8_t I2CsensorAddress, uint8_t registerAddress, uint8_t nBytes, uint8_t * data){
//     Wire.beginTransmission(I2CsensorAddress);           // begins forming transmission to sensor
//     Wire.write(registerAddress);                     // Add register address to transmission
//     Wire.endTransmission();
//     Wire.requestFrom(I2CsensorAddress, nBytes);         // Request and listen for response
//     // Record response, wire will be available until nBytes are read
//     int i = 0;
//     while(Wire.available()){
//         data[i] = Wire.read();
//         i++;
//     }
// }
//
// /**
//  * Parameters: Register Address, Write Data
//  * Returns: None
//  * This function writes data to specified address
// **/
// void writeByte (uint8_t I2CsensorAddress, uint8_t registerAddress, uint8_t writeData){
//     Wire.beginTransmission(I2CsensorAddress);               // begin communication with the sensor
//     Wire.write(registerAddress);                                // point to address to be written to
//     Wire.write(writeData);                                      // write data to adress specificed above
//     Wire.endTransmission();                                     // end communication
// }
