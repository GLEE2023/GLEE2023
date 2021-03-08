#include <Arduino.h>
#include <Wire.h>
#include "Capacitive.h"

Capacitive::Capacitive(int _id, bool _debug){
  Capacitive::info.id = _id;
  Capacitive::info.name = "Capacitive Sensor";
  Capacitive::info.address = 0x01;
  Capacitive::sensorDebug = _debug;
  Wire.begin(Capacitive::info.address);
}
uint8_t Capacitive::getAddress(){
  return Capacitive::info.address;
}
int Capacitive::getRawData(int pin){
  analogRead(pin);
}
// double Capacitive::getDieletricConstant(int analogReadIn){
// In Progress
// }
// void Capacitive::addCalibrationPoint(int correctDielectricValue){
// In Progress
// }
