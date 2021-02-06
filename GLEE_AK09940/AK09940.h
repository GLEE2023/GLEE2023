/*
*Description: This is an Arduino (C++) Header file required for the AK09940 Magnetometer
*Project Info: Created For GLEE (The Great Lunar Expedition for Everyone)
*Library Author: John Walker Johnson
*Library Contributors: Lawson Nerenberg
*Library Created on: July 13th 2020
*Last Modified on: Nov 21 2020
*Primary Resources Used in Creation:
*AK09940 Datasheet
*Arduino Wire Library Reference Guide
*
*/

#ifndef AK09940_H
#define AK09940_H

#include <Arduino.h>
#include "GLEE_Sensor.h"

//BEGIN REGISTER MAP DEFINITIONS
#define AK09940_WAI_1 0x00	//(READ) Who Am I Register #1 - Company ID
#define AK09940_WAI_2 0x01	//(READ) Who Am I Register #2 - Device ID
#define AK09940_ST_1 0x10	//(READ) Status Register - Data Status 1
#define AK09940_HXL 0x11	//(READ) Magnetic Field Measurement Data - X-Axis Low Byte
#define AK09940_HXM 0x12	//(READ) Magnetic Field Measurement Data - X-Axis Middle Byte	
#define AK09940_HXH 0x13	//(READ) Magnetic Field Measurement Data - X-Axis High Byte
#define AK09940_HYL 0x14	//(READ) Magnetic Field Measurement Data - Y-Axis Low Byte
#define AK09940_HYM 0x15	//(READ) Magnetic Field Measurement Data - Y-Axis Middle Byte
#define AK09940_HYH 0x16	//(READ) Magnetic Field Measurement Data - Y-Axis High Byte
#define AK09940_HZL 0x17	//(READ) Magnetic Field Measurement Data - Z-Axis Low Byte
#define AK09940_HZM 0x18	//(READ) Magnetic Field Measurement Data - Z-Axis Middle Byte
#define AK09940_HZH 0x19	//(READ) Magnetic Field Measurement Data - Z-Axis High Byte
#define AK09940_TMPS 0x1A	//(READ) Temperature Measurement Data
#define AK09940_ST_2 0x1B	//(READ) Status Register - Data Status 2
#define AK09940_CNTL_1 0x30	//(READ/WRITE) Control Settings #1
#define AK09940_CNTL_2 0x31	//(READ/WRITE) Control Settings #2
#define AK09940_CNTL_3 0x32	//(READ/WRITE) Control Settings #3
#define AK09940_CNTL_4 0x33	//(READ/WRITE) Control Settings #4
//END REGISTER MAP DEFINITIONS

//AK09940 Sensor ADDRESS as given by data sheet: 0x0C
#define AK09940_SENSOR_ADDR 0x0C				

// The sensor Class defines the following data types
// TODO: Readme documentation for use of each type

typedef struct ak09940_RawData_s {
	// This structure represents raw sensor readings: temp, and magnetic x,y,z output 

	uint8_t xMagLow;							//x magnetic output - X-Axis Low byte
	uint8_t xMagMid;							//x magnetic output - X-Axis Middle byte
	uint8_t xMagHigh;							//x magnetic output - X-Axis High byte
	uint8_t yMagLow;							//Y magnetic output - Y-Axis Low byte
	uint8_t yMagMid;							//Y magnetic output - Y-Axis Middle byte
	uint8_t yMagHigh;							//Y magnetic output - Y-Axis High byte
	uint8_t zMagLow;							//Z magnetic output - Z-Axis Low byte
	uint8_t zMagMid;							//Z magnetic output - Z-Axis Middle byte
	uint8_t zMagHigh;							//Z magnetic output - Z-Axis High byte
	uint8_t tempByte;							//Temperature output - Temperature byte
};

typedef struct ak09940_CalculatedData_s {
	// This structure represents processesed temperature, and magnetic (x,y,z) output 

	int32_t xMag;
	int32_t yMag;
	int32_t zMag;
	float temperature;
};

typedef struct ak09940_DataStatus_s{
	bool dataValid;
	bool dataOverflow;
};

