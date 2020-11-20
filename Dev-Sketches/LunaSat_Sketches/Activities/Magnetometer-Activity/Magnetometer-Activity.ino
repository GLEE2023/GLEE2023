#include <stdio.h>
#include <LunaSat.h>



// Define variable to store current time
unsigned long currentTime;

// Define lunaSat instance

LunaSat lunaSat;

int temp;
void setup() {
  Serial.begin(9600);      
  LunaSat lunaSat();
}

void loop() {
  
  delay(500);
}
