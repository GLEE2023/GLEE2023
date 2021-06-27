
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

double * CAP::getLinearRegression(){
	int size = CAP::index;
	double xAvg = 0;
	double yAvg = 0;
	double stdProductSum = 0;
	double xStdSquared = 0;
	double yStdSquared = 0;
	double r = 0;
	double b =0;
	double yIntercept=0;

	static double coefs[2]; 
	
	// X defines ADC Values, Y defines Dielectric constants

	for(int i =0; i < size;i++){
		xAvg = xAvg+ CAP::calibrationPoints[i][0];
		yAvg = yAvg+ CAP::calibrationPoints[i][1];
	}
	
	xAvg= xAvg/size;
	yAvg= yAvg/size;
	
	for(int j =0; j < size;j++){
		stdProductSum=stdProductSum+((CAP::calibrationPoints[j][0]-xAvg)*(CAP::calibrationPoints[j][1]-yAvg));
		xStdSquared = xStdSquared+pow((CAP::calibrationPoints[j][0]-xAvg),2);
		yStdSquared = yStdSquared+pow((CAP::calibrationPoints[j][1]-yAvg),2);	
	}

	r = stdProductSum/sqrt((xStdSquared*yStdSquared));
	b= r*(sqrt((yStdSquared/(size-1))/(xStdSquared/(size-1))));
	yIntercept = yAvg - b*xAvg;

	CAP::m = b;
	CAP::b = yIntercept;
	
	coefs[0] = b;
	coefs[1] = m;

	Serial.println("LINEAR REGRESSION");
	Serial.println("-----------------");
	Serial.print("y = ");
	Serial.print(b);
	Serial.print("x + (");
	Serial.print(yIntercept);
	Serial.print(")");

	return coefs;
}

void CAP::addCalibrationPoint(double x, double y){
	if(CAP::index+1!=15){
		CAP::calibrationPoints[CAP::index][0] = x;
		CAP::calibrationPoints[CAP::index][1] = y;
		CAP::index = CAP::index+1;
	}
	else{
		Serial.println("You have reached the max of 15 calibration points");
	}
}

double CAP::getDieletricConstant(double ADCvalue){
	return (CAP::m * ADCvalue) + CAP::b;
}

double CAP::getDielectric(){
	int rawADC = getRawData();
	double dielectricConst = CAP::m * rawADC + CAP::b;
	return dielectricConst;
}

void CAP::setCal(double c1, double c2){
	CAP::m = c1;
	CAP::b = c2;
}
