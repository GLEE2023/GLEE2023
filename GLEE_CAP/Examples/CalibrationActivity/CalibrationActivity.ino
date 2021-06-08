#include <Arduino.h>
#include "GLEE_Sensor.h"
#include <CAP.h>

CAP cap(2,1); //Initialize Capactitive Object (Id, Debug)

void setup() {
  Serial.begin(9600); //Set BPM
  cap.begin(); //Begin Capacitive Sensor
  
  cap.addCalibrationPoint(17,94); //Add Calibration Point 1
  cap.addCalibrationPoint(13,73); //Add Calibration Point 2
  cap.addCalibrationPoint(12,59); //Add Calibration Point 3
  cap.addCalibrationPoint(15,80); //Add Calibration Point 4
  cap.addCalibrationPoint(16,93); //Add Calibration Point 5
  
  cap.getLinearRegression(); //Create Linear Regression Using Calibration Points
  
  Serial.println(""); 
  Serial.print("Dielectric Constant: ");
  Serial.print(cap.getDieletricConstant(4.2)); //Test Regression and Find Dielectric Constant by plugging in an ADC Value
}

void loop() {
  // put your main code here, to run repeatedly:
}
