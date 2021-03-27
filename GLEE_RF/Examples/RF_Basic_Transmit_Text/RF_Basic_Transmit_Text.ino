#include <GLEE_Radio.h>

LunaRadio Rad;


void setup() {
  Serial.begin(9600);
  Rad.initialize_radio(915.0,17,250.0,12,8);
}

void loop() {
  Rad.transmit_data("Hello World!");
  delay(1000);  
}
