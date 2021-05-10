#include <Arduino.h>
#include "TMP117.h"
#include <GLEE_Radio.h>

//only x-axis threshold implemented, but can add y and z if needed

// Initiate LunaSat Radio (SX1272 Transciever)
LunaRadio Rad;

// Instantiate TMP117 class with id of one and debugging set to true
TMP117 thermometer(1,true);


// Initialize buffer and string variables used in data conversion for transmission
char data_buffer[16]; // Character array for string-numerical conversiona
String temperature_string;

// Initialize 2-Byte array for data transmission
//byte temperature_data = {0x00, 0x00};

// Temperature Threshold Value
int temperature_threshold = 79; //user-defined threshold

// Dynamic Variable 
float temperatureF;  // Variable to store an observed temperature in Fahrenheit
float startTime;    // Variables for timing offset calculations
float endTime;

//Delay between samples in ms
int sampleDelay = 1000;

void setup(){
    Serial.begin(9600);

    // Initialize Transciever with following parameters
    // Argument 1: Set frequency to 915
    // Argument 2: Set output power to 17
    // Argument 3: Set Bandwidth to 250
    // Argument 4: Set spreading factor to 12
    // Argument 5: Set coding rate to 8
    Rad.initialize_radio(915.0, 17, 250.0, 12, 8);
}

void loop(){
    // Record start time for offsetting delay by run time
    startTime = millis();

    // Call the getTemperatureF() function of the TMP117 thermometer 
    temperatureF = thermometer.getTemperatureF();

    // Output the temperature via seiral
    Serial.println(temperatureF);
    
    // Check if the temperature exceeds the threshold
    if(temperatureF > temperature_threshold){
        // If theshold exceeded, transmit temperature
        Serial.println("Threshold Exceeded, Transmitting Temperature...");
        
        // Convert int temperature to arduino String
        temperature_string = String(temperatureF);
        // Convert string to byte/char array stored in buffer
        temperature_string.toCharArray(data_buffer,8);
        // Transmit buffer containing temperature in the form of a byte array
        Rad.transmit_data(data_buffer);
    }

    // Record the end time for delay offset
    endTime = millis();
    
    delay(sampleDelay - (endTime - startTime));  
}
