/*
* Description: This is an Arduino (C++) .cpp file required for the VT03 RADFET
* Project Info: Created For GLEE (The Great Lunar Expedition for Everyone)
* Library Authors: Yvonne Liu, Lawson Nerenberg,
* Resources Used in Creation:
* VT03 Datasheet
*
*/

#include "VT03.h"

/*
 * Constructor
 * Parameters: the sensor ID as an integer and the debug mode as a booelan
 * Returns: none
 * This is the constructor for the Capacitive class.
*/
VT03::VT03(int _id, bool _debug){
    VT03::info.id = _id;
    VT03::info.name = "RADFET Sensor";
    VT03::info.address = 0x01
    VT03::sensorDebug = _debug;
    VT03::pin = A0; // Hardcoded analog pin on LunaSat???
}

/*
Parameters: none
Returns: none
This function begins communication with the sensor.
 */
void VT03::begin(){
	pinMode(VT03::pin,INPUT);
}

/**
 * Parameters: none
 * Returns: uint8_t for the address
 * This functions gets the address for the sensor.
**/
uint8_t VT03::getAddress(){
	return VT03::info.address;
}

/**
 * Parameters: int pin
 * Returns: none
 * This function gets the raw data for the RADFET.
**/
int CAP::getRawData(){
	return analogRead(VT03::pin);
}