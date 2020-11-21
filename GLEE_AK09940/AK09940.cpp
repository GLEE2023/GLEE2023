/*
*Description: This is an Arduino (C++) .cpp file required for the AK09940 Magnetometer
*Project Info: Created For GLEE (The Great Lunar Expedition for Everyone)
*Library Author: John Walker Johnson
*Library Contributors: Lawson Nerenberg
*Library Created on: July 13th 2020
*Last Modified on: October 8th 2020
*Resources Used in Creation:
*AK09940 Datasheet
*Arduino Wire Library Reference Guide
*
*/

//
//
//	Function status: Percentage, Enum(TBD,TBR,FINAL,ETC) , ????
//
//

#include "AK09940.h"
#include <Arduino.h>
#include <Wire.h>


AK09940::AK09940() 			//constructor - Function Status: TBR -- add inital sensor settings?
{							
	AK09940::info.name = "AK09940 Magnetometer";		//assign sensor name
	AK09940::info.address = AK09940_SENSOR_ADDR;		//assign sensor i2c address
	AK09940::sensorDebug=false;							//debug flag status
	Wire.begin(AK09940::info.address);					//begin i2c comms with sensor, must be called once
	
	if (AK09940::sensorDebug) {							//Print Statements for debugging and visualization of what this sensor does!
		Serial.print("Beginning Communication with AK09940");
	}
}


bool AK09940::ak09940WAI(void) 				//Internal Who Am I Register verification, returns true if values given by data sheet are correct!
{											//Function status: TBR
	uint8_t whoAmI_1,whoAmI_2;
	whoAmI_1=readByte(AK09940_WAI_1);				//read 1st Who Am I Register
	whoAmI_2=readByte(AK09940_WAI_2);				//Read 2nd Who Am I register 

	if (AK09940::sensorDebug && (whoAmI_1 != 0x48 || whoAmI_2 != 0xA1))			//check for debug and incorrect sensor values
	{
		Serial.println("ERROR: WHO AM I REGISTERS NOT RETURNING CORRECT VALUES");
		Serial.print("WHO_AM_I_1 REGISTER SHOULD RETURN 48, WHO_AM_I_1 RETURNS: ");
		Serial.println(whoAmI_1);
		Serial.print("WHO_AM_I_2 REGISTER SHOULD RETURN A1, WHO_AM_I_2 RETURNS: ");
		Serial.println(whoAmI_2);
		return false;
	}

	else if (AK09940::sensorDebug)												//check for just debug print statements
	{
		Serial.print("WHO_AM_I_1 REGISTER SHOULD RETURN 48, WHO_AM_I_1 RETURNS: ");
		Serial.println(whoAmI_1);
		Serial.print("WHO_AM_I_2 REGISTER SHOULD RETURN A1, WHO_AM_I_2 RETURNS: ");
		Serial.println(whoAmI_2);
		return true;														//return true if both WAI read correctly
	}

	else if (whoAmI_1 == 0x48 || whoAmI_2 == 0xA1) 			//no debug and WAI registers return correct value
	{
		return true;										//returns true if WAI registers are correctly read
	}
	else 
	{
		return false;										// return falst if not read correctly
	}
	
}


void AK09940::getCalculatedData(void) 					// Converts raw data bytes to nT and Celcius, change function name??
{
	
		
		AK09940::calculatedData.xMag=AK09940::calcMag( AK09940::rawData.xMagLow , AK09940::rawData.xMagMid , AK09940::rawData.xMagHigh );		//only converts data that was read with no errors,  no overflow, no invalid data
		AK09940::calculatedData.yMag=AK09940::calcMag( AK09940::rawData.yMagLow , AK09940::rawData.yMagMid , AK09940::rawData.yMagHigh );
		AK09940::calculatedData.zMag=AK09940::calcMag( AK09940::rawData.zMagLow , AK09940::rawData.zMagMid , AK09940::rawData.zMagHigh );
		AK09940::calculatedData.temperature=AK09940::calcTemp( AK09940::rawData.tempByte);													//assigns results to sensor data structure 
		
}


