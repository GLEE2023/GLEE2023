//Dependencies
#include <Arduino.h>
#include "GLEE_Sensor.h"
#include "TMP117.h"

// Instantiate TMP117 class with id of one and debugging set to true
TMP117 thermometer(1,true);

// Observation Configuration
int sampleRate; // Sample rate in samples per second (Hz)

//Dynamic Variables
float temperaturePastC; // Variable to store a previously observed temperature in Celsius
float temperatureCurrentC; //Variable to store a currently observed temperature in Celsius

float temperatureGradient; //Variable to store change in temperature per second
float gradientThreshold; //Variable to store threshold for the temperature gradient

float startTime; // Variables for timing offset calculations
float endTime;

int LED1 = 4; //For storing output pin configuration of LED
int LED2 = 5;

void setup(){
    // Begin Serial Communications (Lunasat baud rate set to 9600)
    Serial.begin(9600);

    // Set previously observed temperature to default value
    temperaturePastC = 0; 

    //Set currently observed temperature to default value
    temperatureCurrentC = 0; 

    //Set temperature gradient to default value
    temperatureGradient = 0;

    //Set temperature gradient threshold to a positive value (1 degree per second by default)
    gradientThreshold = 1;

    // Set sample rate to 1 sample per second (1Hz)
    sampleRate = 1; 

    //Set pins LED1 and LED2 as output
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
};

void loop(){
    startTime = millis();
   
    // Set value to last recorded temperature
    temperaturePastC = temperatureCurrentC; 

    // Call the getTemperatureC() function of the TMP117 thermometer 
    temperatureCurrentC = thermometer.getTemperatureC();

    //Calculate change in temperature per second
    temperatureGradient = (temperatureCurrentC - temperaturePastC) * sampleRate; 

    //Print a detection state and blink LED if magnitude of temperature gradient meets or exceeds threshold
    if(abs(temperatureGradient) >= gradientThreshold){

        if(temperatureGradient >= 0 ){
            Serial.print("Temperature rising by ");
            Serial.print(temperatureGradient);
            Serial.print("degrees (Celsius) per second for ");
            Serial.print((1/sampleRate));
            Serial.println(" second(s)");

            digitalWrite(LED1, HIGH); //Turn LED on
            delay(((1/sampleRate)*1000)/10); //Keep LED on for a delay of on_time milliseconds based on sampleRate
            digitalWrite(LED1, LOW); //Turn LED off
        } else {
            Serial.print("Temperature falling by ");
            Serial.print(temperatureGradient);
            Serial.print("degrees (Celsius) per second for ");
            Serial.print((1/sampleRate));
            Serial.println(" second(s)");

            digitalWrite(LED2, HIGH);
            delay(((1/sampleRate)*1000)/10); 
            digitalWrite(LED2, LOW); 
        }
   
    }

    endTime = millis();

    delay(sampleRate * 1000 - (endTime - startTime));
};