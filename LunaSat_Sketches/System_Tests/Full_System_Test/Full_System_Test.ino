#include <Arduino.h>
#include <LunaSat.h>

LunaSat lunaSat(0, true);


lunaSat_sample_t sample;

void setup() {
  lunaSat.begin(9600);
}

void loop() {
  sample = lunaSat.getSample();
  lunaSat.dispSample(sample);
  delay(2000);
}
