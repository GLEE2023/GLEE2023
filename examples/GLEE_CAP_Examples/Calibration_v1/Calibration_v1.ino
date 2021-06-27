#include <Arduino.h>
#include "GLEE_Sensor.h"
#include <CAP.h>

CAP cap(2,1); //Initialize Capactitive Object (Id, Debug)

double *coefs;

void setup() {
  Serial.begin(9600); //Set BPM
  cap.begin(); //Begin Capacitive Sensor
  
  // CHANGE THESE POINTS TO YOUR CALIBRATION POINTS (ADC, DIELECTRIC CONST)
  cap.addCalibrationPoint(17,94); // Add Calibration Point 1
  cap.addCalibrationPoint(13,73); // Add Calibration Point 2
  cap.addCalibrationPoint(12,59); // Add Calibration Point 3
  cap.addCalibrationPoint(15,80); // Add Calibration Point 4
  cap.addCalibrationPoint(16,93); // Add Calibration Point 5
  
  coefs = cap.getLinearRegression(); // Create Linear Regression Using Calibration Points
  
  Serial.println(""); 
  Serial.print("Dielectric Constant: ");
  Serial.print(cap.getDieletricConstant(4.2)); // Test Regression and Find Dielectric Constant by plugging in an ADC Value

  Serial.print("First Coef: "); Serial.println(*coefs);
  Serial.print("Second Coef: "); Serial.println(*(coefs + 1));
}

void loop() {
  // put your main code here, to run repeatedly:
}