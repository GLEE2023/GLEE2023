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

bool AK_DEBUG = false; //AK_DEBUG flag for printouts


AK09940::AK09940(void) {
	//constructor

}

void AK09940::begin(void) {
	Wire.begin(AK09940_SLAVE_ADDR);
	if (AK_DEBUG) {
		Serial.print("Beginning Communication with AK09940");
	}
}

bool AK09940::isConnected(void) {
	Wire.beginTransmission(AK09940_SLAVE_ADDR);
	byte status;
	status = Wire.endTransmission();
	if (status==0) {
		if (AK_DEBUG) {
			Serial.println("Successful Connection with AK09940");
		}
		return true;
	}
	else {
		if(AK_DEBUG) {
		Serial.println("ERROR: UNABLE TO CONNECT TO AK09940");
		Serial.print("Status Byte = ");
		Serial.println(status);
		}
		return false;
	}
}

void AK09940::writeByte(uint8_t AK09940_reg, uint8_t AK09940_data) {
	Wire.beginTransmission(AK09940_SLAVE_ADDR);	//begins communication with AK09940
	Wire.write(AK09940_reg);			//Gives register to write to
	Wire.write(AK09940_data);			//Gives data to write to register
	Wire.endTransmission();				//Writes data to register and end communication
}


byte AK09940::readByte(uint8_t AK09940_reg) {
	byte readByte;	//will store data read
	Wire.beginTransmission(AK09940_SLAVE_ADDR);	//begin comms w/ AK09940
	Wire.write(AK09940_reg);	//identifies register to read data from
	Wire.endTransmission();		//end transmission
	Wire.requestFrom(AK09940_SLAVE_ADDR,1);	//requests 1 byte from register previously called
	readByte = Wire.read();			//reads requested byte
	if (AK_DEBUG) {
		//AK_DEBUG printouts
		Serial.print("Reading from Register:");
		Serial.println(AK09940_reg,HEX);
		Serial.print("Register Data (HEX)= ");
		Serial.println(readByte,HEX);
		Serial.print("Register Data (BIN)= ");
		Serial.println(readByte,BIN);
	}
	return readByte;		//returns readByte
}

void AK09940::whoAmI(void) {
	byte whoAmI_1,whoAmI_2;
	whoAmI_1=readByte(AK09940_WAI_1);
	whoAmI_2=readByte(AK09940_WAI_2);
	if (AK_DEBUG && whoAmI_1!=0x48 && whoAmI_2!=0xA1) {
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

void AK09940::getStatus(void) {


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

float AK09940::getMagX(void) {
	int hByte, mByte, lByte, corrected_hByte;
	int hByte_MSB_Remove = 0b00000011;
	int32_t rawMag18, rawMag32;
	float mag;
	lByte = readByte(AK09940_HXL);
	mByte = readByte(AK09940_HXM);
	hByte = readByte(AK09940_HXH);
	corrected_hByte = (hByte && hByte_MSB_Remove); 
	rawMag18 = (hByte<<16 | mByte<<8 | lByte); 	//Stores 18 bit int in 32 bit variable
	rawMag32 = interpret18BitAs32Bit(rawMag18); 	//Converts from 18 bit to 32 bit decimal
	mag = 10 * rawMag32; 				//output is in unit of nT
	return mag;

}

float AK09940::getMagY(void) {
	int hByte, mByte, lByte, corrected_hByte;
	int hByte_MSB_Remove = 0b00000011;
	int32_t rawMag18, rawMag32;
	float mag;
	lByte = readByte(AK09940_HYL);
	mByte = readByte(AK09940_HYM);
	hByte = readByte(AK09940_HYH);
	corrected_hByte = (hByte && hByte_MSB_Remove); 
	rawMag18 = (hByte<<16 | mByte<<8 | lByte); 	//Stores 18 bit int in 32 bit variable
	rawMag32 = interpret18BitAs32Bit(rawMag18); 	//Converts from 18 bit to 32 bit decimal
	mag = 10 * rawMag32; 				//output is in unit of nT
	return mag;



}

float AK09940::getMagZ(void) {
	int hByte, mByte, lByte, corrected_hByte;
	int hByte_MSB_Remove = 0b00000011;
	int32_t rawMag18,rawMag32;
	float mag;
	lByte = readByte(AK09940_HZL);
	mByte = readByte(AK09940_HZM);
	hByte = readByte(AK09940_HZH);
	corrected_hByte = (hByte && hByte_MSB_Remove); 
	rawMag18 = (hByte<<16 | mByte<<8 | lByte); 	//Stores 18 bit int in 32 bit variable
	rawMag32 = interpret18BitAs32Bit(rawMag18); 	//Converts from 18 bit to 32 bit decimal
	mag = 10 * rawMag32; 				//output is in unit of nT
	return mag;



}