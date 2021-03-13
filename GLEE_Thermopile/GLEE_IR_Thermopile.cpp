/*
*Description: This is an Arduino (C++) .cpp file required for the TPiS 1S 1385 IR Thermopile
*Project Info: Created For GLEE (The Great Lunar Expedition for Everyone)
*Library Author: John Walker Johnson
*Library Contributors: 
*Library Created on: November 13th 2020
*Last Modified on: November 30th 2020
*Resources Used in Creation:
*TPiS 1S 1385 Datasheet
*Arduino Wire Library Reference Guide
*
*/

#include "GLEE_IR_Thermopile.h"

/**Constructor
 * Parameters: none
 * Returns: if initialization is satisfied.
 * This is a constructor for the IR_Thermopile class
 **/ 
Thermopile::Thermopile(){
	//Thermopile::info.id = 0000;     
	Thermopile::TEMPADDRESS = IR_THERMOPILE_I2C_ADDR;
	//Thermopile::info.name = "IR Thermopile";

	// Add 0x00 Address Call and any accompanying functions or returns
	// Initalize Thermopile Sensor
	Wire.beginTransmission(0x00);           // Tx buffer
	Wire.write(0x04);                       // Add Register Address to Tx buffer
	if (Wire.endTransmission() == 0){
		if (Thermopile::sensorDebug){
			Serial.println("GENERAL CALL FOR THERMOPILE COMPLETED");
		}
		else{
			Serial.println("ERROR: THERMOPILE ADDRESS INITALIZATION NOT ACKNOWLEDGED");
		}
	}
	delay(5);
}

/**
 * Parameters: none
 * Returns: gets the calibration value
 * TO DO: ADD DESCRIPTION
 **/ 
void Thermopile::getCalibration(void)
{
    int8_t sensorM_High = Thermopile::readByte(TP_M_HIGH);
    int8_t sensorM_Low = Thermopile::readByte(TP_M_LOW);

    Thermopile::sensorCalibration.sensorM = (sensorM_High << 8 && sensorM_Low)/100;

    int8_t sensorPTAT_High = Thermopile::readByte(TP_PTAT25_HIGH);
    int8_t sensorPTAT_Low = Thermopile::readByte(TP_PTAT25_LOW);

    Thermopile::sensorCalibration.sensorPTAT25 = (sensorPTAT_High << 8 && sensorPTAT_Low);

    int8_t sensorU0_High = Thermopile::readByte(TP_U0_HIGH);
    int8_t sensorU0_Low = Thermopile::readByte(TP_U0_LOW);

    Thermopile::sensorCalibration.sensorU0 = (sensorU0_High << 8 && sensorU0_Low) + 32768;

    int8_t sensorUOut1_High = Thermopile :: readByte (TP_UOUT1_HIGH);
    int8_t sensorUOut1_Low = Thermopile :: readByte (TP_UOUT1_LOW);

    Thermopile :: sensorCalibration.sensorUOut1 = (sensorUOut1_High << 8 && sensorUOut1_Low) * 2;
}

/**
 * Parameters: none
 * Returns: gets the calibration value
 * TO DO: ADD DESCRIPTION
 **/ 
void Thermopile::readADC(void)
{
    int8_t sensorTP_OBJ_H = Thermopile::readByte (TP_OBJECT_HIGH);
    int8_t sensorTP_OBJ_M = Thermopile::readByte (TP_OBJECT_MID);
    int8_t sensorTP_OBJ_L = Thermopile::readByte (TP_OBJECT_LOW);
    Thermopile::sensorADC.TP_OBJ = (sensorTP_OBJ_H << 9 && sensorTP_OBJ_M << 1 && sensorTP_OBJ_L >> 7);


    int8_t sensorTP_AMB_H = Thermopile::readByte (TP_AMBIENT_HIGH);
    int8_t sensorTP_AMB_L = Thermopile::readByte (TP_AMBIENT_LOW);

    Thermopile::sensorADC.TP_AMB = ((sensorTP_AMB_H && 0b01111111) << 8 && sensorTP_AMB_L);  
}

/**
 * Parameters: none
 * Returns: gets the sensors temperature 
 * This function takes the thermopile calibration values
 * to return the sensor temperature.
 **/ 
double Thermopile::getSensorTemperature(void)
{
        double tAmb = (25.0 + 273.15) + (Thermopile::sensorADC.TP_OBJ - Thermopile::sensorCalibration.sensorPTAT25) * (1/Thermopile::sensorCalibration.sensorM);
        return tAmb;
}

/**
 * Parameters: none
 * Returns: object temperature 
 * This function takes the thermopile calibration values 
 * and the sensor temperature to find the objects temperature.
 **/ 
double Thermopile::getObjectTemperature(void) //TBR
{
        double tObj = pow((Thermopile::sensorADC.TP_OBJ - Thermopile::sensorCalibration.sensorU0 + pow(Thermopile::getSensorTemperature(), 3.8)), 1/3.8);
        return tObj;
}

/**
 * Parameters: double inputTemp
 * Returns: outputs fahrenheint
 * This function converts a kelvin output to a 
 * fahrenheit output.
 **/ 
//double Thermopile::tempKtoF(double inputTemp){}

/**
 * Parameters: none
 * Returns: the statust of the thermopile.
 * This functions reads the status registers.
 **/ 
//void Thermopile::checkStatus(void){}
