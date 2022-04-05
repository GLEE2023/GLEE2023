#include <Arduino.h>
#include "GLEE_Sensor.h"
#include "TMP117.h"

int sizeOfBuffer = 20; //comfortable max is around 1500
char buff[20] = {'\0'};
char tempBuffer[5] = {' '};

double temperatureC;

int numOfSamples = 0;
int counter = 0;

float startTime;    // Variables for timing offset calculations
float endTime;
int timeBetweenSample = 3000; // Time between samples in ms 

TMP117 thermometer(1, false); // LunaSat object initialization is used for declaration of parameters such as ID and debugging mode


void setup() {
  Serial.begin(9600);
}

void loop() {
  startTime = millis();
  numOfSamples++;
  Serial.println(numOfSamples);
  temperatureC = thermometer.getTemperatureC();
  dtostrf(temperatureC, 4, 2, tempBuffer); // converts temp to 2 digits after decimal and stores in tempBuffer
  strcat(buff, tempBuffer); // appends tempBuffer to buff
  Serial.println(buff);
  strcat(buff,","); // appends comma to buff
  if(buff[sizeOfBuffer - 4] != 0){
    Serial.println("Maximum memory reached.");
    delay(500);
    exit(0);
  }
  tempBuffer[5] = {' '};
  endTime = millis();
  delay(timeBetweenSample - (endTime - startTime));
}
