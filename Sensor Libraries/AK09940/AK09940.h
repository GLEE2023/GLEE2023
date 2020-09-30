/*
*Description: This is an Arduino (C++) Header file required for the AK09940 Magnetometer
*Project Info: Created For GLEE (The Great Lunar Expedition for Everyone)
*Library Author: John Walker Johnson
*Library Created on: July 13th 2020
*Last Modified on: July 14th 2020
*Resources Used in Creation:
*AK09940 Datasheet
*Arduino Wire Library Reference Guide
*/

#include <Arduino.h>

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

#define AK09940_SENSOR_ADDR 0x0C	//AK09940 Sensor ADDRESS

typedef struct ak09940RawData_s {		//structure to hold sensor temp, and magnetic x,y,z output 
	float magX;							//x magnetic output
	float magY;							//x magnetic output
	float magZ;							//z magnetic output
	float temp;					//temperature output
};

typedef enum ak09940_Measurement_Modes_t{									//Sensor Modes From DataSheet
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

 typedef enum ak09940_Drive_mode_t {			//
	 LOW_NOISE_1=0b00,
	 LOW_NOISE_2=0b01,
	 LOW_POWER_1=0b10,
	 LOW_POWER_2=0b11,
 };

class AK09940: public Sensor{
	public:
		
		AK09940(void);

		void getSensorData(void);

		void setMeasurementMode(ak09940_Measurement_Modes_t);
		ak09940_Measurement_Modes_t getMode(void);

		void setWatermarkMode(uint8_t steps);
		uint8_t getWatermarkMode(void);

		void setDriveMode(void)
		ak09940_Drive_mode_t getDriveMode(void);

	private:
		ak09940RawData_s rawData;
		int32_t interpret18BitAs32Bit(int32_t input)
		float getTemp(void);
		float getRawMag(uint8_t lowRegister, uint8_t midRegister, uint8_t highRegister);
		


}

/*class AK09940 {
	
	//This the AK09940 Class containing necessary functions for magnetic field measurements

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










void setMeasurementMode(SINGLE_MEASURE)
void setDriveMode(LOW_NOISE_1)



writeByte(32h,SINGLE_MEASURE+LOW_NOISE_1) == writeByte(0x32h,0010010)