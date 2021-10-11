#include <CAP.h> // Include Capacitive sensor library dependency

CAP cap(2,1); //Initialize Capactitive Object (Id, Debug)

// Enter calibration coefficient here. Fit Function: Dielectric const = c1*ADC + c2
double c1 = 30;
double c2 = 10;

double dielectricConst; // Variable used to store sampled dielectricConstants

void setup() {
  Serial.begin(9600); // Begin serial communication with 9600 baud rate

  cap.begin(); // Begin Capacitive Sensor (initialization method)
  
  cap.setCal(c1,c2); // Set calibration using user defined calibration coefficients 
}

void loop() {
  dielectricConst = cap.getDielectric(); // Sample dielectric constant (using set calibration coefficents)

  Serial.print("Dielectric Const: "); Serial.println(dielectricConst);

  delay(1000); // Wait 1s between samples
}