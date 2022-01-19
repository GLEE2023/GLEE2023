#include "TPIS1385.h"

/**
 * Sketch functionality --> 
 * A sketch that detects a hand above the lunasat.
 * Approach: calculate average object temperature 
 * observed by lunasat and then compare against readings
 * after calibration.
 **/

TPIS1385 thermopile(1); // Instantiate thermopile object

const int LED = 4; // For storing output pin configuration of LED

int accumulator = 0; // Counter for average loops

float sumTempDiff; // Sum of differences between object and ambient temperature
float avgTempDiff;
float tempDiff; // Absolute difference between object and ambient temperature

double thresh = 0.4; // The bubble of certainty allowed between average Sensor and average Object

TPsample_t temperatures; // Initialize sensor temperature to Zero

void setup(){
    // Begin Serial Communications
    Serial.begin(9600);

    thermopile.begin();

    // Set calibration values by reading from EEprom
    thermopile.readEEprom();
    
    // Set pinMode for LED
    pinMode(LED, OUTPUT);

    delay(50);

    for(int i = 0; i < 3; i++){ // Calibration period of 3 seconds, calibrates to get current temps

        temperatures = thermopile.getSample();
        
        Serial.print("Object: "); Serial.println(temperatures.object, 4); // Prints object temp
        Serial.print("Ambient: "); Serial.println(temperatures.ambient, 4); // Prints ambient temp

        sumTempDiff += abs(temperatures.object - temperatures.ambient);
    
        delay(200);
    };

    avgTempDiff = sumTempDiff/3; // Saves average temperature difference 
};

void loop(){

    temperatures = thermopile.getSample(); // Read in the Analog to Digital Converter

    accumulator++; // Begin counting total accumulation

    if(accumulator == 500){ // Once accumulated a specific number of samples to average
        accumulator = 0; // Reset accumulator

        tempDiff = abs(temperatures.object - temperatures.ambient); // Find temp difference

        Serial.print("Object: "); Serial.println(temperatures.object, 4); // Prints object temp
        Serial.print("Ambient: "); Serial.println(temperatures.ambient, 4); // Prints ambient temp
        Serial.print("Deviation from Avg Difference: "); Serial.println(abs(tempDiff-avgTempDiff));

        if(abs(tempDiff-avgTempDiff) > thresh){ // If the difference of the current temperature difference
                                                // and average temperature difference is greater than the threshold,
            Serial.println("Hand is detected."); // Print message to serial monitor
            digitalWrite(LED, HIGH); // Blink LED
            delay(500);
            digitalWrite(LED, LOW);
        }
        else{ // If Object and Sensor are NOT the same
            Serial.println("Hand is NOT detected.");
        }
    }
};
