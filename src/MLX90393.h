/**
 * Description: This is an Arduino (C++) Header file required for the MLX90393 Magnetometer
 * Project Info: Created For GLEE (The Great Lunar Expedition for Everyone)
 * Library Contributors: John Walker Johnson, Lawson Nerenberg
 * Primary Resources Used in Creation:
 * MLX90393 Datasheet (https://media.digikey.com/pdf/Data%20Sheets/AKM%20Semiconductor%20Inc.%20PDFs/MLX90393_Prelim_DS_11-2-18.pdf)
 * Arduino Wire Library Reference Guide
 *
**/

#ifndef MLX90393_H
#define MLX90393_H

#include "GLEE_Sensor.h"

// Read Only Registers
#define MLX90393_WAI_1 0x00		    // WAI Company ID (Typically 0x48)
#define MLX90393_WAI_2 0x01		    // WAI Device ID (Typically)

#define MLX90393_ST_1 0x10			// Status Register - Data Status 1
#define MLX90393_HX 0x11				// Magnetic Field Measurement Data - X-Axis Low Byte
#define MLX90393_HY 0x14				// Magnetic Field Measurement Data - Y-Axis Low Byte
#define MLX90393_HZ 0x17				// Magnetic Field Measurement Data - Z-Axis Low Byte
#define MLX90393_TMPS 0x1A			// Temperature Measurement Data

#define MLX90393_ST_2 0x1B		    // Status Register 2

// Control settings registers (all 1xByte)
#define MLX90393_CNTL_1 0x30				// Watermark settings 		3 bit value D[0:2] = [WM[2],WM[1],WM[0]] (page 34 datasheet)
#define MLX90393_CNTL_2 0x31				// Temperature settings 	1 bit value D[6] = TEM[0] (temperature enable)
#define MLX90393_CNTL_3 0x32				// Operational settings 	8 bit value D[0:7] = [FIFO, MT[1], MT[0], MODE[4], MODE[3], MODE[2], MODE[1], MODE[0]]
#define MLX90393_CNTL_4 0x33				// Reset control			1 bit value D[0] = SRST (soft reset)

// MLX90393 Sensor ADDRESS as given by data sheet: 0x0C
#define MLX90393_SENSOR_ADDR 0x0C				

// The sensor Class defines the following data types
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

// Sensor Modes From DataSheet
enum MLX90393_Measurement_Mode_t{
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
enum MLX90393_Drive_Mode_t{
	LOW_POWER_1 = 0b00,
	LOW_POWER_2 = 0b01,
	LOW_NOISE_1 = 0b10,
	LOW_NOISE_2 = 0b11,
};

// Primary Magnetometor class inharents parent sensor class variables and methods
class MLX90393: public Sensor{
	// Initialization for Sensor Data, Sensor Info and Data Structures 
	public:
		MLX90393(int _id, bool _debug = false);

        void begin(void);
        void readWAI(void);
        void setOpMode(bool fifo, MLX90393_Drive_Mode_t driveMode, MLX90393_Measurement_Mode_t measureMode);
        bool dataReady(void);

        // Getters
        AK_RawData_t getRawData(void);
        
        float getTemperature(uint8_t rawTemp);
        sensor_float_vec_t getMagnetic(AK_RawData_t rawData);
        float getMagFieldStrength(sensor_float_vec_t magnetic);

        AK_Sample_t getSample(void);

    private:
        bool FIFO;
        uint8_t watermarkLevel;  
        MLX90393_Measurement_Mode_t measurementMode;
        MLX90393_Drive_Mode_t driveMode;
};

#endif
