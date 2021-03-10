/*
*Description: This is an Arduino (C++) .cpp file required for the AK09940 Magnetometer
*Project Info: Created For GLEE (The Great Lunar Expedition for Everyone)
*Library Author: John Walker Johnson
*Library Contributors: Lawson Nerenberg
*Library Created on: July 13th 2020
*Last Modified on: Nov. 22 2020
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

/**
 * Constructor
 * Parameters: the sensor ID as an integer and the debug mode as a booelan
 * Returns: none
 * This is the constructor for the AK09940 class. 
**/
AK09940::AK09940(int _id, bool _debug){
	//constructor - Function Status: TBR -- add inital sensor settings?
	AK09940::info.id = _id;
	AK09940::info.name = "AK09940 Magnetometer";		//assign sensor name
	AK09940::info.address = AK09940_SENSOR_ADDR;		//assign sensor i2c address
	AK09940::sensorDebug= _debug;						//debug flag status
	Wire.begin(AK09940::info.address);					//begin i2c comms with sensor, must be called once
	
	if (AK09940::sensorDebug){							//Print Statements for debugging and visualization of what this sensor does!
		Serial.print("Beginning Communication with AK09940");
	}
}

/**
 * Parameters: none
 * Returns: whether the expected registers are valid and if the data returned correctly as a boolean 
 * This function is a Who Am I function for checking proper sensor initialization and will
 *  return true if all the expected registers are valid and if the data returned correctly.
**/

//Internal Who Am I Register verification, returns true if values given by data sheet are correct!
//Function status: TBR
bool AK09940::ak09940WAI(void){

	uint8_t whoAmI_1,whoAmI_2;
	whoAmI_1 = readByte(AK09940_WAI_1);				//Read 1st Who Am I Register
	whoAmI_2 = readByte(AK09940_WAI_2);				//Read 2nd Who Am I register 

	if (AK09940::sensorDebug && (whoAmI_1 != 0x48 || whoAmI_2 != 0xA1)){
		//check for debug and incorrect sensor values
		Serial.println("ERROR: WHO AM I REGISTERS NOT RETURNING CORRECT VALUES");
		Serial.print("WHO_AM_I_1 REGISTER SHOULD RETURN 48, WHO_AM_I_1 RETURNS: ");
		Serial.println(whoAmI_1);
		Serial.print("WHO_AM_I_2 REGISTER SHOULD RETURN A1, WHO_AM_I_2 RETURNS: ");
		Serial.println(whoAmI_2);
		return false;
	}

	else if (AK09940::sensorDebug){
		//check for just debug print statements
		Serial.print("WHO_AM_I_1 REGISTER SHOULD RETURN 48, WHO_AM_I_1 RETURNS: ");
		Serial.println(whoAmI_1);
		Serial.print("WHO_AM_I_2 REGISTER SHOULD RETURN A1, WHO_AM_I_2 RETURNS: ");
		Serial.println(whoAmI_2);
		//return true if both WAI read correctly
		return true;														
	}

	else if (whoAmI_1 == 0x48 || whoAmI_2 == 0xA1){
		// no debug and WAI registers return correct value
		// returns true if WAI registers are correctly read
		return true;										
	}
	else {
		// return falst if not read correctly
		return false;										
	}
	
}

/**
 * Parameters: none
 * Returns: none
 * This function converts raw data bytes to nT and Celcius using the
 * calcMag and calcTemp private functions.
**/
void AK09940::setCalculatedData(void){
	// Converts raw data bytes to nT and Celcius, change function name??
	// Only converts data that was read with no errors, no overflow, no invalid data
	AK09940::calculatedData.xMag = AK09940::calcMag(AK09940::rawData.xMagLow, AK09940::rawData.xMagMid, AK09940::rawData.xMagHigh);		
	AK09940::calculatedData.yMag = AK09940::calcMag( AK09940::rawData.yMagLow, AK09940::rawData.yMagMid, AK09940::rawData.yMagHigh);
	AK09940::calculatedData.zMag = AK09940::calcMag( AK09940::rawData.zMagLow, AK09940::rawData.zMagMid, AK09940::rawData.zMagHigh);
	
	//Assigns results to sensor data structure 
	AK09940::calculatedData.temperature = AK09940::calcTemp(AK09940::rawData.tempByte);
}


