#ifndef Capacitive_H
#define Capacitive_H

#include <Arduino.h>
#include "GLEE_Sensor.h"

class Capacitive : public Sensor{
  public:
      Capacitive(int _id, bool _debug = false);
      uint8_t getAddress();
      int getRawData(int pin);
      double getDieletricConstant(int analogReadIn);
      void addCalibrationPoint(int correctDielectricValue);
  };
#endif
