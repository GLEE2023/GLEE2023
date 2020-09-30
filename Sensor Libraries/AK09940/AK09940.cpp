/*
*Description: This is an Arduino (C++) .cpp file required for the AK09940 Magnetometer
*Project Info: Created For GLEE (The Great Lunar Expedition for Everyone)
*Library Author: John Walker Johnson
*Library Created on: July 13th 2020
*Last Modified on: July 14th 2020
*Resources Used in Creation:
*AK09940 Datasheet
*Arduino Wire Library Reference Guide
*/


#include "AK09940.h"
#include <Arduino.h>
#include <Wire.h>






void AK09940::AK09940(void) {							//constructor
	AK09940.sensorName = "AK09940 Magnetometer";		//assign sensor name
	AK09940.sensorAddress = AK09940_SENSOR_ADDR;		//assign sensor i2c address
	AK09940.sensorDebug=false;							//debug flag status
	Wire.begin(AK09940.sensorAddress);					//begin i2c comms with sensor, must be called once
	if (AK09940.sensorDebug) {
		Serial.print("Beginning Communication with AK09940");
	}
}


void AK09940::whoAmI(void) {
	byte whoAmI_1,whoAmI_2;
	whoAmI_1=readByte(AK09940_WAI_1);
	whoAmI_2=readByte(AK09940_WAI_2);
	if (AK09940.sensorDebug && whoAmI_1!=0x48 || whoAmI_2!=0xA1) {
	Serial.println("ERROR: WHO AM I REGISTERS NOT RETURNING CORRECT VALUES");
	Serial.print("WHO_AM_I_1 REGISTER SHOULD RETURN 48, WHO_AM_I_1 RETURNS: ");
	Serial.println(whoAmI_1);
	Serial.print("WHO_AM_I_2 REGISTER SHOULD RETURN A1, WHO_AM_I_2 RETURNS: ");
	Serial.println(whoAmI_2);
	}
}

void AK09940::initialize(void) {
	//writeByte(AK09940_CNTL_1,0x01);
	writeByte(AK09940_CNTL_2,0x40); //Enables temp sensor
	writeByte(AK09940_CNTL_3,0XEA);	//Enable FIFO, Measurement Type = Low Noise 2, Measurement Mode = 200Hz

}

void AK09940::getSensorData(void) {
	rawData.magX=AK09940.getRawMag(AK09940_HXL,AK09940_HXM,AK09940_HXH)
	rawData.magY=AK09940.getRawMag(AK09940_HYL,AK09940_HYM,AK09940_HYH)
	rawData.magZ=AK09940.getRawMag(AK09940_HZL,AK09940_HZM,AK09940_HZH)
	rawData.temp=
	
}

float AK09940::getTemperature(void) { //byte-->Int-->float seems convoluted, change later?
	byte tOutByte;
	float tOutDec;
	float tOut;
	tOutByte = readByte(AK09940_TMPS);
	tOutDec = (float) tOutByte;
	tOut = 30 - tOutDec/1.72;
	return tOut;
	Serial.println(tOutByte, HEX);
	Serial.println("YEET");
}

int32_t AK09940::interpret18BitAs32Bit(int32_t input) {
	if (bitRead(input,17)==0) {
		for(int i=31;i>=17;i--) {
			bitWrite(input,i,0);
		}
	}
	else if (bitRead(input,17)==1) {
		for(int i=31;i>=17;i--) {
			bitWrite(input,i,1);	
		}
	}
	else {
		Serial.println("FATAL ERROR IN INTERPRETING 18 BIT INT AS 32 BIT");
	}
	return input;
}

float AK09940::getRawMag(uint8_t lowByte, uint8_t midByte, uint8_t highByte) {
	int hByte, mByte, lByte, corrected_hByte;
	int hByte_MSB_Remove = 0b00000011;
	int32_t rawMag18, rawMag32;
	float mag;
	lByte = AK09940.readByte(lowByte);
	mByte = AK09940.readByte(midByte);
	hByte = AK09940.readByte(highByte);
	corrected_hByte = (hByte && hByte_MSB_Remove); 
	rawMag18 = (hByte<<16 | mByte<<8 | lByte); 			//Stores 18 bit int in 32 bit variable
	rawMag32 = interpret18BitAs32Bit(rawMag18); 		//Converts from 18 bit to 32 bit decimal
	mag = 10 * rawMag32; 								//output is in unit of nT
	return mag;

}