/**
 * Parameters: none
 * Returns: whether data is read successfully as a boolean 
 * This function reads the raw data. New function is defined below using
 * the new sensor data type.
**/
bool AK09940::getRawData(void){
	// TBR -- Redundancy?? 
	uint8_t XML, XMM , XMH;			//variables to hold low, mid, and high bytes for X-axis magnetic field 
	uint8_t YML, YMM , YMH;			//variables to hold low, mid, and high bytes for Y-axis magnetic field 
	uint8_t ZML, ZMM , ZMH;			//variables to hold low, mid, and high bytes for Z-axis magnetic field 
	uint8_t tempRead;				//byte to hold temperature data
	
	// Bytes are held here before assigning to lunasat data structure in order to verify data status after reading data

	if (AK09940::dataReady()){
		//data must be ready to be read

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
		AK09940::checkDataStatus();					//check the data status -- Valid and No Overflow -- should these be saved to LunaSat if the data read isnt saved, data and status mismatch in LunaSat data???


		if (AK09940::dataStatus.dataValid && !AK09940::dataStatus.dataOverflow){
			// If the data status is good save all raw data to LunaSat sensor data structure.

			AK09940::rawData.xMagLow = XML;
			AK09940::rawData.xMagMid = XMM;
			AK09940::rawData.xMagHigh = XMH;
			AK09940::rawData.yMagLow = YML;
			AK09940::rawData.yMagMid = YMM;
			AK09940::rawData.yMagHigh = YMH;		//stores to LunaSat data structure
			AK09940::rawData.zMagLow = ZML;
			AK09940::rawData.zMagMid = ZMM;
			AK09940::rawData.zMagHigh = ZMH;
			AK09940::rawData.tempByte = tempRead;
			return true;
		}

		else {
			// If things break
			Serial.println("ERROR in getRawData: Data invalid or data overflow.");				
			return false;
		}
	}

	else {
		// Data isnt ready to be read!
		Serial.println("ERROR in getRawData: Data Not Ready.");						
		return false;
	}
}

/**
 * Parameters: none 
 * Returns: the raw data as a sensor_float_vec_t 
 * This function gets raw data returns raw data in sensor float structure for testing.
**/
sensor_float_vec_t AK09940::getrawData(void){
	// Suggested refactoring example, uses old style for getRawData (updates local rawData structure instance) Probably should refrence a local pointer to a struct
	// get raw data returns raw data in sensor float structure for testing
	
	// Reads raw data and sets local instance 
	AK09940::getRawData();		// By "get" it really means update

	// Updates calculated data instance
	AK09940::setCalculatedData();

	sensor_float_vec_t rawData_;
	rawData_.x = 3.2;
	rawData_.y = 5.2;
	rawData_.z = 6.2;

	return rawData_;
}

/**
 * Parameters: none
 * Returns: the fuzzed raw data as a sensor_float_vec_t 
 * This function gets the fuzzed data for data structure passed through testing.
**/
sensor_float_vec_t AK09940::getRawData_fuzzed(void){
	// fuzzed data return for data structure pass through testing
	// get raw data returns raw data in sensor float structure for testing
	
	// AK09940::getRawData();
	// AK09940::setCalculatedData();

	sensor_float_vec_t rawData_;
	rawData_.x = 3.2;
	rawData_.y = 5.2;
	rawData_.z = 6.2;

	return rawData_;
}

/**
 * Parameters: the raw difital temperature reading as an unsigned 8-bit integer
 * Returns: the calculated temperature in Celsius as a float
 * This function is private and converts raw digital temperature reading into degrees Celcius.
**/
float AK09940::calcTemp(uint8_t tempByte){
	// Converts a temperature byte to celcius 
	float tempDec = float(tempByte);
	float tOut;										
	tOut = 30.0 - tempDec/1.72;

	if (AK09940::sensorDebug){
		Serial.println("Converting temperature byte to celcius");
	}
	return tOut;
}

/**
 * Parameters: the input to be converted as a 32-bit signed integer
 * Returns: the converted value as a 32-bit signed integer
 * This function converts 18 bit magnetic sensor output to 32 bit output for ardiuno.
**/
int32_t AK09940::interpret18BitAs32Bit(int32_t input){
	// The output of the magnetometer is an 18 bit output, for ease of use it is converted to a 32bit output)
	if (bitRead(input, 17) == 0){
		// If the MSB of the 18 bit output is 0 - this means output is positive

		// Set all bits from [32:18] to 0 so that the output is positive and unchanged by the extra leading bits
		for (int i = 31; i >= 17; i--){								
			bitWrite(input, i, 0);
		}
	}
	else if (bitRead(input, 17) == 1){
		// If the MSB of the 18 bit output is 1, the value is negative	
		// so all prior bits [32:18] must be set to 1 to have a negative output with the value unchanged by the prior bits										
		for(int i = 31; i >= 17; i--){
			bitWrite(input, i, 1);								 
		}
	}
	else if (AK09940::sensorDebug){
		Serial.println("FATAL ERROR IN INTERPRETING 18 BIT INT AS 32 BIT");		//if things break!
	}
	return input;
}

