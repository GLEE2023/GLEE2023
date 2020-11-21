#include "GLEE_Radio.h"

LunaRadio Rad;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Rad.initialize_radio(915.0,17,250.0,12,8);
}

void loop() {
  // put your main code here, to run repeatedly:
  String output = Rad.receive_data_string();
  Serial.println(output);
}
