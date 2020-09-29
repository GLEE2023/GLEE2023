/*
*Description: This is an Arduino (C++) .cpp file required for the ICM20602 Accelerometer
*Project Info: Created For GLEE (The Great Lunar Expedition for Everyone)
*Library Author: John Walker Johnson
*Library Created on: July 13th 2020
*Last Modified on: July 14th 2020
*Resources Used in Creation:
*ICM20602 Datasheet
*Arduino Wire Library Reference Guide
*/


#include "ICM20602.h"
#include <Arduino.h>
#include <Wire.h>

bool ICM_DEBUG = false;

// Acc Full Scale Range  +-2G 4G 8G 16G 
enum Ascale
{
    AFS_2G=0,  
    AFS_4G,
    AFS_8G,
    AFS_16G
};

// Gyro Full Scale Range +-250 500 1000 2000 Degrees per second
enum Gscale
{
    GFS_250DPS=0,   
    GFS_500DPS,
    GFS_1000DPS,
    GFS_2000DPS
};

// Scale resolutions per LSB for the sensors
float aRes, gRes; 
int Ascale = AFS_2G;
int Gscale = GFS_1000DPS;

ICM20602::ICM20602(void){
	//constructor
}

void ICM20602::begin(void)
{

	Wire.begin(ICM20602_slave_addr);
	Serial.println("Beginning Communication With ICM20602");
	return true;

}


bool ICM20602::isConnected(void) {
  Wire.beginTransmission(ICM20602_slave_addr);
  if(Wire.endTransmission() == 0) {
    if(ICM_DEBUG)
      Serial.println("ICM20602 Connected on I2C bus.");
    return true;
  }
  if(ICM_DEBUG)
    Serial.println("ICM20602 Not Connected.");
  return false;
}



void ICM20602::writeByte(uint8_t ICM20602_reg,uint8_t ICM20602_data)
{
	Wire.beginTransmission(ICM20602_slave_addr);
	Wire.write(ICM20602_reg);
	Wire.write(ICM20602_data);
	Wire.endTransmission();
	
	
}


byte ICM20602::readByte(uint8_t ICM20602_reg)
{
	Wire.beginTransmission(ICM20602_slave_addr);
	Wire.write(ICM20602_reg);
	Wire.endTransmission();
	Wire.requestFrom(ICM20602_slave_addr,1);
	byte tempReadByte;
	tempReadByte = Wire.read();

		if(ICM_DEBUG)
		{
			Serial.print("Byte ");
			Serial.println(tempReadByte,HEX);
		}
	
return tempReadByte;
	
}


// Communication test: WHO_AM_I register reading 
void ICM20602::whoAmI(void)
{
	if(ICM_DEBUG)
	{
    		byte whoAmI = readByte(ICM20602_WHO_AM_I);
    		Serial.println("I AM ICM20602 AND THIS SHOULD PRINT 12");
    		Serial.print(whoAmI, HEX);
    	}
    
}

void ICM20602::initialize(void)
{
	Wire.setClock(400000);
	writeByte(ICM20602_PWR_MGMT_1, 0x01);		//set clock set to auto
	writeByte(ICM20602_PWR_MGMT_2,0x07);		//disable gyro
	writeByte(ICM20602_CONFIG, 0x01); //176Hz    	// set TEMP_OUT_L, DLPF=3 (Fs=1KHz):0x03
	writeByte(ICM20602_GYRO_CONFIG, 0x00); 		// bit[4:3] 0=+-250d/s,1=+-500d/s,2=+-1000d/s,3=+-2000d/s :0x18
	writeByte(ICM20602_ACCEL_CONFIG, 0x10);		// bit[4:3] 0=+-2g,1=+-4g,2=+-8g,3=+-16g, ACC_HPF=On (5Hz):0x01
	
	
}


float ICM20602::getAccelX(void)
{
	int hByte,lByte,rawoutput;
	float scale,output;
	scale=8.0/32768.0;
	hByte=readByte(ICM20602_ACCEL_XOUT_H);
	lByte=readByte(ICM20602_ACCEL_XOUT_L);
	rawoutput=(hByte<<8|lByte);
	output=scale*rawoutput;
	return output;
	
}

float ICM20602::getAccelY(void)
{
		
	int hByte,lByte,rawoutput;
	float scale,output;
	scale=8.0/32768.0;
	hByte=readByte(ICM20602_ACCEL_YOUT_H);
	lByte=readByte(ICM20602_ACCEL_YOUT_L);
	rawoutput=(hByte<<8|lByte);
	output=scale*rawoutput;
	return output;
}

float ICM20602::getAccelZ(void)
{
	
	int hByte,lByte,rawoutput;
	float scale,output;
	scale=8.0/32768.0;
	hByte=readByte(ICM20602_ACCEL_ZOUT_H);
	lByte=readByte(ICM20602_ACCEL_ZOUT_L);
	rawoutput=(hByte<<8|lByte);
	output=scale*rawoutput;
	return output;
}