/**
 * Parameters: the low byte, middle byte, and high byte all as unsigned 8-bit integers
 * Returns: the magnetic field as a float in nano teslas
 * This function is private and calculates the magnetic field strength for one axis. 
**/
float AK09940::calcMag(uint8_t lowByte, uint8_t midByte, uint8_t highByte){			
	uint8_t corrected_hByte; //save the corrected high byte

	// Remove all redundant data past the 18th bit, bits [18:24] all have the same value
	uint8_t hByte_MSB_Remove = 0b00000011; 

	//one variable to store 18 bit value, one to store the converted 32 bit variable
	int32_t rawMag18, rawMag32;				
	float mag;
	corrected_hByte = (highByte && hByte_MSB_Remove); 
	rawMag18 = (corrected_hByte << 16 | midByte << 8 | lowByte);		//Stores 18 bit int in 32 bit variable
	rawMag32 = interpret18BitAs32Bit(rawMag18); 						//Converts from 18 bit to 32 bit decimal
	mag = 10 * rawMag32; 												//output is in unit of nT
	return mag;															//return magnetic field data
}

/**
 * Parameters: the desired drive mode as an ak09940_Drive_Mode_t enumeration 
 * Returns: none
 * This function sets the drive mode.
**/
void AK09940::setDriveMode(ak09940_Drive_Mode_t driveMode){
	//writes to CNTRL 3 Register. Bits: [7] = FIFO, [6:5] = Drive Mode, [4:0] = Measurement Mode
	uint8_t FIFOData = uint8_t(AK09940::FIFOEnabled); 		//gets FIFO data 
	uint8_t measurementMode = AK09940::getMeasurementMode();
	uint8_t writeData = (FIFOData << 7 | measurementMode << 5 | driveMode);
	AK09940::writeByte(AK09940_CNTL_3 , writeData); 		//sets new measurement mode
}

/**
 * Parameters: none
 * Returns: the currrent drive mode as an ak09940_Drive_Mode_t enumeration
 * This function gets the current drive mode in the form of the custom data
 * type (ak09940_Drive_Mode_t).
**/
ak09940_Drive_Mode_t AK09940::getDriveMode(void){
	uint8_t byteRead = AK09940::readByte(AK09940_CNTL_3); 		//reads register control settings #3
	uint8_t removeByte = 0b01100000;
	uint8_t driveMode = ((byteRead && removeByte) >> 5);
	return driveMode;
}

/**
 * Parameters: the desired measurement mode as an ak09940_Measurement_Mode_t enumeration
 * Returns: none
 * This function sets the measurement mode and expects preconstructed sensor measurement 
 * mode configuration in the form of the custom datatype (ak09940_Measurement_Mode_t enumeration). 
**/
void AK09940::setMeasurementMode(ak09940_Measurement_Mode_t measurementMode){
	// Writes to CNTRL 3 Register. Bits: [7] = FIFO, [6:5] = Drive Mode, [4:0] = Measurement Mode
	uint8_t driveMode = AK09940::getDriveMode();

	uint8_t FIFOData = uint8_t (AK09940::FIFOEnabled); 		//gets FIFO data
	uint8_t writeData = (FIFOData << 7 | measurementMode << 5 | driveMode);

	if (AK09940::sensorDebug){ 		//if in debug mode
		Serial.println("Measurement Mode Set");
		Serial.print("New Mode: ");
		Serial.println(ak09940_Measurement_Mode_t (measurementMode));
	}

	AK09940::writeByte( AK09940_CNTL_3 , writeData);
}

/**
 * Parameters: none
 * Returns: the current measurement mode as an ak09940_Measurement_Mode_t enumeration
 * This function gets the current measurement mode in the form of the custom datatype
 * (ak09940_Measurement_Mode_t).
**/
ak09940_Measurement_Mode_t AK09940::getMeasurementMode(void){
	uint8_t byteRead = AK09940::readByte(AK09940_CNTL_3); //reads register control settings #3

	uint8_t removeByte = 0b00011111; //this will delete FIFO and drive mode data, leaving only the measurement mode
	uint8_t measurementMode = ( byteRead && removeByte); //combine the data to remove and data recieved to get our final data

	if (AK09940::sensorDebug){ 		//if in debug mode
		Serial.println("Measurement Mode Retreived");
		Serial.print("Current mode set: ");
		Serial.println( ak09940_Measurement_Mode_t (measurementMode));
	}
	return measurementMode;
}

