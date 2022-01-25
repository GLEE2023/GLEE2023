
#include "CAP.h"

/**
 * Constructor
 * Parameters: the sensor ID as an integer and the debug mode as a booelan
 * Returns: none
 * This is the constructor for the Capacitive class.
**/
CAP::CAP(int _id, bool _debug){
	CAP::info.id = _id;
	CAP::info.name = F("CAP");
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

/**
 * Parameters: None
 * Returns: Linear regression Coefficients
 * This function calculates and returns the linear regression for ADC values vs Dielectric constants
**/
double * CAP::getLinearRegression(){
	//Initialize Variables
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

	//Calculate x and y averages from Calibration Points
	for(int i =0; i < size;i++){
		xAvg = xAvg+ CAP::calibrationPoints[i][0];
		yAvg = yAvg+ CAP::calibrationPoints[i][1];
	}
	
	xAvg= xAvg/size;
	yAvg= yAvg/size;
	
	//Calculate y Intercept (b) and slope (m) 
	for(int j =0; j < size;j++){
		stdProductSum=stdProductSum+((CAP::calibrationPoints[j][0]-xAvg)*(CAP::calibrationPoints[j][1]-yAvg));
		xStdSquared = xStdSquared+pow((CAP::calibrationPoints[j][0]-xAvg),2);
		yStdSquared = yStdSquared+pow((CAP::calibrationPoints[j][1]-yAvg),2);	
	}

	r = stdProductSum/sqrt((xStdSquared*yStdSquared));
	b= r*(sqrt((yStdSquared/(size-1))/(xStdSquared/(size-1))));
	yIntercept = yAvg - b*xAvg;

	//Store coefficients locally
	CAP::m = b;
	CAP::b = yIntercept;
	
	//Store coefficients in array
	coefs[0] = b;
	coefs[1] = m;
	
	//Print Linear Regression
	Serial.println(F("LINEAR REGRESSION"));
	Serial.println(F("-----------------"));
	Serial.print(F("y = "));
	Serial.print(b);
	Serial.print(F("x + ("));
	Serial.print(yIntercept);
	Serial.print(F(")"));
	
	//Return Linear Regression Coefficients as an array (mx + b)
	return coefs;
}

/**
 * Parameters: X and Y coordinates of calibration point that will be added locally
 * Returns: None
 * This function stores calibration points to local array if it is not full (Max 15 Calibration Points)
**/
void CAP::addCalibrationPoint(double x, double y){
	if(CAP::index+1!=15){
		CAP::calibrationPoints[CAP::index][0] = x;
		CAP::calibrationPoints[CAP::index][1] = y;
		CAP::index = CAP::index+1;
	}
	else{
		Serial.println(F("15 points max"));
	}
}

/**
 * Parameters: ADC Value
 * Returns: Dielectric Constant from given ADC Value
 * This function calculates Dielectric Constant using ADC value and the calculated linear regression
**/
double CAP::getDieletricConstant(double ADCvalue){
	return (CAP::m * ADCvalue) + CAP::b;
}

/**
 * Parameters: None
 * Returns: Dielectric Constant
 * This function calculates Dielectric Constant using raw ADC value and the calculated linear regression
**/
double CAP::getDielectric(){
	int rawADC = getRawData();
	double dielectricConst = CAP::m * rawADC + CAP::b;
	return dielectricConst;
}

/**
 * Parameters: M and b points to be stores
 * Returns: None
 * This function stores m and b for linear regression
**/
void CAP::setCal(double c1, double c2){
	CAP::m = c1;
	CAP::b = c2;
}
