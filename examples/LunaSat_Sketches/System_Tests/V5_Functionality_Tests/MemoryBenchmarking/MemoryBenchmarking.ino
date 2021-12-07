#include <Arduino.h>
#include "GLEE_Sensor.h"
#include "TMP117.h"


int sizeOfBuffer = 100;
char buff[100];

char tempBuffer[5] = {' '};

float temperatureC;
String bufString;

int numOfSamples = 10;

float startTime;    // Variables for timing offset calculations
float endTime;

int timeBetweenSample = 1000; // Time between samples in ms 

// LunaSat object initialization is used for declaration of parameters such as ID and debugging mode
TMP117 thermometer(1,false);

void setup(){
    Serial.begin(9600);
    /*
    temperatureC = thermometer.getTemperatureC();
    snprintf(tempBuffer,5,"%d",int(temperatureC));
    strcpy(buff,"Test");
    strcat(buff,",");
    strcat(buff,tempBuffer);
    for(int i = 0; i < 100; i++){
      Serial.print(buff[i]);
    }
    */
  
    int counter = 0;
    while(counter < numOfSamples){
        
        counter++;
        startTime = millis();
        temperatureC = thermometer.getTemperatureC();
        sprintf(tempBuffer, "%d", int(temperatureC));
        strcat(buff, tempBuffer);
        delay(10);
        strcat(buff,",");
        delay(10);
        bufString = "";
        for(int i = 0; i < sizeOfBuffer; i++){
            bufString = bufString + buff[i];
        }
        Serial.println("Buffer: " + bufString);
        endTime = millis();
        delay(timeBetweenSample - (endTime - startTime));
    }
    Serial.println("Checking buffer...");
    bufString = "";
    for(int i = 0; i < sizeOfBuffer; i++){
        bufString = bufString + buff[i];
    }
    Serial.println("Buffer: " + bufString);
    Serial.println("Checking performance...");
    
}

void loop(){
    /*
    startTime = millis();
    temperatureC = thermometer.getTemperatureC();
    temperature_string = String(temperatureC);
    Serial.println("Temperature: " + temperature_string);
    endTime = millis();
    delay(timeBetweenSample - (endTime - startTime));
    */
}