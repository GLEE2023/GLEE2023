#include <Arduino.h>
#include <Wire.h>
#include "Capacitive.h"

/**
 * Constructor
 * Parameters: the sensor ID as an integer and the debug mode as a booelan
 * Returns: none
 * This is the constructor for the Capacitive class. 
**/
Capacitive::Capacitive(int _id, bool _debug){
  Capacitive::info.id = _id;
  Capacitive::info.name = "Capacitive Sensor";
  Capacitive::info.address = 0x01;
  Capacitive::sensorDebug = _debug;
  Wire.begin(Capacitive::info.address);
}

/**
 * Parameters: none
 * Returns: uint8_t add
 * This functions gets the address for the sensor. 
**/
uint8_t Capacitive::getAddress(){
  return Capacitive::info.address;
}

/**
 * Parameters: int pin
 * Returns: none
 * This function gets the raw data for the capacitive.
**/
int Capacitive::getRawData(int pin){
  analogRead(pin);
}


// double Capacitive::getDieletricConstant(int analogReadIn){
// In Progress
// }
// void Capacitive::addCalibrationPoint(int correctDielectricValue){
// In Progress
// }
