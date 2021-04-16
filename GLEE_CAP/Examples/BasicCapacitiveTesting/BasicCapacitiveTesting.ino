#include <Arduino.h>
#include "GLEE_Sensor.h"
#include <CAP.h>
CAP cap(2,1);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  cap.begin();
  cap.addCalibrationPoint(17,94);
  cap.addCalibrationPoint(13,73);
  cap.addCalibrationPoint(12,59);
  cap.addCalibrationPoint(15,80);
  cap.addCalibrationPoint(16,93);
  cap.getLinearRegression();
  Serial.println("");
  Serial.print("Dielectric Constant: ");
  Serial.print(cap.getDieletricConstant(4.2));
}

void loop() {
  // put your main code here, to run repeatedly:
}
