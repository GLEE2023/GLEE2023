// Include necessary Librarys
// TODO: Make sure librarys arnt included twice unncecessarily (use ifdef as check)
#include <Arduino.h>
#include <Wire.h>

// Define variable to store current time
unsigned long currentTime;

// Define lunaSat instance
LunaSat lunaSat; 

void setup() {
  //begin serial communication w/ LunaSat; baud=9600
  //TODO: Note what baud rate is determined by
  Serial.begin(9600);      

  //This calls the constructor which inilializes the set of sensors and RF
  lunaSat = new LunaSat() 

  // Wait for lunasat to acknowledge and begin communiction
  //TODO: LunaSat class should have has its own begin function which begins transmission with other sensors)
  while (lunaSat.begin()!=true){
    delay(250);
    Serial.println("Waiting to Begin");
  }
  Serial.println("Begin");
}

void loop() {
  currentTime=millis();
  lunaSat.getData(currentTime);
  lunaSat.printData();
  delay(500);
}
