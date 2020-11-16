#include <Arduino.h>
#include <Wire.h>
#include "TMP117.h"


TMP117::TMP117(){
    TMP117::sensorName = "TMP117 Temperature Sensor";
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
    write2Byte(TMP117_LOW_LIMIT_REG, lLimit);
}
float TMP117::getLowLimit(){
    int16_t LowLimit = read2Byte(TMP117_LOW_LIMIT_REG);
    float limit = (float)LowLimit * TMP117_RESOLUTION;
    return limit;
}

void TMP117::setHighLimit(float highLimit){
    int16_t hLimit = highLimit/TMP117_RESOLUTION;
    write2Byte(TMP117_HIGH_LIMIT_REG, hLimit);
}

float TMP117::getHighLimit(){
    int16_t HighLimit = read2Byte(TMP117_HIGH_LIMIT_REG);
    float limit = (float)HighLimit * TMP117_RESOLUTION;
    return limit;
}
bool TMP117::getHighAlert(){
    uint16_t config = read2Byte(TMP117_CONFIG_REG);
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
    write2Byte(TMP117_CONFIG_REG, defaultConfig);
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
    Wire.beginTransmission(TMP117::sensorAddress);           //begins comms with sensor specified
    Wire.write(registerAddress);                            //identifies register for data to be read from
    Wire.endTransmission();                                 //end transmission
    Wire.requestFrom(TMP117::sensorAddress, uint8_t(2) );               //request 2 bytes from the sensor address
    uint8_t data[2] = {0};			// Declares an array of length 2 to be empty
  	int16_t datac = 0;				// Declares the return variable to be 0
  	if (Wire.available() <= 2) // Won't read more than 2 bits
  	{
  		data[0] = Wire.read();			// Reads the first set of bits (D15-D8)
  		data[1] = Wire.read();			// Reads the second set of bits (D7-D0)
  		datac = ((data[0] << 8) | data[1]); // Swap the LSB and the MSB
  	}
  	return datac;                                //read data and store in the readByte variable                                       //return the read data byte
}
void TMP117::write2Byte(uint8_t reg, uint16_t data) // originally TMP117_Register reg
{
	Wire.beginTransmission(TMP117::sensorAddress); // Originally cast uint8_t when a register value again
	Wire.write(reg);
	Wire.write(highByte(data)); // Write MSB (D15-D8)
	Wire.write(lowByte(data));  // Write LSB (D7-D0)
	Wire.endTransmission();	 // Stop transmitting data
}
