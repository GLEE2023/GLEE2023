
#include "CAP.h"

/**
 * Constructor
 * Parameters: the sensor ID as an integer and the debug mode as a booelan
 * Returns: none
 * This is the constructor for the Capacitive class. 
**/
CAP::CAP(int _id, bool _debug){
	CAP::info.id = _id;
	CAP::info.name = "Capacitive Sensor";
	CAP::info.address = 0x01;
	CAP::sensorDebug = _debug;
	CAP::pin = A0; // Hardcoded analog pin on LunaSat
}

void CAP::begin(){
	pinMode(CAP::pin,INPUT);
}

/**
 * Parameters: none
 * Returns: uint8_t add
 * This functions gets the address for the sensor. 
**/
uint8_t CAP::getAddress(){
	return CAP::info.address;
}

/**
 * Parameters: int pin
 * Returns: none
 * This function gets the raw data for the capacitive.
**/
int CAP::getRawData(){
	return analogRead(CAP::pin);
}
