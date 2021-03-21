#include "Arduino.h"
#include "TPIS1385.h"

TPIS1385 thermopile(1,false);
float Tamb, Tobj = 0;
uint16_t TPamb = 0;
uint32_t TPobj = 0;
void setup(){
    Serial.begin(9600);
    Serial.println("test");
    thermopile.begin();
    thermopile.readEEprom();
}

void loop(){
    TPamb = thermopile.getTPamb();
    Tamb = thermopile.getTamb(TPamb);

    TPobj = thermopile.getTPobj();
    Tobj = thermopile.getTobj(TPobj, Tamb);
    Serial.println(CToF(Tamb - 273.15f));
    Serial.println(CToF(Tobj - 273.15f));
    delay(1000);
}

float CToF(float c){
  return c*(9.0f/5.0f)+32;
}
