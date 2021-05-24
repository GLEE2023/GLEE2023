//Dependencies
#include <Arduino.h>
#include "GLEE_Sensor.h"
#include "AK09940.h"
#include <math.h>
#include <String.h>

// Instantiate AKO9940 class with id of one and debugging set to true
AK09940 magnetometer = AK09940(1,true);

AK_Sample_t sample;

//Magnetic compass heading (calculated later)
float heading;

//For storing output pin configuration of LED
int LED = 4;

//Direction of compass as determined by heading
String direction;

float smallestdegree;
float largestdegree;

float minHeadingOne = 0;
float maxHeadingOne = 180;
float minHeadingTwo = 180;
float maxHeadingTwo = 360;

/*float rawNorth;
float rawSouth;
float rawEast;
float rawWest;*/

float findHeading(float xMag, float yMag){
    if(yMag > 0){

        heading = 270 - (atan(xMag/-yMag))*(180/M_PI); //M_PI = 3.141...

    } else if (yMag < 0){

        heading = 90 - (atan(xMag/-yMag))*(180/M_PI);

    } else {

        if(xMag < 0){

            heading = 180;

        } else {

            heading = 0.0;

        }
    }
    return heading;
}

void setup (){

    // Begin Serial Communications (Lunasat baud rate set to 9600)
    Serial.begin(9600);
    delay(3000);
    magnetometer.begin();
    magnetometer.setOpMode(true, LOW_POWER_1, POWER_DOWN);

    // Set pin LED as output
    pinMode(LED, OUTPUT);

    Serial.println(F("Spin the magnetometer in all directions in 3"));
    delay(1000);
    Serial.println(F("2"));
    delay(1000);
    Serial.println(F("1"));
    delay(1000);
    Serial.println(F("Go"));

    AK_Sample_t calMag[1000]; // Array of sample initial readings
    // Get sample readings
    for(int i = 0; i < 1000; i++){
        calMag[i] = magnetometer.getSample();
    }

    Serial.println(F("Performing calibration calculations..."));
    delay(1000);

    float calHeadings[1000];

    // Find headings for each initial reading
    for(int k = 0; k < 1000; k++){
        calHeadings[k] = findHeading(calMag[k].magnetic.x,calMag[k].magnetic.y);
    }

    // Find the ranges of values for the headings
    for(int l = 1; l < 500; l++){
        if(calHeadings[l] <= 180){
            if(minHeadingOne = 0){
                minHeadingOne = calHeadings[l];
            }
            if(maxHeadingOne = 180){
                maxHeadingOne = calHeadings[l];
            }
            if(minHeadingOne > calHeadings[l]){
                minHeadingOne = calHeadings[l];
            }
            if(maxHeadingOne < calHeadings[l]){
                maxHeadingOne = calHeadings[l];
            }
        } else {
            if(minHeadingTwo = 180){
                minHeadingTwo = calHeadings[l];
            }
            if(maxHeadingTwo = 360){
                maxHeadingTwo = calHeadings[l];
            }
            if(minHeadingTwo > calHeadings[l]){
                minHeadingTwo = calHeadings[l];
            }
            if(maxHeadingTwo < calHeadings[l]){
                maxHeadingTwo = calHeadings[l];
            }
        }
    }

    /*float maxXMag = calMag[0].magnetic.x;
    float associatedMaxX = calMag[0].magnetic.y;
    float maxYMag = calMag[0].magnetic.y;
    float associatedMaxY = calMag[0].magnetic.x;
    float minXMag = calMag[0].magnetic.x;
    float associatedMinX = calMag[0].magnetic.y;
    float minYMag = calMag[0].magnetic.y;
    float associatedMinY = calMag[0].magnetic.x;*/

    /*for(int j = 1; j < 500; j++){
        if(calMag[j].magnetic.x > maxXMag){
            maxXMag = calMag[j].magnetic.x;
            associatedMaxX = calMag[j].magnetic.y;
        }
        if(calMag[j].magnetic.x < minXMag){
            minXMag = calMag[j].magnetic.x;
            associatedMinX = calMag[j].magnetic.y;
        }
        if(calMag[j].magnetic.x > maxYMag){
            maxYMag = calMag[j].magnetic.y;
            associatedMaxY = calMag[j].magnetic.x;
        }
        if(calMag[j].magnetic.x < minXMag){
            minYMag = calMag[j].magnetic.y;
            associatedMinY = calMag[j].magnetic.x;
        }
    }

    rawNorth = findHeading(maxXMag,associatedMaxX);
    rawSouth = findHeading(minXMag,associatedMinX);
    rawEast = findHeading(maxYMag,associatedMaxY);
    rawWest = findHeading(minYMag,associatedMinY);*/

    Serial.println(F("Ready to use."));
    delay(100);
    Serial.println();
    Serial.println();

};

