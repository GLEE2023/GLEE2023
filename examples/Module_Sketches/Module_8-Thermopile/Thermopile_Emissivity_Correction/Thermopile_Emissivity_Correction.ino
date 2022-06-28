#include "TPIS1385.h"

TPIS1385 thermopile(1);
TPsample_t temperatures; // Saves temperature samples

float objectEmissivity = 0.9; // Emissivity value of the individual object being measured

void setup(){
    Serial.begin(9600); // Sets baud rate to 9600 for serial transmission and starts serial communication
    thermopile.begin(); // Thermopile start-up
    thermopile.readEEprom(); // Prints eeprom and updates calibration constants
}

void loop(){
    temperatures = thermopile.getCorrectedSample(objectEmissivity); // Gets corrected temp samples
    Serial.print("Object temperature (F): "); Serial.println(CtoF(temperatures.object)); // Print object temp
    Serial.print("Ambient temperature (F): "); Serial.println(CtoF(temperatures.ambient)); // Print ambient temp
    delay(1000);
}

float CtoF(float c){
  return c*(9.0f/5.0f)+32; // Converts Celsius to Fahrenheit
}