bool AK09940::getRawData(void)		//TBR -- Redundancy?? 
{
	uint8_t XML, XMM , XMH;			//variables to hold low, mid, and high bytes for X-axis magnetic field 
	uint8_t YML, YMM , YMH;			//variables to hold low, mid, and high bytes for Y-axis magnetic field 
	uint8_t ZML, ZMM , ZMH;			//variables to hold low, mid, and high bytes for Z-axis magnetic field 
	uint8_t tempRead;				//byte to hold temperature data
									//bytes are held here before assigning to lunasat data structure in order to verify data status after reading data

	if (AK09940::dataReady())		//data must be ready to be read
	{

		XML = AK09940::readByte(AK09940_HXL);		//read X Low byte
		XMM = AK09940::readByte(AK09940_HXM);		//read X Middle byte
		XMH = AK09940::readByte(AK09940_HXH);		//read X High byte
		YML = AK09940::readByte(AK09940_HYL);		//read X Low byte
		YMM = AK09940::readByte(AK09940_HYM);		//read X Middle byte
		YMH = AK09940::readByte(AK09940_HYH);		//read X High byte
		ZML = AK09940::readByte(AK09940_HZL);		//read X Low byte
		ZMM = AK09940::readByte(AK09940_HZM);		//read X Middle byte
		ZMH = AK09940::readByte(AK09940_HZH);		//read X High byte
		tempRead = AK09940::readByte(AK09940_TMPS);	//read temperature data
		AK09940::getDataStatus();					//get the data status -- Valid and No Overflow -- should these be saved to LunaSat if the data read isnt saved, data and status mismatch in LunaSat data???


		if (AK09940::dataStatus.dataValid && !AK09940::dataStatus.dataOverflow)		//if the data status is good save all raw data to LunaSat sensor data structure.	
		{

			AK09940::rawData.xMagLow = XML;
			AK09940::rawData.xMagMid = XMM;
			AK09940::rawData.xMagHigh = XMH;
			AK09940::rawData.yMagLow = YML;
			AK09940::rawData.yMagMid = YMM;
			AK09940::rawData.yMagHigh = YMH;					//stores to LunaSat data structure
			AK09940::rawData.zMagLow = ZML;
			AK09940::rawData.zMagMid = ZMM;
			AK09940::rawData.zMagHigh = ZMH;
			AK09940::rawData.tempByte = tempRead;
			return true;

		}

		else
		{
			Serial.println(" ERROR in getRawData: Data invalid or data overflow.");				//If things break
			return false;
		}
		

	}

	else
	{
		Serial.println("ERROR in getRawData: Data Not Ready.");						//Data isnt ready to be read!
		return false;
	}
	

	
}




float AK09940::calcTemp(uint8_t tempByte)			//converts a temperature byte to celcius
{ 
	float tempDec = float(tempByte);
	float tOut;										//
	tOut = 30.0 - tempDec/1.72;

	if (AK09940::sensorDebug)
	{
		Serial.println("Converting temperature byte to celcius");
	}

	return tOut;
}

int32_t AK09940::interpret18BitAs32Bit(int32_t input) 			//The output of the magnetometer is an 18 bit output, for ease of use it is converted to a 32bit output)
{
	if (bitRead(input,17)==0) 									//if the MSB of the 18 bit output is 0 - this means output is positive
	{
		for(int i=31;i>=17;i--) {								//set all bits from [32:18] to 0 so that the output is positive and unchanged by the extra leading bits
			bitWrite(input,i,0);
		}
	}
	else if (bitRead(input,17)==1) 								//if the MSB of the 18 bit output is 1, the value is negative
	{											
		for(int i=31;i>=17;i--) 
		{
			bitWrite(input,i,1);								//so all prior bits [32:18] must be set to 1 to have a negative output with the value unchanged by the prior bits 
		}
	}
	else if (AK09940::sensorDebug)
	{
		Serial.println("FATAL ERROR IN INTERPRETING 18 BIT INT AS 32 BIT");		//if things break!
	}
	return input;
}

float AK09940::calcMag(uint8_t lowByte, uint8_t midByte, uint8_t highByte) 
{			
	uint8_t corrected_hByte;
	uint8_t hByte_MSB_Remove = 0b00000011; // removes all redundant data past the 18th bit, bits [18:24] all have the same value
	int32_t rawMag18, rawMag32;				//one variable to store 18 bit value, one to store the converted 32 bit variable
	float mag;
	corrected_hByte = (highByte && hByte_MSB_Remove); 
	rawMag18 = (corrected_hByte<<16 | midByte<<8 | lowByte); 			//Stores 18 bit int in 32 bit variable
	rawMag32 = interpret18BitAs32Bit(rawMag18); 					//Converts from 18 bit to 32 bit decimal
	mag = 10 * rawMag32; 											//output is in unit of nT
	return mag;														//return magnetic field data

}




