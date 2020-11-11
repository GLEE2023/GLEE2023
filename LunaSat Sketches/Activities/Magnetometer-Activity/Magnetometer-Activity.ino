#include <stdio.h>
#include <LunaSat.h>

// Define variable to store current time
unsigned long currentTime;

// Define lunaSat instance

float readings[4];
LunaSat lunaSat;
void setup() {
  //begin serial communication w/ LunaSat; baud=9600
  Serial.begin(9600);      
  LunaSat lunaSat();
  //This calls the constructor which inilializes the set of sensors and RF

  // Wait for lunasat to acknowledge and begin communiction
  //while (lunaSat.begin()!=true){            // While lunasat communications have not begun
  //  delay(250);                             // Wait a bit
  //  Serial.println("Waiting to Begin");     // Indicate waiting
  //}
  //Serial.println("Begin");                  // Announce begining communications
}

void loop() {
  currentTime=millis();
  
  lunaSat.getData();
  lunaSat.printData();

  //TODO: Add LED change or other conditional behavior based on magnetomoter reading
  delay(500);
}
