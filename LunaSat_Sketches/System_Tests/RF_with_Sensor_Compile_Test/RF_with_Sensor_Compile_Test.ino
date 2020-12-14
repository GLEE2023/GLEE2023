#include <Arduino.h>
#include <LunaSat.h>
#include <GLEE_Radio.h>

LunaRadio Rad;
LunaSat lunaSat(0, true);
lunaSat_sample_t sample;

void setup() {
  lunaSat.begin(9600);
  Rad.initialize_radio();
}

void loop() {
  sample = lunaSat.getSample();
  lunaSat.dispSample(sample);

  Rad.transmit_data("sample go's here");
  delay(2000);
}
