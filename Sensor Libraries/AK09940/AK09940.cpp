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
	
	if (AK09940.sensorDebug) {							//Print Statements for debugging and visualization of what this sensor does!
		Serial.print("Beginning Communication with AK09940");
	}
}


bool AK09940::ak09940WAI(void) {						//Internal Who Am I Register verification, returns true if values given by data sheet are correct!
	uint8_t whoAmI_1,whoAmI_2;
	uint8_t whoAmI_1=readByte(AK09940_WAI_1);
	uint8_t whoAmI_2=readByte(AK09940_WAI_2);

	if (AK09940.sensorDebug && (whoAmI_1!=0x48 || whoAmI_2!=0xA1))
	{
		Serial.println("ERROR: WHO AM I REGISTERS NOT RETURNING CORRECT VALUES");
		Serial.print("WHO_AM_I_1 REGISTER SHOULD RETURN 48, WHO_AM_I_1 RETURNS: ");
		Serial.println(whoAmI_1);
		Serial.print("WHO_AM_I_2 REGISTER SHOULD RETURN A1, WHO_AM_I_2 RETURNS: ");
		Serial.println(whoAmI_2);
		return false;
	}

	else if (AK09940.sensorDebug)
	{
		Serial.print("WHO_AM_I_1 REGISTER SHOULD RETURN 48, WHO_AM_I_1 RETURNS: ");
		Serial.println(whoAmI_1);
		Serial.print("WHO_AM_I_2 REGISTER SHOULD RETURN A1, WHO_AM_I_2 RETURNS: ");
		Serial.println(whoAmI_2);
		return true;
	}

	else if (whoAmI_1 = 0x48 || whoAmI_2 = 0xA1) 
	{
		return true;
	}
	else 
	{
		return false;
	}
	
}


void AK09940::getCalculatedData(void) {
	
	
		
		AK09940.readableData.magX=AK09940.getMag( AK09940.rawData.XMagLow , AK09940.rawData.XMagMid , AK09940.rawData.XMagHigh );
		AK09940.readableData.magY=AK09940.getMag( AK09940.rawData.YMagLow , AK09940.rawData.YMagMid , AK09940.rawData.YMagHigh );
		AK09940.readableData.magZ=AK09940.getMag( AK09940.rawData.ZMagLow , AK09940.rawData.ZMagMid , AK09940.rawData.ZMagHigh );
		AK09940.readableData.temperature=AK09940.getTemperature( AK09940.rawData.tempByte);
	
}


void AK09940::getRawData(void)
{
	uint8_t XML, XMM , XMH;
	uint8_t YML, YMM , YMH;
	uint8_t ZML, ZMM , ZMH;
	uint8_t tempRead;

	if (AK09940.dataReady())
	{

		XML = AK09940.readByte(AK09940_HXL);
		XMM = AK09940.readByte(AK09940_HXM);
		XMH = AK09940.readByte(AK09940_HXH);
		YML = AK09940.readByte(AK09940_HYL);
		YMM = AK09940.readByte(AK09940_HYM);
		YMH = AK09940.readByte(AK09940_HYH);
		ZML = AK09940.readByte(AK09940_HZL);
		ZMM = AK09940.readByte(AK09940_HZM);
		ZMH = AK09940.readByte(AK09940_HZH);
		tempRead = AK09940.readByte(AK09940_TMPS);
		AK09940.getDataStatus();


		if (AK09940.dataStatus.dataValid && !AK09940.dataStatus.dataOverflow)
		{

			AK09940.rawData.xMagLow = XML;
			AK09940.rawData.xMagMid = XMM;
			AK09940.rawData.xMagHigh = XMH;
			AK09940.rawData.yMagLow = YML;
			AK09940.rawData.yMagMid = YMM;
			AK09940.rawData.yMagHigh = YMH;
			AK09940.rawData.zMagLow = ZML;
			AK09940.rawData.zMagMid = ZMM;
			AK09940.rawData.zMagHigh = ZMH;
			AK09940.rawData.tempByte = tempRead;

		}

		else
		{
			Serial.println(" ERROR in getRawData: Data invalid or data overflow.");
		}
		

	}

	else
	{
		Serial.println("ERROR in getRawData: Data Not Ready.")
	}
	

	
}




