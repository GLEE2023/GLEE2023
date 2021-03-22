#include "TPIS1385.h"

/**
 * Sketch functionality --> 
 * A sketch that detects a hand above the lunasat.
 * Approach: calculate average object temperature 
 * observed by lunasat and then compare against readings
 * after calibration.
 **/

TPIS1385 thermopile(1); // Instantiate thermopile object

// float startTime = 0.0;
// float endTime = 0.0;

double sensorTemperature = 0.0; // Initialize sensor temperature to zero

const int LED = 4; // For storing output pin configuration of LED

int accumulator = 0; // Counter for average loops

double sumObjectTemperature = 0.0; // Total to calculate Object Temperature average

double sumSensorTemperature = 0.0; // Total to calculate Sensor Temperature average

double error = 0.5; // The bubble of certainty allowed between average Sensor and average Object

TPsample_t temperatures;

void setup(){
    // Begin Serial Communications
    Serial.begin(9600);

    // set calibration values from by reading eeprom
    thermopile.readEEprom();
    
    // Set pinMode for LED
    pinMode(LED, OUTPUT);
};

void loop(){

    temperatures = thermopile.getSample(); // Read in the Analog to Digital Converter

    accumulator++; // Begin counting total accumulation

    sumObjectTemperature+= temperatures.object; // Get the sum of Object Temperatures
    sumSensorTemperature+= temperatures.ambient; // Get the sum of Sensor Temperatures

    if(accumulator == 1000){ // Once accumulated a specific number of samples to average
        accumulator = 0; // Reset accumulator
        double object_averageLoop = sumObjectTemperature / 1000.0; // Calculate Object Temeprature average
        double sensor_averageLoop = sumSensorTemperature / 1000.0; // Calculate Sensor Temperature average

        if(abs(object_averageLoop) < abs(sensor_averageLoop - error)){ // If Object and Sensor are the same
            Serial.println("Hand is detected.");
            digitalWrite(LED, HIGH); // Turn LED on
        }
        else{ // If Object and Sensor are NOT the same
            Serial.println("Hand is NOT detected.");
            digitalWrite(LED, LOW); // Turn LED off
        }

        sumSensorTemperature = 0.0; // Reset Sensor sum
        sumObjectTemperature = 0.0; // Reset Object sum
    }
};