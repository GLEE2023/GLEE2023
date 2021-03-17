#include "GLEE_IR_Thermopile.h"

/**
 * Sketch functionality --> 
 * A sketch that detects a hand above the lunasat.
 * Approach: calculate average object temperature 
 * observed by lunasat and then compare against readings
 * after calibration.
 **/


Thermopile IRsensor; // Calling the thermopile class

// Variables to store the sensor and object temperature 
double sensorTemperature = 0.0; // Initialize sensor temperature to zero
double objectTemperature = 0.0; // Initialize object temperature to zero

// For storing output pin configuration of LED's
int LED = 9;

void setup(){
    // Begin Serial Communications
    Serial.begin(9600);

    // Get the beginning calibration value
    IRsensor.getCalibration();
    
    // Set pinMode as LED_low and LED_high as output
    pinMode(LED_low, OUTPUT);
    pinMode(LED_high, OUTPUT);
}

void loop(){
    // Read the ADC Dialectric Sensor
    // IRsensor.readADC(); 

    sensorTemperature = IRsensor.getSensorTemperature();
    objectTemperature = IRsensor.getObjectTemperature();

    // If statement to check if object temperature is the same to background temperature
    if(objectTemperature != sensorTemperature){ //hand is detected
        
        Serial.print("Hand is detected.");
        digitalWrite(LED, HIGH); // Turn LED on
        delay(1000);
        digitalWrite(LED, LOW); // Turn LED off
        
    } else {

        Serial.print("No hand detected.");
        Serial.print("The sensor temperature ");
        Serial.print(IRsensor.getSensorTemperature);
        Serial.print(" equals object temperature ");
        Serial.print(IRsensor.getObjectTemperature);
        delay(1000);

    }
}