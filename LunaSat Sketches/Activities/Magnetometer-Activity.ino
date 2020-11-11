
#include <Arduino.h>
#include <Wire.h>
#include <LunaSat.h>

// Define variable to store current time
unsigned long currentTime;

// Define lunaSat instance
LunaSat lunaSat; 
float readings[4];

void setup() {
  //begin serial communication w/ LunaSat; baud=9600
  Serial.begin(9600);      

  //This calls the constructor which inilializes the set of sensors and RF
  lunaSat = new LunaSat(); //Args: (List of Sensors,modes) 

  // Wait for lunasat to acknowledge and begin communiction
  while (lunaSat.begin()!=true){            // While lunasat communications have not begun
    delay(250);                             // Wait a bit
    Serial.println("Waiting to Begin");     // Indicate waiting
  }
  Serial.println("Begin");                  // Announce begining communications
}

void loop() {
  currentTime=millis();
  readings = [currentTime, lunaSat.ak09940.getMagX(), lunaSat.ak09940.getMagY(), lunaSat.ak09940.getMagZ()];
  
  for(int i = 0; i < 4; i++){
    Serial.print(readings[i],7);
    Serial.print(",");
  }
  Serial.println(readings[3],7);

  //TODO: Add LED change or other conditional behavior based on magnetomoter reading
  delay(500);
}