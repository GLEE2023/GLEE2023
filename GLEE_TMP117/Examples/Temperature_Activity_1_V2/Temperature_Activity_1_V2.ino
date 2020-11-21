// Include dependencies (libraries)
#include <Arduino.h>
#include "GLEE_Sensor.h"
#include "TMP117.h"

// Instantiate TMP117 class with id of one and debugging set to true
TMP117 thermometer(1,true);

// Observation configuration
int sampleRate; // Sample rate in samples per second (Hz)

// Dynamic Variable 
float temperature;  // Variable to store an observed temperature
float startTime;    // Variables for timing offset calculations
float endTime;

void setup(){
    // Set sample rate to 1 sample per second (1Hz)
    sampleRate = 1;
};

void loop(){
    // Record start time for letter offseting of delay to make the loop run once per second
    startTime = millis();

    // Call the getTemperatureC() function of the TMP117 thermometer  
    temperature = thermometer.getTemperatureC();

    // Output the temperature via seiral
    Serial.println(temperature);

    // Record the end time so we know how long it took to record the measurement
    endTime = millis();

    // Delay each loop by the sample rate off set by the time it took to get the temperature
    delay(sampleRate * 1000 - (endTime - startTime));
};