void AK09940::setDriveMode(ak09940_Drive_Mode_t driveMode)			//writes to CNTRL 3 Register. Bits: [7] = FIFO, [6:5] = Drive Mode, [4:0] = Measurement Mode
{
	uint8_t FIFOData = uint8_t(AK09940::FIFOEnabled);
	uint8_t measurementMode = AK09940::getMeasurementMode();

	uint8_t writeData = ( FIFOData <<7 | measurementMode << 5 | driveMode );

	AK09940::writeByte( AK09940_CNTL_3 , writeData);
}

ak09940_Drive_Mode_t AK09940::getDriveMode(void)
{
	uint8_t byteRead = AK09940::readByte(AK09940_CNTL_3);

	uint8_t removeByte = 0b01100000;
	uint8_t driveMode = ((byteRead && removeByte) >> 5);

	return driveMode;
}


void AK09940::setMeasurementMode(ak09940_Measurement_Mode_t measurementMode)	//writes to CNTRL 3 Register. Bits: [7] = FIFO, [6:5] = Drive Mode, [4:0] = Measurement Mode
{
	uint8_t driveMode = AK09940::getDriveMode();

	uint8_t FIFOData = uint8_t (AK09940::FIFOEnabled);
	uint8_t writeData = (FIFOData << 7 | measurementMode << 5 | driveMode );

	if (AK09940::sensorDebug)
	{
		Serial.println("Measurement Mode Set");
		Serial.print("New Mode: ");
		Serial.println( ak09940_Measurement_Mode_t (measurementMode) );
	}

	AK09940::writeByte( AK09940_CNTL_3 , writeData );
}

ak09940_Measurement_Mode_t AK09940::getMeasurementMode(void)
{
	uint8_t byteRead = AK09940::readByte(AK09940_CNTL_3);

	uint8_t removeByte = 0b00011111;										//this will delete FIFO and drive mode data, leaving only the measurement mode
	uint8_t measurementMode = ( byteRead && removeByte );					//combine the data to remove and data recieved to get our final data

	if (AK09940::sensorDebug)
	{
		Serial.println("Measurement Mode Retreived");
		Serial.print("Current mode set: ");
		Serial.println( ak09940_Measurement_Mode_t (measurementMode) );
	}

	return measurementMode;

}


void AK09940::setWatermarkMode(uint8_t watermarkLevel)		//must be between 0 && 7
{
	ak09940_Measurement_Mode_t currentMeasurementMode = AK09940::getMeasurementMode();

	if (currentMeasurementMode != POWER_DOWN )			//verify mode is POWER_DOWN
	{
		AK09940::setMeasurementMode(POWER_DOWN);			// must be in power down mode in order to write to this register

		if (AK09940::sensorDebug)						//Debug for print statements
		{
			Serial.println("Sensor not in POWER_DOWN mode! Changing mode to POWER_DOWN mode in order to write to register");
		}
	}

	if (watermarkLevel >= 0 && watermarkLevel <= 7 )		//verify input is between 1 and 8
	{
		AK09940::writeByte(AK09940_CNTL_1, (watermarkLevel));		//sets the watermark Level as requested
		AK09940::setMeasurementMode(currentMeasurementMode);			// restores sensor mode before function was called
		
		if (AK09940::sensorDebug)									//debug print statments
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
		AK09940::writeByte( AK09940_CNTL_1 , 0 );				// if input is not between 0 and 7 - default watermark level = 1
		AK09940::setMeasurementMode(currentMeasurementMode);		//set measurement mode back to previous mode

		if (AK09940::sensorDebug)
		{
			Serial.println("Error, value entered is not between 0 and 7.");
			Serial.println("Watermark level has been set to a default of 1 data set; WM[2:0]=0 ");
			Serial.print("Measurement mode has been changed back to: ");
			Serial.println(currentMeasurementMode);
		}

	}
	



}

uint8_t AK09940::getWatermarkMode(void)		//prints level of watermark
{
	uint8_t currentWatermarkMode;									//stores watermark level as interger
	currentWatermarkMode = AK09940::readByte(AK09940_CNTL_1);		//read byte that stores the current water mark mode

	if (AK09940::sensorDebug)
	{
		Serial.print("Current Watermark Mode:");
		Serial.println(currentWatermarkMode);
	}

	return currentWatermarkMode;									//return that value

}




