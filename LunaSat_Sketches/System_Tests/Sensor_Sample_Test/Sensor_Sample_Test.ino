#include <Arduino.h>
#include <LunaSat.h>

// Initialize Lunasat with an id of 0 and debugging set to true
// Initialize variable to store a lunasat sample

LunaSat lunaSat(0, false); 
lunaSat_sample_t sample;

void setup() {
  lunaSat.begin(9600);
}

void loop() {
  sample = lunaSat.getSample();
  lunaSat.dispSample(sample);
  delay(2000);
}
