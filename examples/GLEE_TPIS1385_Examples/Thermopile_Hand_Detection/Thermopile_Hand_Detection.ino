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

float sumTempDiff;
float avgTempDiff;
float tempDiff;

double thresh = 0.4; // The bubble of certainty allowed between average Sensor and average Object

TPsample_t temperatures; // Initialize sensor temperature to zero

void setup(){
    // Begin Serial Communications
    Serial.begin(9600);

    thermopile.begin();

    // set calibration values from by reading eeprom
    thermopile.readEEprom();
    
    // Set pinMode for LED
    pinMode(LED, OUTPUT);

    delay(50);

    for(int i = 0; i < 3; i++){ // Calibration period of 3 seconds

        temperatures = thermopile.getSample();
        
        Serial.print("Object: "); Serial.println(temperatures.object, 4);
        Serial.print("Ambient: "); Serial.println(temperatures.ambient, 4);

        sumTempDiff += abs(temperatures.object - temperatures.ambient);
    
        delay(200);
    };

    avgTempDiff = sumTempDiff/3;
};

void loop(){

    temperatures = thermopile.getSample(); // Read in the Analog to Digital Converter

    accumulator++; // Begin counting total accumulation

    if(accumulator == 500){ // Once accumulated a specific number of samples to average
        accumulator = 0; // Reset accumulator

        tempDiff = abs(temperatures.object - temperatures.ambient);

        Serial.print("Object: "); Serial.println(temperatures.object, 4);
        Serial.print("Ambient: "); Serial.println(temperatures.ambient, 4);
        Serial.print("Deviation from Avg Difference: "); Serial.println(abs(tempDiff-avgTempDiff));

        if(abs(tempDiff-avgTempDiff) > thresh){ // If Object and Sensor are the same
            Serial.println("Hand is detected.");
            digitalWrite(LED, HIGH);
            delay(500);
            digitalWrite(LED, LOW);
        }
        else{ // If Object and Sensor are NOT the same
            Serial.println("Hand is NOT detected.");
        }
    }
};
