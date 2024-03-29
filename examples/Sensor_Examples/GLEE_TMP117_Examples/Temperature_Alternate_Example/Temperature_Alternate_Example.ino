// Include dependencies (libraries)
#include <Arduino.h>
#include "GLEE_Sensor.h"
#include "TMP117.h"

// Instantiate TMP117 class with id of one and debugging set to false
TMP117 thermometer(1,false);

// Observation configuration
int timeBetweenSample; // Time between samples in ms 

// Dynamic Variables
float temperatureC;  // Variable to store an observed temperature in Celsius
float temperatureF;  // Variable to store an observed temperature in Fahrenheit
float startTime;    // Variables for timing offset calculations
float endTime;

void setup(){
    // Begin Serial Communications (Lunasat baud rate set to 9600)
    Serial.begin(9600);

    // Set time between samples to 1000ms (1s)
    timeBetweenSample = 1000;
};

void loop(){
    // Record start time for letter offseting of delay to make the loop run once per second
    startTime = millis();

    // Sample temperature in C
    temperatureC = thermometer.getTemperatureC();

    // Output the temperature via serial
    Serial.println(temperatureC);

    // Record the end time so we know how long it took to record the measurement
    endTime = millis();

    // Delay each loop by the sample rate offset by the time it took to get the temperature
    delay(timeBetweenSample - (endTime - startTime));
};