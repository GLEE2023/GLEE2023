
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

void CAP::getLinearRegression(double v[10][2]){
    int size = 10;
    double xAvg = 0;
    double yAvg = 0;
    double stdProductSum = 0;
    double xStdSquared = 0;
    double yStdSquared = 0;
    double r = 0;
    double b =0;
    double yIntercept=0;
    for(int i =0; i < size;i++){
        xAvg = xAvg+ v[i][0];
        yAvg = yAvg+ v[i][1];
    }
    xAvg= xAvg/size;
    yAvg= yAvg/size;
    for(int j =0; j < size;j++){
        stdProductSum=stdProductSum+((v[j][0]-xAvg)*(v[j][1]-yAvg));
        xStdSquared = xStdSquared+pow((v[j][0]-xAvg),2);
        yStdSquared = yStdSquared+pow((v[j][1]-yAvg),2);
    }
    r = stdProductSum/sqrt((xStdSquared*yStdSquared));
    b= r*(sqrt((yStdSquared/(size-1))/(xStdSquared/(size-1))));
    yIntercept = yAvg - b*xAvg;
}
