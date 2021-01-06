#include <Arduino.h>
#include <LunaSat.h>

LunaSat lunaSat(0,false);

void setup() {}; // Empty Setup, pinMode initialization occures upon lunaSat object construction

void loop() {
  lunaSat.blink(1,500); // LunaSat.blink(int [LED #], int [on and off delay] )
};
