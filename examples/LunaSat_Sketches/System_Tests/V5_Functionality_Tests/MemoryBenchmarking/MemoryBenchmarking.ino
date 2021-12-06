#include <Arduino.h>
#include "GLEE_Sensor.h"
#include "TMP117.h"


int sizeOfBuffer = 100;
char buff[100];

char tempBuffer[50] = {' '};

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
    temperatureC = 1.0;
    snprintf(tempBuffer,5,"%f",temperatureC);
    Serial.print((tempBuffer[0]));
    strcpy(buff,"Test");
    strcat(buff,",");
    strcat(buff,tempBuffer);
    for(int i = 0; i < 100; i++){
      Serial.print(buff[i]);
    }

    
    /*
    int counter = 0;
    while(counter < numOfSamples){
        
        counter++;
        startTime = millis();
        temperatureC = thermometer.getTemperatureC();
        sprintf(tempBuffer, "%f", temperatureC);
        strcpy(buff, tempBuffer);
        strcat(buff,",");
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
    */
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