/**
 * Parameter: the desired water mark level as an unsigned 8-bit integer
 * Returns: void 
 * This function sets the water mark level with the level being between
 * 0 and 7.
**/
void AK09940::setWatermarkMode(uint8_t watermarkLevel){
	// Must be between 0 && 7
	ak09940_Measurement_Mode_t currentMeasurementMode = AK09940::getMeasurementMode();

	if (currentMeasurementMode != POWER_DOWN){
		// Verify mode is POWER_DOWN

		// Must be in power down mode in order to write to this register
		AK09940::setMeasurementMode(POWER_DOWN);			
		if (AK09940::sensorDebug){
			//Debug for print statements
			Serial.println("Sensor not in POWER_DOWN mode! Changing mode to POWER_DOWN mode in order to write to register");
		}
	}

	if (watermarkLevel >= 0 && watermarkLevel <= 7 ){
		//Verify input is between 1 and 8

		// Set the watermark Level as requested
		AK09940::writeByte(AK09940_CNTL_1, (watermarkLevel));		
		// Restores sensor mode before function was called
		AK09940::setMeasurementMode(currentMeasurementMode);	    
		
		if (AK09940::sensorDebug){
			// Debug print statments
			Serial.print("The watermark level has been set to: ");
			Serial.print(watermarkLevel+1);
			Serial.println(" data sets");
			Serial.print("Measurement mode has been changed back to: ");
			Serial.println(currentMeasurementMode);
		}
	}
	
	else {
		// If input is not between 0 and 7 - default watermark level = 1
		AK09940::writeByte(AK09940_CNTL_1, 0);

		// Set measurement mode back to previous mode				
		AK09940::setMeasurementMode(currentMeasurementMode);		

		if (AK09940::sensorDebug){
			Serial.println("Error, value entered is not between 0 and 7.");
			Serial.println("Watermark level has been set to a default of 1 data set; WM[2:0]=0 ");
			Serial.print("Measurement mode has been changed back to: ");
			Serial.println(currentMeasurementMode);
		}
	}
}

/**
 * Parameters: none
 * Returns: the current water mark mode as an unisgned 8-bit interger
 * This function gets the current watermark mode.
**/
uint8_t AK09940::getWatermarkMode(void){
	// Method to Print level of watermark
	// Stores watermark level as 8 un-interger
	uint8_t currentWatermarkMode;

	// Read byte that stores the current water mark mode
	currentWatermarkMode = AK09940::readByte(AK09940_CNTL_1);		

	if (AK09940::sensorDebug){
		Serial.print("Current Watermark Mode:");
		Serial.println(currentWatermarkMode);
	}

	// Return water mark mode
	return currentWatermarkMode;									
}

/**
 * Parameters: the desired FIFO state as a boolean (true => enabled, false => disabled)
 * Returns: none
 * This function sets the state of the FIFO buffer.
**/
void AK09940::setFIFO(bool FIFOState){
	uint8_t writeData;
	uint8_t measurementMode = AK09940::getMeasurementMode(); //get the current measurement mode
	uint8_t driveMode = AK09940::getDriveMode(); //get the current drive mode

	writeData = (uint8_t(FIFOState) << 7 | driveMode << 5 | measurementMode); 

	AK09940::writeByte(AK09940_CNTL_3, writeData); //writes the FIFO state to control settings number 3 register

	if (AK09940::sensorDebug){ //if in sensor debug mode
		if (FIFOState){
			Serial.println("FIFO Enabled");
		}
		else {
			Serial.println("FIFO Disabled");
		}
	}
	AK09940::FIFOEnabled = AK09940::getFIFOState();
}

/**
 * Parameters: none
 * Returns: the state of FIFO as a boolean
 * This function gets whether the FIFO state
 * is enabled or disabled.
 **/
bool AK09940::getFIFOState(void){
	uint8_t byteRead = AK09940::readByte(AK09940_CNTL_3);		//reads from control settings number 3 register
	uint8_t FIFOState = (byteRead >> 7);		//7 bit right shift

	if (AK09940::sensorDebug){
		if (bool(FIFOState)){ 
			//Enabled if true
			Serial.println("FIFO Enabled");
		}
		else { 
			//Disabled if false
			Serial.println("FIFO Disabled");
		}
	}
	return bool(FIFOState);
}


