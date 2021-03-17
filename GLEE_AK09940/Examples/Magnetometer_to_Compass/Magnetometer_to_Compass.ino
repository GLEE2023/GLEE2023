//Dependencies
#include <Arduino.h>
#include "GLEE_Sensor.h"
#include "AK09940.h"
#include <math.h>

// Instantiate AKO9940 class with id of one and debugging set to true
AK09940 ak09940 = AK09940(1,true);

//Magnetic compass heading (calculated later)
float heading;

//Used for calculating the true north bearing
float declination;

//Used for storing the true north bearing (not currently incorporated)
float trueHeading;

//For storing output pin configuration of LED
int LED = 4;

void setup (){
    // Begin Serial Communications (Lunasat baud rate set to 9600)
    Serial.begin(9600);
    
    ak09940.setDriveMode(LOW_NOISE_1); //Might change later, unsure of what drive mode is best
    ak09940.setMeasurementMode(POWER_DOWN);

    //Set pins LED1 and LED2 as output
    pinMode(LED, OUTPUT);

    //Should be changed depending on location of LunaSat on Earth or the Moon
    declination = 0.0;
};

void loop (){
    if (ak09940.getMeasurementMode() == POWER_DOWN){
        //Set measurement mode to SINGLE_MEASURE
        ak09940.setMeasurementMode(SINGLE_MEASURE);

        //Retrieve data from magnetometer
        ak09940.updateRawData();
        ak09940.updateCalculatedData();

        float xMag = ak09940.calculatedData.xMag;
        float yMag = ak09940.calculatedData.yMag;

        //Determine compass heading (magnetic north)
        if(yMag > 0){
            heading = 90 - (atan(xMag/yMag))*(180/M_PI); //M_PI = 3.141...
        } else if (yMag < 0){
            heading = 270 - (atan(xMag/yMag))*(180/M_PI);
        } else {
            if(xMag < 0){
                heading = 180;
            } else {
                heading = 0.0;
            }
        }

        //Determine if heading is indicative of magnetic north
        if(heading==0.0){
            Serial.println("Facing magnetic north");
        } else {
            Serial.print("Compass Heading (Magnetic): ");
            Serial.println(heading);
        }

        if(heading<=180.0){
            digitalWrite(LED, HIGH);
            delay(50 + (heading*4)); //Delay blink of LED based on value of heading
            digitalWrite(LED, LOW);
        } else {
            digitalWrite(LED, HIGH);
            delay(50 + ((360-heading)*4)); 
            digitalWrite(LED, LOW);
        }
        
        //TODO: Determine true north heading and print to serial
        trueHeading = heading + declination;

    }

    delay(1000);
  
};
