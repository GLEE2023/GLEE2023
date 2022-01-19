// Include dependencies (libraries)
#include <Arduino.h>
#include <GLEE_Radio.h>
#include <GLEE_Sensor.h>
#include <TMP117.h>


// Instantiate TMP117 class with id of one and debugging set to false
TMP117 thermometer(1,false);

LunaRadio Rad;

char MSG[16];
String msg;

// Observation configuration
int sampleRate; // Sample rate in samples per second (Hz)

// Dynamic Variables 
float temperatureC;  // Variable to store an observed temperature in Celsius
float temperatureF;  // Variable to store an observed temperature in Fahrenheit
float startTime;    // Variables for timing offset calculations
float endTime;

void setup(){
    // Begin Serial Communications (Lunasat baud rate set to 9600)
    Serial.begin(9600);
    Rad.initialize_radio();
    delay(50);

    // Set sample rate to 1 sample per second (1Hz)
    sampleRate = 1;
};

void loop(){
    // Record start time for letter offseting of delay to make the loop run once per second
    startTime = millis();

    // Call the getTemperatureC() function of the TMP117 thermometer  
    temperatureC = thermometer.getTemperatureC();

    // Output the temperature via seiral
    Serial.println(temperatureC);

    msg = String(temperatureC);
    msg.toCharArray(MSG,8);
    Rad.transmit_data(MSG);

    // Record the end time so we know how long it took to record the measurement
    endTime = millis();

    // Delay each loop by the sample rate off set by the time it took to get the temperature
    delay(sampleRate * 1000 - (endTime - startTime));
};
