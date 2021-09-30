#include "TPIS1385.h"

TPIS1385 thermopile(1);
TPsample_t temperatures;

float objectEmissivity = 0.9; // Emissivity value of the individual object being measured

void setup(){
    Serial.begin(9600);
    thermopile.begin();
    thermopile.readEEprom(); // Prints eeprom and updates calibration constants
}

void loop(){
    temperatures = thermopile.getCorrectedSample(objectEmissivity); 
    Serial.print("Object temperature (F): "); Serial.println(CtoF(temperatures.object));
    Serial.print("Ambient temperature (F): "); Serial.println(CtoF(temperatures.ambient));
    delay(1000);
}

float CtoF(float c){
  return c*(9.0f/5.0f)+32; // Converts Celsius to Fahrenheit
}
