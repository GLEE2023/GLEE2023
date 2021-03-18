#include "GLEE_IR_Thermopile.h"

/**
 * Sketch functionality --> 
 * A sketch that detects a hand above the lunasat.
 * Approach: calculate average object temperature 
 * observed by lunasat and then compare against readings
 * after calibration.
 **/


Thermopile IRsensor; // Calling the thermopile class

float startTime = 0;
float endTime = 0;

double sensorTemperature = 0.0; // Initialize sensor temperature to zero

// For storing output pin configuration of LED
const int LED = 4;

int accumulator = 0;
double sumObjectTemperature = 0.0;
double sumSensorTemperature = 0.0;
double error = 0.5;


void setup(){
    // Begin Serial Communications
    Serial.begin(9600);

    // Get the beginning calibration value
    IRsensor.getCalibration();
    
    // Set pinMode for LED
    pinMode(LED, OUTPUT);
};

void loop(){

    IRsensor.readADC(); 

    accumulator++;

    sumObjectTemperature+= IRsensor.getObjectTemperature();
    sumSensorTemperature+= IRsensor.getSensorTemperature();

    if(accumulator == 100){
        accumulator = 0;
        double object_averageLoop = sumObjectTemperature / 100.0;
        double sensor_averageLoop = sumSensorTemperature / 100.0;

        if(abs(object_averageLoop) < abs(sensor_averageLoop - error){
            Serial.println("Hand is detected.");
            digitalWrite(LED, HIGH); // Turn LED on
        }
        else{
            Serial.println("Hand is NOT detected.");
            digitalWrite(LED, LOW); // Turn LED off
        }

        sumSensorTemperature = 0.0;
        sumObjectTemperature = 0.0;
    }
};