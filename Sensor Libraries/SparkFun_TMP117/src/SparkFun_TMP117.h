/******************************************************************************
SparkFunTMP117.h
SparkFunTMP117 Library Header File
Madison Chodikov @ SparkFun Electronics
Original Creation Date: April 29, 2016
https://github.com/sparkfunX/Qwiic_TMP117

This file prototypes the TMP117 class, implemented in SparkFunTMP117.cpp.

Development environment specifics:
	IDE: Arduino 1.8.9
	Hardware Platform: Arduino Uno
	TMP117 Breakout Version: 1.0.0

This code is beerware; if you see me (or any other SparkFun employee) at the
local, and you've found our code helpful, please buy us a round!

Distributed as-is; no warranty is given.
******************************************************************************/

#ifndef __SparkFun_TMP117_H__
#define __SparkFun_TMP117_H__

#include <Wire.h>
#include <Arduino.h>
#include "SparkFun_TMP117_Registers.h"

#define DEVICE_ID_VALUE 0x0117			// Value found in the device ID register on reset (page 24 Table 3 of datasheet)
#define TMP117_RESOLUTION 0.0078125f	// Resolution of the device, found on (page 1 of datasheet)
#define CONTINUOUS_CONVERSION_MODE 0b00 // Continuous Conversion Mode
#define ONE_SHOT_MODE 0b11				// One Shot Conversion Mode
#define SHUTDOWN_MODE 0b01				// Shutdown Conversion Mode

// Configuration register found on page 25 Figure 26 and Table 6
typedef union {
	struct
	{
		uint8_t EMPTY : 1;			// Empty bit in register
		uint8_t TMP_SOFT_RESET : 1; // Software reset bit
		uint8_t DR_ALERT : 1;		// ALERT pin select bit
		uint8_t POL : 1;			// ALERT pin polarity bit
		uint8_t T_NA : 1;			// Therm/alert mode select
		uint8_t AVG : 2;			// Conversion averaging modes
		uint8_t CONV : 3;			// Conversion cycle bit
		uint8_t MOD : 2;			// Set conversion mode
		uint8_t EEPROM_BUSY : 1;	// EEPROM busy flag
		uint8_t DATA_READY : 1;		// Data ready flag
		uint8_t LOW_ALERT : 1;		// Low Alert flag
		uint8_t HIGH_ALERT : 1;		// High Alert flag
	} CONFIGURATION_FIELDS;
	uint8_t CONFIGURATION_COMBINED;
} CONFIGURATION_REG;

// Device ID Register used for checking if the device ID is the same as declared
// This register is found on Page 30 of the datasheet in Table 15 and Figure 34
typedef union {
	struct
	{
		uint16_t DID : 12; // Indicates the device ID
		uint8_t REV : 4;   // Indicates the revision number
	} DEVICE_ID_FIELDS;
	uint16_t DEVICE_ID_COMBINED;
} DEVICE_ID_REG;

class TMP117
{
public:
	TMP117(); // Constructor

	bool begin(uint8_t sensorAddress = 0x48, TwoWire &wirePort = Wire); // Checks for ACK over I2C, and sets the device ID of the TMP and chooses the wire port
	uint8_t getAddress();												// Returns the address of the device
	double readTempC();													// Returns the temperature in degrees C
	double readTempF();													// Converts readTempC result to degrees F
	void softReset();													// Performs a software reset on the Configuration Register Field bits
	float getTemperatureOffset();										// Reads the temperature offset
	void setTemperatureOffset(float offset);							// Writes to the temperature offset
	float getLowLimit();												// Returns the low limit register
	void setLowLimit(float lowLimit);									// Sets the low limit temperature for the low limit register
	float getHighLimit();												// Returns the high limit register
	void setHighLimit(float highLimit);									// Sets the low limit temperature for the low limit register
	uint16_t getConfigurationRegister();								// Get Configuration Register
	uint8_t getHighLowAlert();											// Reads in Alert mode for high and low alert flags
	bool getHighAlert();												// Reads in Alert mode for a high alert flag
	bool getLowAlert();													// Reads in Alert mode for a low alert flag
	void setAlertFunctionMode(uint8_t setAlertMode);					// Set alert or therm mode
	uint8_t getAlertFunctionMode();										// Check to see if in alert or therm mode
	uint8_t getConversionMode();										// Checks to see the Conversion Mode the device is currently in
	void setContinuousConversionMode();									// Sets the Conversion Mode of the Device to be Continuous
	void setOneShotMode();												// Sets the Conversion Mode of the Device to be One Shot
	void setShutdownMode();												// Sets the Conversion Mode of the Device to be Shutdown
	void setConversionAverageMode(uint8_t convMode);					// Sets the conversion averaging mode
	uint8_t getConversionAverageMode();									// Returns the Conversion Averaging Mode
	void setConversionCycleBit(uint8_t convTime);						// Sets the conversion cycle time bit
	uint8_t getConversionCycleBit();									// Returns the conversion cycle time bit value
	bool dataReady();													// Checks to see if there is data ready from the device

private:
	TwoWire *_i2cPort = NULL; //The generic connection to user's chosen I2C hardware
	uint8_t _deviceAddress;   // Address of Temperature sensor

	uint16_t readRegister(uint8_t reg);				// Reads 2 register bytes from sensor
	void writeRegister(uint8_t reg, uint16_t data); // Wires single byte of data to the sensor
};

#endif