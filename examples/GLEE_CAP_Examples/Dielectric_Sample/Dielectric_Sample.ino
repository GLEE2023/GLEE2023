#include <Arduino.h>
#include "GLEE_Sensor.h"
#include <CAP.h>

CAP cap(2,1); //Initialize Capactitive Object (Id, Debug)

// Enter calibration coeficients here. Fit Function: Dielectric const = c1*ADC + c2
double c1 = 30;
double c2 = 10;

double dielectricConst;

void setup() {
  Serial.begin(9600); //Set BPM
  cap.begin(); //Begin Capacitive Sensor

  cap.setCal(c1,c2);
}

void loop() {
  dielectricConst = cap.getDielectric();
  Serial.print("Dielectric Const: "); Serial.println(dielectricConst);
  delay(1000); // Wait 1s between samples (TODO: Periodic readings or readings triggered on serial message?)
}
