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

void Capacitive::linearRgression(vector <vector<int>> v){
    double xAvg = 0;
    double yAvg = 0;
    double stdProductSum = 0;
    double xStdSquared = 0;
    double yStdSquared = 0;
    double r = 0;
    double b =0;
    double yIntercept=0;
    for(int i =0; i < v.size();i++){
        xAvg = xAvg+ v[i][0];
        yAvg = yAvg+ v[i][1];
    }
    xAvg= xAvg/v.size();
    yAvg= yAvg/v.size();
    for(int j =0; j < v.size();j++){
        stdProductSum=stdProductSum+((v[j][0]-xAvg)*(v[j][1]-yAvg));
        xStdSquared = xStdSquared+pow((v[j][0]-xAvg),2);
        yStdSquared = yStdSquared+pow((v[j][1]-yAvg),2);
    }
    r = stdProductSum/sqrt((xStdSquared*yStdSquared));
    b= r*(sqrt((yStdSquared/(v.size()-1))/(xStdSquared/(v.size()-1))));
    yIntercept = yAvg - b*xAvg;
    cout<<"y = "<<yIntercept<<" + "<< b<<"x"<<endl;
}
