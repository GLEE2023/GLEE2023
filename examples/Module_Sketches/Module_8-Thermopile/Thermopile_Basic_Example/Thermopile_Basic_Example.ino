#include "TPIS1385.h"

TPIS1385 thermopile(1);
TPsample_t temperatures; // Saves temperature measurements

void setup(){
    Serial.begin(9600);
    thermopile.begin();
    thermopile.readEEprom(); // Prints eeprom and updates calibration constants
}

void loop(){
    temperatures = thermopile.getSample(); // Gets the temp samples
    Serial.print("Object temperature (F): "); Serial.println(CtoF(temperatures.object)); // Print out object temp
    Serial.print("Ambient temperature (F): "); Serial.println(CtoF(temperatures.ambient)); // Print out ambient temp
    delay(1000);
}

float CtoF(float c){ // Function to convert from celsius to farenheit
  return c*(9.0f/5.0f)+32;
}
