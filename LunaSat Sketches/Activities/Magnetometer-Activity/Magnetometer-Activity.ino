#include <ICM20602.h>
#include <tmp117.h>

#include <stdio.h>

// Define variable to store current time
unsigned long currentTime;

// Define lunaSat instance

TMP117 tempSensor;
ICM20602 accler;
int temp;
void setup() {
  Serial.begin(9600);      
  TMP117 tempSensor();
  ICM20602 accler();

  temp = tempSensor.getAddress();
}

void loop() {
  
  delay(500);
}