typedef enum ak09940_Measurement_Mode_t{
	// Sensor Modes From DataSheet
	// TODO: Write readme documentation for measurement mode (what each bit represents)

	POWER_DOWN=0b00000,							// These sensor modes and values
	SINGLE_MEASURE = 0b00001,					// represent bits [4:0] of the CNTL3 Byte
	CONT_MEASURE_1 = 0b00010,
	CONT_MEASURE_2 = 0b00100,
	CONT_MEASURE_3 = 0b00110,
	CONT_MEASURE_4 = 0b01000,
	CONT_MEASURE_5 = 0b01010,
	CONT_MEASURE_6 = 0b01100,
	SELF_TEST = 0b10000,
 };

typedef enum ak09940_Drive_Mode_t {
	// Drive mode options from data sheet
	// TODO: Write readme documentation for drive mode (what each bit represents)

	LOW_NOISE_1=0b00,
	LOW_NOISE_2=0b01,
	LOW_POWER_1=0b10,
	LOW_POWER_2=0b11,
};

class AK09940: public Sensor{
	// Primaray Magnetometor class inharents parent sensor class variables and methods
	
	public:
	
		// Initialization for Sensor Data, Sensor Info and Data Structures 

		ak09940_RawData_s rawData;
		ak09940_DataStatus_s dataStatus;
		ak09940_CalculatedData_s calculatedData;	
		ak09940_Measurement_Mode_t measurementMode;
		ak09940_Drive_Mode_t driveMode; 
		uint8_t watermarkLevel;
		bool FIFOEnabled;

		//Sensor Functions 
		AK09940(int _id = 0, bool _debug = true);						//constructor
		
		void getCalculatedData(void);		//get data data converted to nT and Celcius

		// Setter for measurement mode expects preconstructed sensor measurement mode configuration inform of the custom datatype
		void setMeasurementMode(ak09940_Measurement_Mode_t sensorMeasurementMode);

		// Getter which returns measurement mode in the form of the custom datatype	
		ak09940_Measurement_Mode_t getMeasurementMode(void);							

		// TODO: Some of these discriptions need to be revisited (What is watermark level?)

		// Setter for size of watermark level. When the number of sets of data in buffer exceeds the Watermark level, the data ready pin and bit turn on. 
		void setWatermarkMode(uint8_t steps);											
		
		// Getter for Water Mark level. Output is between 0 and 7, add one to output to determine watermark level. (i.e. 0 means watermark set at 1, 2 means watermark is set at 3, etc.)
		uint8_t getWatermarkMode(void);													

		// Setter for Drive Mode. 4 possible options, each with different noise rating and power consumption.
		void setDriveMode(ak09940_Drive_Mode_t newMode);														
		
		// Getter for drive mode returns drivemode info in the form of the custom data type.
		ak09940_Drive_Mode_t getDriveMode(void);										
		
		// Setter for FIFO Status -- "First in first out" -- a data buffer capable of storing up to 8 sets of data from registers HXL to TMPS
		void setFIFO(bool FIFOState);													
		
		// Get current FIFO status. True = FIFO enabled, False = FIFO Disabled
		bool getFIFOState(void);														
		
		// Getter for returning number of data sets currently stored in FIFO buffer
		uint8_t getNumDataInBuffer(void);												

		// Setter for debug mode
		void setDebugMode(bool state);


		// General Methods

		// Who Am I method for checking proper sensor initialization, returns true if all expected registers are valid and data returned correctly
		bool ak09940WAI(void);

		// Soft Rest TODO: Cycles sensor power?
		void softReset(void);															

		// Method which converts 18 bit magnetic sensor output to 32 bit output for ardiuno
		int32_t interpret18BitAs32Bit(int32_t input);									

		// TODO: Suggestion that getRawData returns raw data as appropriate data type, possible redundancy in get data status
		// Getter that reads the registers and returns true if data is read and stored in the sensors raw data data structure
		bool getRawData(void);

		// Refactoring raw data here
		sensor_float_vec_t getrawData(void);

		// Method for library structure testing
		sensor_float_vec_t getRawData_fuzzed(void);		

		// Method which returns boolean signifying that data is ready
		bool dataReady(void);			

		// Getter for Data Status which verifies that the data was correctly read, with no data overwrites or gaps in measurements 
		// Note: Must be called after measuring data from registers, it 
		void getDataStatus(void);		

		private:

			// Private Data Processing Methods 
			
			// Method for Calculating magnetic field strenght.
			// Returns mag field strength in units of nT given low, mid, and high bytes from one axis' sample
			float calcMag(uint8_t lowByte , uint8_t middleByte , uint8_t highByte);		

			// Method for converting raw digital temperature reading into degrees celcius
			float calcTemp(uint8_t tempByte);										
};

#endif
