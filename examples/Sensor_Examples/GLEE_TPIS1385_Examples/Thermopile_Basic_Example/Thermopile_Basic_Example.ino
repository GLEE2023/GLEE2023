#include "TPIS1385_LunarVersion.h"

TPIS1385_New thermopile(1);
TPsample_t temperatures;

void setup(){
    Serial.begin(9600); // Sets baud rate to 9600 bits per second and starts serial communications
    thermopile.begin(); // Thermopile start-up
    thermopile.readEEprom(); // Prints eeprom and updates calibration constants
}

void loop(){
    temperatures = thermopile.getSample(); // Gets a temp sample
    Serial.print("Object temperature (F): "); Serial.println(CtoF(temperatures.object));
    Serial.print("Ambient temperature (F): "); Serial.println(CtoF(temperatures.ambient));
    delay(1000);
}

float CtoF(float c){ // Function to convert from Celsius to Farenheit
  return c*(9.0f/5.0f)+32;
}