float AK09940::calcTemp(uint8_t tempByte)
{ 
	float tempDec = float(tempByte);
	float tOut;
	tOut = 30.0 - tempDec/1.72;
	return tOut;
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

float AK09940::calcMag(uint8_t lowByte, uint8_t midByte, uint8_t highByte) 
{			
	uint8_t corrected_hByte;
	uint8_t hByte_MSB_Remove = 0b00000011;
	int32_t rawMag18, rawMag32;
	float mag;
	corrected_hByte = (highByte && hByte_MSB_Remove); 
	rawMag18 = (corrected_hByte<<16 | mByte<<8 | lByte); 			//Stores 18 bit int in 32 bit variable
	rawMag32 = interpret18BitAs32Bit(rawMag18); 					//Converts from 18 bit to 32 bit decimal
	mag = 10 * rawMag32; 											//output is in unit of nT
	return mag;

}




void AK09940::setDriveMode(ak09940_Drive_mode_t driveMode)			//writes to CNTRL 3 Register. Bits: [7] = FIFO, [6:5] = Drive Mode, [4:0] = Measurement Mode
{
	uint8_t FIFOData = uint8_t(AK09940.sensorFIFOEnabled);
	uint8_t measurementMode = AK09940.getMeasurementMode();

	uint8_t writeData = (FIFO<<7 | measurementMode << 5 | driveMode );

	AK09940.writeByte( AK09940_CNTL_3 , writeData);
}

ak09940_Drive_Mode_t AK9940::getDriveMode(void)
{
	uint8_t byteRead = AK09940.readByte(AK09940_CNTL_3);

	uint8_t removeByte = 0b01100000;
	uint8_t driveMode = ((byteRead && removeByte) >> 5);

	return driveMode;
}


void AK09940::setMeasurementMode(ak09940_Measurement_Mode_t measurementMode)	//writes to CNTRL 3 Register. Bits: [7] = FIFO, [6:5] = Drive Mode, [4:0] = Measurement Mode
{
	uint8_t driveMode = AK09940.getDriveMode();

	uint8_t FIFOData = uint8_t (Ak09940.sensorFIFOEnabled);
	uint8_t writeData = (FIFOData << 7 | measurementMode << 5 | driveMode );

	if (AK09940.sensorDebug)
	{
		Serial.println("Measurement Mode Set");
		Serial.print("New Mode: ");
		Serial.println( ak09940_Measurement_Mode_t (measurementMode) );
	}

	Ak09940.writeByte( AK09940_CNTL_3 );
}

ak09940_Measurement_Mode_t AK09940::getMeasurementMode(void)
{
	uint8_t byteRead = AK09940.readByte(AK09940_CNTL_3);

	uint8_t removeByte = 0b00011111;										//this will delete FIFO and drive mode data, leaving only the measurement mode
	uint8_t measurementMode = ( byteRead && removeByte );					//combine the data to remove and data recieved to get our final data

	if (AK09940.sensorDebug)
	{
		Serial.println("Measurement Mode Retreived");
		Serial.print("Current mode set: ");
		Serial.println( ak09940_Measurement_Mode_t (measurementMode) );
	}

	return measurementMode;

}


void AK09940::setWatermarkMode(uint8_t watermarkLevel)		//must be between 0 && 7
{
	ak09940_Measurement_Mode_t currentMeasurementMode = AK09940.getMeasurementMode();

	if (currentMeasurementMode != POWER_DOWN )			//verify mode is POWER_DOWN
	{
		AK09940.setMeasurementMode(POWER_DOWN);			// must be in power down mode in order to write to this register

		if (AK09940.sensorDebug)						//Debug for print statements
		{
			Serial.println("Sensor not in POWER_DOWN mode! Changing mode to POWER_DOWN mode in order to write to register")
		}
	}

	if (watermarkLevel >= 0 && watermarkLevel <= 7 )		//verify input is between 1 and 8
	{
		AK09940.writeByte(AK09940_CNTL_1, (watermarkLevel));
		AK09940.setMeasurementMode(currentMeasurementMode);
		
		if (AK09940.sensorDebug)
		{
			Serial.print("The watermark level has been set to: ");
			Serial.print(watermarkLevel+1);
			Serial.println(" data sets");
			Serial.print("Measurement mode has been changed back to: ");
			Serial.println(currentMeasurementMode);
		}
	}
	
	else
	{
		AK09940.writeByte( AK09940_CNTL_1 , 0 );
		AK09940.setMeasurementMode(currentMeasurementMode);

		if (Ak09940.sensorDebug)
		{
			Serial.println("Error, value entered is not between 0 and 7.");
			Serial.println("Watermark level has been set to a default of 1 data set; WM[2:0]=0 ");
			Serial.print("Measurement mode has been changed back to: ");
			Serial.println(currentMeasurementMode);
		}

	}
	



}

AK09940::getWatermarkMode(void);
{
	uint8_t currentWatermarkMode;
	currentWatermarkMode = AK09940.readByte(AK09940_CNTL_1);
	return currentWatermarkMode;

	if (AK09940.debug)
	{
		Serial.print("Current Watermark Mode:");
		Serial.println(currentWatermarkMode);
	}

}




void AK09940:setFIFO(bool FIFOState)
{
	uint8_t writeData;
	uint8_t measurementMode = AK09940.getMeasurementMode();
	uint8_t driveMode = AK09940.getDriveMode();

	writeData = (uint8_t(FIFOState) << 7 | driveMode << 5 | measurementMode);


	AK09940.writeByte(AK09940_CNTL_3, writeData);

	if (AK09940.sensorDebug)
	{
		if (FIFOState)
		{
			Serial.println("FIFO Enabled");
		}
		else 
		{
			Serial.println("FIFO Disabled");
		}
	}

	AK09940.sensorFIFOEnabled = AK09940.getFIFOState();

}


bool AK09940::getFIFOState(void)
{
	uint8_t byteRead = AK09940.readByte(AK09940_CNTL_3);
	uint8_t FIFOState = (byteRead >> 7)
	return bool(FIFOState);

	if (AK09940.sensorDebug)
	{
		if (bool(FIFOState))
		{
			Serial.println("FIFO Enabled");
		}
		else 
		{
			Serial.println("FIFO Disabled");
		}
	}

}


uint8_t AK09940:getNumDataInBuffer(void)
{
	uint8_t byteRead = AK09940.readByte(AK09940_ST_1);

	uint8_t numData = (byteRead>>1);

	return numData;

	if (AK09940.debug)
	{
		Serial.print("Current number of datasets available in buffer: ");
		Serial.println(numData);

	}



}

void AK09940::softReset(void)
{
	if (AK09940.sensorDebug)
	{
		Serial.println("SOFT RESET COMMAND RECIEVED: RESETTING AK09940 NOW");
	}

	AK09940.writeByte(AK09940_CNTL_4 , 1);
}



void AK09940::getDataStatus (void)
{
	uint8_t dataStatusByte = Ak09940.readByte(AK09940_ST_1);
	bool dataInvalidFlag, dataOverflowFlag;

	dataInvalid = (bool ((dataStatusByte && 0b00000010) >> 1 ));
	AK09940.dataStatus.dataValid = !dataInvalidFlag;

	dataOverflowFlag = bool ((dataStatusByte && 0b00000001));
	AK09940.dataStatus.dataOverflow = dataOverflowFlag;

	if (AK09940.sensorDebug)
	{
		if (dataInvalidFlag)
		{
			Serial.println("ERROR: AK09940 Reports invalid data. "); 
		}

		if (dataOverflowFlag)
		{
			if AK09940.sensorFIFOEnabled
			{
				Serial.println("Data Overflow! A new set of data was measured when the FIFO buffer was full. Data set over flowed from buffer.");
			}
			else
			{
				Serial.println("Data Overflow! FIFO Disabled. Data skipped in continious measurement.");
			}
			
		}

		else if (!dataInvalidFlag && !dataOverflowFlag)
		{
			Serial.println("Data Valid -- No Data Overflow");
		}

	}


}



bool AK09940::dataReady(void)
{
	uint8_t readByte = AK09940.readByte(AK09940_ST_1);
	uint8_t removeByte = 0b00000001;
	bool dataReady = bool(readByte && removeByte);
	return dataReady;

	if (AK09940.sensorDebug)
	{
		if (dataReady)
		{
			Serial.println("Data ready to be read");
		}
		else
		{
			Serial.println("Error: Data not ready to be read");
		}
		
	}
}

