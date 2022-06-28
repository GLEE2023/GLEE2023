//Dependencies
#include <Arduino.h>
#include <GLEE_Radio.h>
#include "GLEE_Sensor.h"
#include "TMP117.h"

// Instantiate TMP117 class with id of one and debugging set to true
TMP117 thermometer(1,true);

// Initiate LunaSat Radio (SX1272 Transciever)
LunaRadio Rad;

// Initialize buffer and string variables used in data conversion for transmission
char data_buffer[16]; // Character array for string-numerical conversion
String temperatureGradientString;

// Observation Configuration
int sampleRate; // Time between samples in ms

// Dynamic Variables
float temperaturePastC; // Variable to store a previously observed temperature in Celsius
float temperatureCurrentC; //Variable to store a currently observed temperature in Celsius

float temperatureGradient; //Variable to store change in temperature per second
float gradientThreshold; //Variable to store threshold for the temperature gradient
float gradientMargin; //Variable to store margin for what is considered an unchanging temperature gradient

float startTime; // Variables for timing offset calculations
float endTime;

// For storing output pin configuration of LEDs
int LED1 = 4; //Used for when a positive temperature gradient exceeds the threshold
int LED2 = 5; //Used for when a negative temperature gradient exceeds the threshold

void setup(){
    // Begin Serial Communications (Lunasat baud rate set to 9600)
    Serial.begin(9600);

    // Set previously observed temperature to default value
    temperaturePastC = 0.0; 

    //Set currently observed temperature to default value
    temperatureCurrentC = 0.0; 

    //Set temperature gradient to default value
    temperatureGradient = 0.0;

    //Set temperature gradient threshold to a positive value (0.05 degrees per second by default)
    gradientThreshold = 0.05;

    //Set margin for unchanging temperature gradient to a positive value smaller than the threshold (.01 degrees per second by default)
    gradientMargin = 0.01;

    // Set time between samples to 1000ms (1s)
    sampleRate = 1000; 

    //Set pins LED1 and LED2 as output
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);

    // Initialize Transciever with following parameters
    // Argument 1: Set frequency to 915
    // Argument 2: Set output power to 17
    // Argument 3: Set Bandwidth to 250
    // Argument 4: Set spreading factor to 12
    // Argument 5: Set coding rate to 8
    Rad.initialize_radio(915.0, 17, 250.0, 12, 8);

    // Calibration
    int calPeriod = 5; // Calibration period in secs
    float calTemps[calPeriod]; // Array of sample initial temperatures

    // Measure first set of initial temperatures for calibration of gradient margin
    for (int i = 0; i<calPeriod; i++){

        startTime = millis();
       
        calTemps[i] = thermometer.getTemperatureC();
        Serial.print("Sample Temperature: ");
        Serial.println(calTemps[i]);
       
        endTime = millis();
       
        delay(1000-(endTime - startTime));

    }

    float calGrads[calPeriod - 1]; // Array to store initial temperature gradients
    float avgCalGrad = 0; // Average temperature gradient

    // Calculate initial temperature gradients based on initial temperature measurements
    for (int i = 1; i < calPeriod; i++){

        calGrads[i-1] = abs(calTemps[i] - calTemps[i-1]); 
        Serial.print("Sample Gradient: ");
        Serial.println(calGrads[i-1]);
        avgCalGrad += calGrads[i-1];

    }

    avgCalGrad /= (calPeriod-1);
    gradientMargin = avgCalGrad + 0.001; // Make gradient margin based on average gradient + .001 degrees
    Serial.print("Calculated Temperature Gradient Margin: ");
    Serial.println(gradientMargin);
    // Serial.println(avgCalGrad);
    
};

void loop(){
    // Record start time
    startTime = millis();
   
    // Set value to last recorded temperature
    temperaturePastC = temperatureCurrentC; 

    // Call the getTemperatureC() function of the TMP117 thermometer 
    temperatureCurrentC = thermometer.getTemperatureC();

    // Calculate change in temperature per second
    temperatureGradient = ((temperatureCurrentC - temperaturePastC) / (sampleRate/1000)); 

    // Print previous temperature and current measured temperature
    Serial.print("Past Temp:    "); Serial.println(temperaturePastC,5);
    Serial.print("Current Temp: "); Serial.println(temperatureCurrentC,5);

    //Print temperatuere gradient
    //Serial.print("Temperature gradient in degrees per second: ");
    //Serial.println(temperatureGradient, 5);

    // Print a detection state if the temperature gradient is unchanging within a small margin, or meeting or exceeding the gradient threshold
    if(abs(temperatureGradient) <= gradientMargin){

        Serial.print("Temperature is stable and is changing by no more than ");
        Serial.print(gradientMargin);
        Serial.print(" degrees (Celsius) per second for ");
        Serial.print(sampleRate/1000);
        Serial.println(" second(s)");

    }else{
        // Gradient exceeds detection margin
        if(temperatureGradient >= 0 ){

            Serial.print("Temperature rising by ");
            Serial.print(temperatureGradient, 5);
            Serial.print(" degrees (Celsius) per second for ");
            Serial.print(sampleRate/1000);
            Serial.println(" second(s)");

            // If the temperature gradient exceeds the gradient threshold, flash the LED and transmit message
            if(abs(temperatureGradient) >= gradientThreshold){

                digitalWrite(LED1, HIGH);   // Turn LED on
                delay(sampleRate/2);        // Keep LED on for a delay of half of the time of the sampleRate in milliseconds
                digitalWrite(LED1, LOW);    // Turn LED off

                // If theshold exceeded, transmit temperature gradient
                Serial.println("Threshold Exceeded, Transmitting Temperature Gradient...");
                // Convert float temperature gradient to arduino String
                temperatureGradientString = String(temperatureGradient);
                // Convert string to byte/char array stored in buffer
                temperatureGradientString.toCharArray(data_buffer,8);
                // Transmit buffer containing temperature in the form of a byte array
                Rad.transmit_data(data_buffer);

            }

        } else {

            Serial.print("Temperature falling by ");
            Serial.print(temperatureGradient, 5);
            Serial.print(" degrees (Celsius) per second for ");
            Serial.print(sampleRate/1000);
            Serial.println(" second(s)");

            // If the temperature gradient exceeds the gradient threshold, flash the LED and transmit message
            if(abs(temperatureGradient) >= gradientThreshold){

                digitalWrite(LED2, HIGH);   // Turn LED on
                delay(sampleRate/2);        // Keep LED on for a delay of half of the time of the sampleRate in milliseconds
                digitalWrite(LED2, LOW);    // Turn LED off

                // If theshold exceeded, transmit temperature gradient
                Serial.println("Threshold Exceeded, Transmitting Temperature Gradient...");
                // Convert float temperature gradient to arduino String
                temperatureGradientString = String(temperatureGradient);
                // Convert string to byte/char array stored in buffer
                temperatureGradientString.toCharArray(data_buffer,8);
                // Transmit buffer containing temperature in the form of a byte array
                Rad.transmit_data(data_buffer);
                
            }
        }  
    } 
    // Record end time
    endTime = millis();

    // Delay the loop based on the length of the measurement and the sampleRate
    delay(sampleRate - (endTime - startTime));
};