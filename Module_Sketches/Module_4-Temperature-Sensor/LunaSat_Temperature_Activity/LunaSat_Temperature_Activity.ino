// Include dependencies (libraries)
#include <Arduino.h>
#include "GLEE_Sensor.h"
#include "TMP117.h"

// Instantiate TMP117 class with id of one and debugging set to false
TMP117 thermometer(1,false);

// Observation configuration
int timeBetweenSample; // Time between samples in ms 

// Dynamic Variable 
float temperatureC;  // Variable to store an observed temperature in Celsius
float temperatureF;  // Variable to store an observed temperature in Fahrenheit
float startTime;    // Variables for timing offset calculations
float endTime;

void setup(){
    // Begin Serial Communications (Lunasat baud rate set to 9600)
    Serial.begin(9600);

    // Set time between samples to 1000ms (1s)
    timeBetweenSample = 500;
};

void loop(){
    // Record start time for letter offseting of delay to make the loop run once per second
    startTime = millis();

    // Call the getTemperatureC() function of the TMP117 thermometer  
    temperatureC = thermometer.getTemperatureC();
    // Call the getTemperatureF() function of the TMP117 thermometer 
    temperatureF = thermometer.getTemperatureF();
    // Output the temperature via seiral
    Serial.println(temperatureC);
    Serial.println(temperatureF);
    // Record the end time so we know how long it took to record the measurement
    endTime = millis();

    // Delay each loop by the sample rate off set by the time it took to get the temperature
    delay(timeBetweenSample - (endTime - startTime));
};
