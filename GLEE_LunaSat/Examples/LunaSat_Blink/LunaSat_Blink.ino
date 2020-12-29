#include <Arduino.h>
#include <LunaSat.h>

LunaSat lunaSat(0,false);

void setup() {
}

void loop() {
  lunaSat.blink(1,500);
}
