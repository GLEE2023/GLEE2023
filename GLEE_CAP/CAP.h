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
		uint8_t getAddress();
		int getRawData();
		double getDieletricConstant(double analogReadIn);
		void addCalibrationPoint(double x, double y);
		void getLinearRegression();
		void materialIdent(double DielectricConst);
	private:
		int pin;
		int index=0;
		double calibrationPoints[15][2];
		double m;
		double b;
};
#endif
