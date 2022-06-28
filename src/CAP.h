/**
* Description: This is an Arduino (C++) .h file required for the Capacitive Sensor
* Project Info: Created For GLEE (The Great Lunar Expedition for Everyone)
* Primary Library Author: Andres Montealegre
* Refrences:
* Arduino Documentation
*
**/

#ifndef CAP_H
#define CAP_H
#include "GLEE_Sensor.h"

class CAP:public Sensor{
	public:
		CAP(int _id, bool _debug = false);
	
		void begin();
	
		uint8_t getAddress(); //Returns sensor address
		int getRawData(); //Returns raw data from pin
		double getDieletricConstant(double analogReadIn); //Calculates Dielectric Constant using ADC value and the calculated linear regression
		void addCalibrationPoint(double x, double y); //Stores calibration points to local array if it is not full (Max 15 Calibration Points)
		double *  getLinearRegression(); //Calculates and returns the linear regression for ADC values vs Dielectric constants
		void materialIdent(double DielectricConst); //UNFINISHED: Identifies Material based of Dielectric Constants
		void setCal(double c1, double c2); //Sets m and b points locally
		double getDielectric(); //Returns Dielectric Constant
		
	private:
		int pin;
		int index=0;
		
		//Stores Calibration Points (Max 15)
		double calibrationPoints[15][2];
	
		//Stores linear regression Coefficients
		double m;
		double b;
};
#endif
