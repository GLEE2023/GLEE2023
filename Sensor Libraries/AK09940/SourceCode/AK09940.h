/*
*Description: This is an Arduino (C++) Header file required for the AK09940 Magnetometer
*Project Info: Created For GLEE (The Great Lunar Expedition for Everyone)
*Library Author: John Walker Johnson
*Library Contributors: Lawson Nerenberg
*Library Created on: July 13th 2020
*Last Modified on: October 7th 2020
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

#define AK09940_SENSOR_ADDR 0x0C				//AK09940 Sensor ADDRESS

typedef struct ak09940_RawData_s 				//structure to hold sensor temp, and magnetic x,y,z output 
{				
	uint8_t xMagLow;							//x magnetic output - X-Axis Low byte
	uint8_t xMagMid;							//x magnetic output - X-Axis Middle byte
	uint8_t xMagHigh;							//x magnetic output - X-Axis High byte
	uint8_t yMagLow;							//Y magnetic output - Y-Axis Low byte
	uint8_t yMagMid;							//Y magnetic output - Y-Axis Middle byte
	uint8_t yMagHigh;							//Y magnetic output - Y-Axis High byte
	uint8_t zMagLow;							//Z magnetic output - Z-Axis Low byte
	uint8_t zMagMid;							//Z magnetic output - Z-Axis Middle byte
	uint8_t zMagHigh;							//Z magnetic output - Z-Axis High byte
	uint8_t tempByte;								//Temperature output - Temperature byte
};

typedef struct ak09940_CalculatedData_s
{
	int32_t xMag;
	int32_t yMag;
	int32_t zMag;
	float temperature;
};

typedef struct ak09940_DataStatus_s
{
	bool dataValid;
	bool dataOverflow;
};



typedef enum ak09940_Measurement_Mode_t			//Sensor Modes From DataSheet
{									
	POWER_DOWN=0b00000,							//These sensor modes and values
	SINGLE_MEASURE = 0b00001,					//represent bits [4:0] of the CNTL3 Byte
	CONT_MEASURE_1 = 0b00010,
	CONT_MEASURE_2 = 0b00100,
	CONT_MEASURE_3 = 0b00110,
	CONT_MEASURE_4 = 0b01000,
	CONT_MEASURE_5 = 0b01010,
	CONT_MEASURE_6 = 0b01100,
	SELF_TEST = 0b10000,
 };

 typedef enum ak09940_Drive_Mode_t {			//Drive mode options from data sheet
	 LOW_NOISE_1=0b00,
	 LOW_NOISE_2=0b01,
	 LOW_POWER_1=0b10,
	 LOW_POWER_2=0b11,
 };



class AK09940: public Sensor{
	public:
		
		//Sensor Data and Information Variables and Data Structures

		ak09940_RawData_s rawData;
		ak09940_DataStatus_s dataStatus;
		ak09940_CalculatedData_s calculatedData;	
		ak09940_Measurement_Mode_t measurementMode;
		ak09940_Drive_Mode_t driveMode; 
		uint8_t watermarkLevel;
		bool FIFOEnabled;

		//Sensor Functions 
		AK09940();						//constructor
		
		void getCalculatedData(void);		//get data data converted to nT and Celcius


		void setMeasurementMode(ak09940_Measurement_Mode_t sensorMeasurementMode);		//set measurement mode to a predefined sensor measurement mode
		ak09940_Measurement_Mode_t getMeasurementMode(void);							//read and return the current sensor mode to the sensors information 

		void setWatermarkMode(uint8_t steps);											//set size of watermark level. When the number of sets of data in buffer exceeds the Watermark level, the data ready pin and bit turn on. 
		uint8_t getWatermarkMode(void);													//retreive the current water mark level. Output is between 0 and 7, add one to output to determine watermark level. (i.e. 0 means watermark set at 1, 2 means watermark is set at 3, etc.)

		void setDriveMode(ak09940_Drive_Mode_t newMode);														//set sensor drive mode. 4 possible options, each with different noise rating and power consumption.
		ak09940_Drive_Mode_t getDriveMode(void);										//read and return the current sensor drive mode to the sensors information

		void setFIFO(bool FIFOState);													//Set FIFO Status -- "First in first out" -- a data buffer capable of storing up to 8 sets of data from registers HXL to TMPS
		bool getFIFOState(void);														//get current FIFO status. True = FIFO enabled, False = FIFO Disabled
		uint8_t getNumDataInBuffer(void);												//Returns number of data sets currently stored in FIFO buffer

		void setDebugMode(bool state);

		void softReset(void);															//soft reset function sets all registers to values as defined on data sheet


		int32_t interpret18BitAs32Bit(int32_t input);									// for 18 bit magnetic sensor output to 32 bit ardiuno output conversion


		bool getRawData(void);			//reads the registers and returns true if data is read and stored in the sensors raw data data structure

		bool dataReady(void);			//Verifies if the data ready to be read

		void getDataStatus(void);		//Must be called after measuring data from registers, it verifies that the data was correctly read, with no data overwrites or gaps in measurements 


		bool ak09940WAI(void);			//checks for proper sensor internal WAI returns true if all registers and correctly returned

		

		private:

		float calcMag(uint8_t lowByte , uint8_t middleByte , uint8_t highByte);		//converts low, mid, and high bytes form one axis into units of nT
		float calcTemp(uint8_t tempByte);										//converts temp byte to celcius
		


};


#endif

/*class AK09940 {
	
	//This the AK09940 Class containing necessary functions for magnetic field measurements

	//FROM OLD AK09940 library

	public:
		AK09940(void);
		void begin(void);
		bool isConnected(void);
		void writeByte(uint8_t AK09940_reg,uint8_t AK09940_data);
		byte readByte(uint8_t AK09940_reg);
        void whoAmI(void);
        void initialize(void);
		void getStatus(void);
		float getTemperature(void);
		int32_t interpret18BitAs32Bit(int32_t input);
		float getMagX(void);
		float getMagY(void);
		float getMagZ(void);
};
*/