/**
 * Parameters: none
 * Returns: the number of data sets stored in the FIFO buffer at the moment as an
 * unsigned 8-bit integer
 * This function is a getter for number of data sets 
 * currently stored in FIFO buffer.
**/
uint8_t AK09940::getNumDataInBuffer(void){
	// Gets the number of data points stored in the FIFO buffer
	uint8_t byteRead = AK09940::readByte(AK09940_ST_1);		//Reads register status 1

	uint8_t numData = (byteRead >> 1);		//shift right one bit to remove dataReady information

	if (AK09940::sensorDebug){
		Serial.print("Current number of datasets available in buffer: ");
		Serial.println(numData);

	}
	return numData;
}


/**
 * Parameters: whether to debug as a boolean
 * Returns: none
 * This function sets the sensor to debug mode.
**/
void AK09940::setDebugMode(bool state){
	AK09940::sensorDebug = state; //Sets sensor debug

	if (AK09940::sensorDebug){
		Serial.println("Sensor Debug mode now enabled");
		Serial.println("Each line of code executed will print out what the code is doing!");
	}
}

/**
 * Parameters: none
 * Returns: none
 * This function sets the pin to the soft rest mode, meaning
 * that it resets all the registers to the values defined on 
 * the data sheet.
**/
void AK09940::softReset(void){
	if (AK09940::sensorDebug){ 		//if in sensor debug mode
		Serial.println("SOFT RESET COMMAND RECIEVED: RESETTING AK09940 NOW");
	}
	AK09940::writeByte(AK09940_CNTL_4, 1);
}


/**
 * Parameters: none
 * Returns: none
 * This function is for checking the Data Status which verifies 
 * that the data was correctly read, with no data overwrites 
 * or gaps in measurements. It must be called after measuring 
 * data from registers.
**/
void AK09940::checkDataStatus(void){
	// This function should be called after reading register data in order to read the Status 2 pin, as required, once data is read.
	uint8_t dataStatusByte = AK09940::readByte(AK09940_ST_2);		//Read the s
	bool dataInvalidFlag, dataOverflowFlag;

	// Combine remove byte with the Status 2 byte in order to get only the Data Invalid bit. Shift right one bit for easy boolean assignment.
	dataInvalidFlag = (bool((dataStatusByte && 0b00000010) >> 1 ));

	// default sensor data is "data invalid", we flipped to "data Valid" for ease of code reading and writing	
	AK09940::dataStatus.dataValid = !dataInvalidFlag;					

	// combine remove byte with the Status 2 byte in order to get only the Data overflow bit. Already shifted as far right as possible.
	dataOverflowFlag = bool ((dataStatusByte && 0b00000001));	

	// Assign our overflow flag status to the sensor data
	AK09940::dataStatus.dataOverflow = dataOverflowFlag;				

	if (AK09940::sensorDebug){
		// Print statements for debuging
		if (dataInvalidFlag){
			// Prints if the data is invalid
			Serial.println("ERROR: AK09940 Reports invalid data. "); 
		}
		if (dataOverflowFlag){
			// Prints if FIFO buffer was overflowed
			if (AK09940::FIFOEnabled){
				// Overflow bit means different things if FIFO is on or off. Here it is on
				Serial.println("Data Overflow! A new set of data was measured when the FIFO buffer was full. Data set over flowed from buffer.");
			}
			else {
				// Print Overflow  erro if FIFO is disabled
				Serial.println("Data Overflow! FIFO Disabled. Data skipped in continious measurement.");
			}
		}
		else if (!dataInvalidFlag && !dataOverflowFlag){
			//Check for valid data and no data overflow
			Serial.println("Data Valid -- No Data Overflow");
		}
	}
}


/**
 * Parameters: none
 * Returns: whether the data is ready to be read as a boolean
 * This function checks whether the data is ready to be read by
 * first reading the data available at the at the status 1 register 
 * then removing extra info and seeing if data is ready.
 **/
bool AK09940::dataReady(void){
	// checks for the data ready bit status, should be called before data measurement
	// function status: To be Tested
	uint8_t readByte = AK09940::readByte(AK09940_ST_1);		//read the data available at the status 1 register
	uint8_t removeByte = 0b00000001;						//removes any other info from FNUM bits (Refer to datasheet)
	bool dataReady = bool(readByte && removeByte);			// convert to boolean value
	

	if (AK09940::sensorDebug){
		// SensorDebug print statments
		if (dataReady){
			Serial.println("Data ready to be read");		
		}
		else {
			Serial.println("Error: Data not ready to be read");
		}
	}
	return dataReady; 		// return boolean statement
}

