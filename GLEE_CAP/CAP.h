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
		uint8_t getAddress();
		int getRawData();
		double getDieletricConstant(double analogReadIn);
		void addCalibrationPoint(int correctDielectricValue);
		void begin();
		void getLinearRegression(double v[10][2]);

	private:
		int pin;


};
#endif