void AK09940::setFIFO(bool FIFOState)
{
	uint8_t writeData;
	uint8_t measurementMode = AK09940::getMeasurementMode();
	uint8_t driveMode = AK09940::getDriveMode();

	writeData = (uint8_t(FIFOState) << 7 | driveMode << 5 | measurementMode);


	AK09940::writeByte(AK09940_CNTL_3, writeData);

	if (AK09940::sensorDebug)
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

	AK09940::FIFOEnabled = AK09940::getFIFOState();

}


bool AK09940::getFIFOState(void)
{
	uint8_t byteRead = AK09940::readByte(AK09940_CNTL_3);
	uint8_t FIFOState = (byteRead >> 7);
	

	if (AK09940::sensorDebug)
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

	return bool(FIFOState);

}


uint8_t AK09940::getNumDataInBuffer(void)		//gets the number of data points stored in the FIFO buffer
{
	uint8_t byteRead = AK09940::readByte(AK09940_ST_1);		//

	uint8_t numData = (byteRead>>1); 						//shift right one bit to remove dataReady information

	if (AK09940::sensorDebug)
	{
		Serial.print("Current number of datasets available in buffer: ");
		Serial.println(numData);

	}

	return numData;

}

void AK09940::setDebugMode(bool state)
{
	AK09940::sensorDebug = state;

	if (AK09940::sensorDebug)
	{
		Serial.println("Sensor Debug mode now enabled");
		Serial.println("Each line of code executed will print out what the code is doing!");
	}
}

void AK09940::softReset(void)		//sets the pin to the soft reset mode, resets all registers to values defined on the data sheet.
{
	if (AK09940::sensorDebug)
	{
		Serial.println("SOFT RESET COMMAND RECIEVED: RESETTING AK09940 NOW");
	}

	AK09940::writeByte(AK09940_CNTL_4 , 1);
}



void AK09940::getDataStatus (void)								// This function should be called after reading register data in order to read the Status 2 pin, as required, once data is read.
{
	uint8_t dataStatusByte = AK09940::readByte(AK09940_ST_2);	//read the s
	bool dataInvalidFlag, dataOverflowFlag;

	dataInvalidFlag = (bool ((dataStatusByte && 0b00000010) >> 1 ));		//combine remove byte with the Status 2 byte in order to get only the Data Invalid bit. Shift right one bit for easy boolean assignment.
	AK09940::dataStatus.dataValid = !dataInvalidFlag;					//default sensor data is "data invalid", we flipped to "data Valid" for ease of code reading and writing

	dataOverflowFlag = bool ((dataStatusByte && 0b00000001));			// combine remove byte with the Status 2 byte in order to get only the Data overflow bit. Already shifted as far right as possible.
	AK09940::dataStatus.dataOverflow = dataOverflowFlag;					//asign our overflow flag status to the sensor data

	if (AK09940::sensorDebug)											//print statements for debuging
	{
		if (dataInvalidFlag)											//prints if the data is invalid
		{
			Serial.println("ERROR: AK09940 Reports invalid data. "); 
		}

		if (dataOverflowFlag)											//prints if FIFO buffer was overflowed
		{
			if (AK09940::FIFOEnabled)								//Overflow bit means different things if FIFO is on or off. Here it is on
			{
				Serial.println("Data Overflow! A new set of data was measured when the FIFO buffer was full. Data set over flowed from buffer.");
			}
			else														//print Overflow  erro if FIFO is disabled
			{
				Serial.println("Data Overflow! FIFO Disabled. Data skipped in continious measurement.");
			}
			
		}

		else if (!dataInvalidFlag && !dataOverflowFlag)				//Check for valid data and no data overflow
		{
			Serial.println("Data Valid -- No Data Overflow");
		}

	}


}



bool AK09940::dataReady(void)								//checks for the data ready bit status, should be called before data measurement
{															//function status: To be Tested
	uint8_t readByte = AK09940::readByte(AK09940_ST_1);		//read the data available at the status 1 register
	uint8_t removeByte = 0b00000001;						//removes any other info from FNUM bits (Refer to datasheet)
	bool dataReady = bool(readByte && removeByte);			// convert to boolean value
	

	if (AK09940::sensorDebug)								//sensorDebug print statments
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


	return dataReady;										// return boolean statement
}

