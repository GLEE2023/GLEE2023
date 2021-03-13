#include <GLEE_Radio.h>

LunaRadio Rad;


void setup() {
  Serial.begin(9600);
  Rad.initialize_radio();
}

void loop() {
  Rad.transmit_data("Hello World!");
  delay(1000);  
}
