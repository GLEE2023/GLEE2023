<<<<<<< HEAD









float TMP117 :: getTemperatureC(void)

{
    uint8_t lowByte, highByte;
    lowByte = TMP117.readByte(Temperature_Output_Byte_Low);
    highByte = TMP117.readByte(Temperature_Output_Byte_High);

    uint16_t rawTempOutput = (highByte<<8 | lowByte);
}
=======
#include <Arduino.h>
#include <Wire.h>
#include "SparkFun_TMP117_Registers.h"
#include "tmp117.h"


TMP117::TMP117(uint8_t addr){
    TMP117.sensorName = "TMP177 Temperature Sensor";
  	TMP117.sensorAddress = addr;
  	TMP117.sensorDebug = false;
  	Wire.begin(TMP117.sensorAddress);
}

uint8_t TMP117::getAddress(){
    return TMP117.sensorAddress;
}

double TMP117::getTemperatureC()
{
    int16_t temp = readByte(TMP117_TEMP_REG);
    return (temp*TMP117_RESOLUTION);
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
    uint16_t config = readByte(TMP117_CONFIG_REG);
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
    uint16_t config = readByte(TMP117_CONFIG_REG);
    if(config & 1 << 13){
      return true;
    }
    return false;
}
>>>>>>> 748ffc6ae408a673f904658f411aa136e8251ac0
