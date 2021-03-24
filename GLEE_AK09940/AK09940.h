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

#include "GLEE_Sensor.h"

// Read Only Registers
#define AK09940_WAI_1 0x00		    // WAI Company ID (Typically 0x48)
#define AK09940_WAI_2 0x01		    // WAI Device ID (Typically)

#define AK09940_ST_1 0x10			// Status Register - Data Status 1
#define AK09940_HX 0x11				// Magnetic Field Measurement Data - X-Axis Low Byte
#define AK09940_HY 0x14				// Magnetic Field Measurement Data - Y-Axis Low Byte
#define AK09940_HZ 0x17				// Magnetic Field Measurement Data - Z-Axis Low Byte
#define AK09940_TMPS 0x1A			// Temperature Measurement Data

#define AK09940_ST_2 0x1B		    // Status Register 2

// Control settings registers (all 1xByte)
#define AK09940_CNTL_1 0x30				// Watermark settings 		3 bit value D[0:2] = [WM[2],WM[1],WM[0]] (page 34 datasheet)
#define AK09940_CNTL_2 0x31				// Temperature settings 	1 bit value D[6] = TEM[0] (temperature enable)
#define AK09940_CNTL_3 0x32				// Operational settings 	8 bit value D[0:7] = [FIFO, MT[1], MT[0], MODE[4], MODE[3], MODE[2], MODE[1], MODE[0]]
#define AK09940_CNTL_4 0x33				// Reset control			1 bit value D[0] = SRST (soft reset)

// AK09940 Sensor ADDRESS as given by data sheet: 0x0C
#define AK09940_SENSOR_ADDR 0x0C				

// The sensor Class defines the following data types
// TODO: Readme documentation for use of each type

// This structure represents raw sensor readings: temp, and magnetic x,y,z output 
typedef struct{
	uint32_t x;	
    uint32_t y;
    uint32_t z;
	uint8_t temp;	//Temperature output - Temperature byte
}AK_RawData_t;

typedef struct{
    sensor_float_vec_t magnetic;
    float strength;
    float temp;
}AK_Sample_t;

// This structure represents processesed temperature, and magnetic (x,y,z) output 
// typedef struct ak09940_CalculatedData_s{
// 	int32_t xMag;
// 	int32_t yMag;
// 	int32_t zMag;
// 	float temperature;
// };

// typedef struct ak09940_DataStatus_s{
// 	bool dataValid;
// 	bool dataOverflow;
// };

// Sensor Modes From DataSheet
// TODO: Write readme documentation for measurement mode (what each bit represents)
enum ak09940_Measurement_Mode_t{
	POWER_DOWN = 0b00000,						// These sensor modes and values
	SINGLE_MEASURE = 0b00001,					// represent bits [4:0] of the CNTL3 Byte
	CONT_MEASURE_1 = 0b00010,
	CONT_MEASURE_2 = 0b00100,
	CONT_MEASURE_3 = 0b00110,
	CONT_MEASURE_4 = 0b01000,
	CONT_MEASURE_5 = 0b01010,
	CONT_MEASURE_6 = 0b01100,
	SELF_TEST = 0b10000,
};

// Drive mode options from data sheet
// TODO: Write readme documentation for drive mode (what each bit represents)
enum ak09940_Drive_Mode_t{
	LOW_POWER_1 = 0b00,
	LOW_POWER_2 = 0b01,
	LOW_NOISE_1 = 0b10,
	LOW_NOISE_2 = 0b11,
};

// TODO: Some of these discriptions need to be revisited (What is watermark level?)
// Primary Magnetometor class inharents parent sensor class variables and methods
class AK09940: public Sensor{
	// Initialization for Sensor Data, Sensor Info and Data Structures 
	public:
		AK09940(int _id, bool _debug = false);

        void begin(void);
        void readWAI(void);
        void setOpMode(bool fifo, ak09940_Drive_Mode_t driveMode, ak09940_Measurement_Mode_t measureMode);
		AK_RawData_t getRawData(void);

        float getTemperature(uint8_t rawTemp);
        sensor_float_vec_t getMagnetic(AK_RawData_t rawData);
        float getMagFieldStrength(sensor_float_vec_t magnetic);

        bool dataReady(void);

        AK_Sample_t getSample(void);

    private:
        // ak09940_Measurement_Mode_t measurementMode;
        // ak09940_Drive_Mode_t driveMode; 
        bool FIFO;
        uint8_t watermarkLevel;  
        ak09940_Measurement_Mode_t measurementMode;
        ak09940_Drive_Mode_t driveMode;
};

#endif
