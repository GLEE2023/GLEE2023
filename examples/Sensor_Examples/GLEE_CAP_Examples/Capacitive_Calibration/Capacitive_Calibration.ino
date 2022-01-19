#include <CAP.h>

CAP cap(2,1); // Initialize Capactitive Object (Id, Debug)

double *coefs; // Double pointer to store returned calibration coefficents

void setup() {
  Serial.begin(9600); // Begin serial communication at 9600 baud rate
  cap.begin(); // Call the begin method for the Capacitive Sensor (initialization method)
  
  // CHANGE THESE POINTS TO YOUR CALIBRATION POINTS (ADC READING, KNOWN DIELECTRIC CONST)
  cap.addCalibrationPoint(17,94); // Add Calibration Point 1
  cap.addCalibrationPoint(13,73); // Add Calibration Point 2
  cap.addCalibrationPoint(12,59); // Add Calibration Point 3
  
  coefs = cap.getLinearRegression(); // Preform Linear Regression on entered Calibration Points 

  // Print Calibration Coefficents
  Serial.print("First Coefficent: "); Serial.println(*coefs);
  Serial.print("Second Coefficent: "); Serial.println(*(coefs + 1));
}

void loop() {
  // put your main code here, to run repeatedly:
}