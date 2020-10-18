#include <Arduino.h>
#include <Wire.h>
#include "tmp117.h"


TMP117::TMP117(){
    TMP117::sensorName = "TMP177 Temperature Sensor";
  	TMP117::sensorAddress = 0x48;
  	TMP117::sensorDebug = false;
  	Wire.begin(TMP117::sensorAddress);
}

uint8_t TMP117::getAddress(){
    return TMP117::sensorAddress;
}

double TMP117::getTemperatureC()
{
  int16_t digitalTempC = read2Byte(TMP117_TEMP_REG); // Calls to read registers to pull all the bits to store in an array

	float finalTempC = digitalTempC * TMP117_RESOLUTION; // Multiplies by the resolution for digital to final temp

	return finalTempC;
}
double TMP117::getTemperatureF(){
    return getTemperatureC() * 9.0 / 5.0 + 32.0;
}
float TMP117::getUncertainty(float inputTemperature){
    double temp = inputTemperature;
    if(temp >= -20 &&  temp <=50){
      return 0.1;
    }
    else if(temp >= -40 &&  temp <=70){
      return 0.15;
    }
    else if(temp >= -55 &&  temp <= 100){
      return 0.2;
    }
    else if(temp >= -55 &&  temp <=125){
      return 0.25;
    }
    else if(temp >= -55 &&  temp <=150){
      return 0.3;
    }
}
void TMP117::setLowLimit(float lowLimit){
    int16_t lLimit = lowLimit/TMP117_RESOLUTION;
    writeByte(TMP117_LOW_LIMIT_REG, lLimit);
}
float TMP117::getLowLimit(){
    int16_t LowLimit = readByte(TMP117_LOW_LIMIT_REG);
    float limit = (float)LowLimit * TMP117_RESOLUTION;
    return limit;
}

void TMP117::setHighLimit(float highLimit){
    int16_t hLimit = highLimit/TMP117_RESOLUTION;
    writeByte(TMP117_HIGH_LIMIT_REG, hLimit);
}

float TMP117::getHighLimit(){
    int16_t HighLimit = readByte(TMP117_HIGH_LIMIT_REG);
    float limit = (float)HighLimit * TMP117_RESOLUTION;
    return limit;
}
bool TMP117::getHighAlert(){
    uint16_t config = readByte(TMP117_CONFIG_REG);
    uint8_t configBit = bitRead(config, 15);
    if(configBit==1){
      return true;
    }
    return false;
}
bool TMP117::getLowAlert(){
    uint16_t config = read2Byte(TMP117_CONFIG_REG);
    uint8_t configBit = bitRead(config, 14);
    if(configBit==1){
      return true;
    }
    return false;
}

void TMP117::reset(){
    uint16_t defaultConfig = 0;
    defaultConfig |= 1UL << 1; //0000000000000010
    writeByte(TMP117_CONFIG_REG, defaultConfig);
}

bool TMP117::dataReady(){
    uint16_t config = read2Byte(TMP117_CONFIG_REG);
    if(config & 1 << 13){
      return true;
    }
    return false;
}

uint16_t TMP117::read2Byte(uint8_t registerAddress){
    uint16_t readByte;                                       //byte to store data that is read
    Wire.beginTransmission(Sensor::sensorAddress);           //begins comms with sensor specified
    Wire.write(registerAddress);                            //identifies register for data to be read from
    Wire.endTransmission();                                 //end transmission
    Wire.requestFrom(Sensor::sensorAddress, uint8_t(2) );               //request 2 bytes from the sensor address
    readByte = Wire.read();                                 //read data and store in the readByte variable
    return readByte;                                        //return the read data byte
}
