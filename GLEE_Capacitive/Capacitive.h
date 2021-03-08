/*
*Description: This is an Arduino (C++) .h file required for the Capacitive Sensor
*Project Info: Created For GLEE (The Great Lunar Expedition for Everyone)
*Library Author: Andres Montealegre
*Library Created on: November 23rd, 2020
*Last Modified on: November 23rd, 2020
*Resources Used in Creation:
*Capacitive Datasheet
*Arduino Wire Library Reference Guide
*/


#ifndef Capacitive_H
#define Capacitive_H

#include <Arduino.h>
#include "GLEE_Sensor.h"

class Capacitive:public Sensor{
  public:
      Capacitive(int _id, bool _debug = false);
      uint8_t getAddress();
      int getRawData(int pin);
      double getDieletricConstant(int analogReadIn);
      void addCalibrationPoint(int correctDielectricValue);
  };
#endif
