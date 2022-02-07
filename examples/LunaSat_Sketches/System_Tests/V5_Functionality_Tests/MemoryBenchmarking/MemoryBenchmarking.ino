#include <Arduino.h>
#include "GLEE_Sensor.h"
#include "TMP117.h"

int sizeOfBuffer = 20;
char buff[20] = {'\0'};

char tempBuffer[5] = {' '};

double temperatureC;
String bufString;

int numOfSamples = 4;

float startTime;    // Variables for timing offset calculations
float endTime;

int timeBetweenSample = 3000; // Time between samples in ms 
int counter = 0;


// LunaSat object initialization is used for declaration of parameters such as ID and debugging mode
TMP117 thermometer(1,false);

void setup(){
    Serial.begin(9600);


    memset(buff, 0, 12); // sets the first 12 bytes of buff to 0


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
  
    /*
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
  //if(counter < numOfSamples){
    //Serial.println(counter);
    startTime = millis();
    temperatureC = thermometer.getTemperatureC();
    dtostrf(temperatureC, 4, 2, tempBuffer); // converts temp to 2 digits after decimal and stores in tempBuffer
    strcat(buff, tempBuffer); // appends tempBuffer to buff
    delay(10);
    strcat(buff,","); // appends comma to buff
    delay(10);
    bufString = "";
    for(int i = 0; i < sizeOfBuffer; i++){
      bufString = bufString + buff[i];
      Serial.println("buffString" + bufString);
    }
    Serial.println("Buffer: " + bufString);
    tempBuffer[5] = {' '};
    endTime = millis();
    delay(timeBetweenSample - (endTime - startTime));
    //counter++;
  //}
    /*
    startTime = millis();
    temperatureC = thermometer.getTemperatureC();
    temperature_string = String(temperatureC);
    Serial.println("Temperature: " + temperature_string);
    endTime = millis();
    delay(timeBetweenSample - (endTime - startTime));
    */

}
