#include <Arduino.h>
#include "GLEE_Sensor.h"
#include "TMP117.h"

int sizeOfBuffer = 100;
char buffer[sizeOfBuffer];
char *p;

char tmpBuffer[10];

float temperatureC;
String temperature_string;
String bufString;

int numOfSamples = sizeOfBuffer % 10;

float startTime;    // Variables for timing offset calculations
float endTime;

int timeBetweenSample; // Time between samples in ms 

// LunaSat object initialization is used for declaration of parameters such as ID and debugging mode
TMP117 thermometer(1,false);

void setup(){
    Serial.begin(9600);
    p = &RSP[0];
    int counter = 0;
    while(counter < numOfSamples){
        counter++;
        startTime = millis();
        temperatureC = thermometer.getTemperatureC();
        temperature_string = String(temperatureC);
        full_message.toCharArray(buffer,10);
        endTime = millis();
        delay(timeBetweenSample - (endTime - startTime));
    }
    Serial.println("Checking buffer...");
    bufString = "";
    for(int i = 0; i < sizeOfBuffer; i++){
        bufString = bufString + buffer[i];
    }
    Serial.println("Buffer: " + bufString);
    Serial.println("Checking performance...");
}

void loop(){
    startTime = millis();
    temperatureC = thermometer.getTemperatureC();
    temperature_string = String(temperatureC);
    Serial.println("Temperature: " + temperature_string);
    endTime = millis();
    delay(timeBetweenSample - (endTime - startTime));
}