#include <Arduino.h>
#include "GLEE_Sensor.h"

// LED output pin configurations
int LED1 = 4;
int LED2 = 5;

// Delay variables in milliseconds
int on_time = 3000; // For repeated blinking
unsigned long single_delay = 600000; // For a single blink

bool singleDelay = false; // True when testing with a single blink and false when testing with repeated blinking

void setup(){
    Serial.begin(9600);
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    delay(5000);
    if(singleDelay){ // For a single blink

        Serial.println(F("Starting wait for single delay..."));
        delay(2000);
        Serial.println(F("Starting in 3"));
        delay(1000);
        Serial.println(F("Starting in 2"));
        delay(1000);
        Serial.println(F("Starting in 1"));
        delay(1000);
        Serial.println(F("Starting delay now."));

        delay(single_delay);
        digitalWrite(LED1, HIGH);
        digitalWrite(LED2, HIGH);
        Serial.print(F("Blink"));
        delay(1000);
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, LOW);

    } else {
        Serial.println("Starting repeating blink...");
        delay(2000);
        Serial.println("Starting in 3");
        delay(1000);
        Serial.println("Starting in 2");
        delay(1000);
        Serial.println("Starting in 1");
        delay(1000);
        Serial.println("Starting time now.");
    }
};

void loop(){
    if(!singleDelay){ // For repeated blinking

        digitalWrite(LED1, HIGH);
        digitalWrite(LED2, HIGH);
        Serial.println(F("Blink on"));
        delay(on_time);

        digitalWrite(LED1, LOW);
        digitalWrite(LED2, LOW);
        Serial.println(F("Blink off"));
        delay(on_time);

    }
};