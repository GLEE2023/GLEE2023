#include "TPIS1385.h"

TPIS1385 thermopile(1);
TPsample_t temperatures;

float object_emisivity = 0.9;

void setup(){
    Serial.begin(9600);
    thermopile.begin();
    thermopile.readEEprom(); // Prints eeprom and updates calibration constants
}

void loop(){
    temperatures = thermopile.getCorrectedSample(object_emisivity);
    Serial.print("Object temperature (F): "); Serial.println(CtoF(temperatures.object));
    Serial.print("Ambient temperature (F): "); Serial.println(CtoF(temperatures.ambient));
    delay(1000);
}

float CtoF(float c){
  return c*(9.0f/5.0f)+32;
}