void loop (){

    Serial.print(F("Data Ready: ")); Serial.println(magnetometer.dataReady());
    sample = magnetometer.getSample();

    // Set aside magnitude of magnetic field in x and y directions for calculations
    float xMag = sample.magnetic.x;
    float yMag = sample.magnetic.y;

    // Determine compass heading (relative to magnetic north)
    if(yMag > 0){
        heading = 270 - (atan(xMag/-yMag))*(180/M_PI); // M_PI = 3.141...
        heading = 180 + ((360 - 180) / (maxHeadingTwo - minHeadingTwo)) * (heading - minHeadingTwo); // Use calibration values
        
    } else if (yMag < 0){
        heading = 90 - (atan(xMag/-yMag))*(180/M_PI);
        heading = 0 + ((180 - 0) / (maxHeadingOne - minHeadingOne)) * (heading - minHeadingOne); // Use calibration values
        
    } else {

        if(xMag < 0){
            heading = 180;

        } else {
            heading = 0.0;

        }
    }

    // Set to Error in order to determine what information should be printed
    direction = "Error";

    // Determine if heading is valid and print general direction of compass
    if(heading < 0.0 || heading > 360.0){

        Serial.print("Error. Heading should be between 0 and 360 degrees. Your heading is: ");
        Serial.print(heading);
        Serial.println(" degrees");

    } else {

        if(heading==0.0 || heading==360.0){
            Serial.println("Facing due north (Heading of 0.0 degrees)");

        } else if (heading == 45.0){
            Serial.println("Facing due northeast (Heading of 45.0 degrees)");

        } else if (heading == 90.0){
            Serial.println("Facing due east (Heading of 90.0 degrees)");

        } else if (heading == 135.0){
            Serial.println("Facing due southeast (Heading of 135.0 degrees)");

        } else if (heading == 180.0){
            Serial.println("Facing due south (Heading of 180.0 degrees)");

        } else if (heading == 225.0){
            Serial.println("Facing due southwest (Heading of 225.0 degrees)");

        } else if (heading == 270.0){
            Serial.println("Facing due west (Heading of 270.0 degrees)");

        } else if (heading == 315.0){
            Serial.println("Facing due northwest (Heading of 315.0 degrees)");

        } else if (heading <= 22.5 || heading > 337.5){
            direction = "north";

        } else if (heading <= 67.5 || heading > 22.5){
            direction = "northeast";

        } else if (heading <= 112.5 || heading > 67.5){   
            direction = "east";

        } else if (heading <= 157.5 || heading > 112.5){  
            direction = "southeast";

        } else if (heading <= 202.5 || heading > 157.5){    
            direction = "south";

        } else if (heading <= 247.5 || heading > 202.5){   
            direction = "southwest";

        } else if (heading <= 292.5 || heading > 247.5){    
            direction = "west";

        } else if (heading <= 337.5 || heading > 292.5){   
            direction = "northwest";
        }

    }

    if(direction != "Error"){
        Serial.print("Direction: ");
        Serial.println(direction);
        Serial.print("Compass Heading: ");
        Serial.print(heading);
        Serial.println(" degrees");

    }
    
    if(heading<=180.0){
        digitalWrite(LED, HIGH);
        delay(50 + (heading*4)); // Delay blink of LED based on value of heading
        digitalWrite(LED, LOW);

    } else {
        digitalWrite(LED, HIGH);
        delay(50 + ((360-heading)*4)); // Delay blink of LED based on value of heading
        digitalWrite(LED, LOW);

    }

    delay(100);
  
};
