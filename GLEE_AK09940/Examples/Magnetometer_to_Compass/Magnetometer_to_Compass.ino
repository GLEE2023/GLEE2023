// Dependencies
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

float minHeadingOne = 0.0; // Minimum heading for between 0 and 180 degrees
float maxHeadingOne = 180.0; // Maximum heading for between 0 and 180 degrees
float minHeadingTwo = 180.0; // Minimum heading for between 180 and 360 degrees
float maxHeadingTwo = 360.0; // Maximum heading for between 180 and 360 degrees

/*float rawNorth;
float rawSouth;
float rawEast;
float rawWest;*/

/**
 * Parameters: float xMag (magnitude of magnetic field in x direction), 
 * float yMag (magnitude of magnetic field in y direction)
 * Returns: compass heading 
 * This function uses the raw magnetomtete readings to output
 * a heading. It does not take into account calibrated ranges so it is
 * only used in setup.
**/
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

    Serial.println(F("Spin the magnetometer in all directions for calibration in 3"));
    delay(1000);
    Serial.println(F("2"));
    delay(1000);
    Serial.println(F("1"));
    delay(1000);
    Serial.println(F("Go"));

    // Calibration Code

    AK_Sample_t calMag[1000]; // Array of sample initial readings

    // Get sample readings
    for(int i = 0; i < 1000; i++){
        calMag[i] = magnetometer.getSample();
        delay(10);
    }

    Serial.println(F("Done"));
    delay(1000);
    Serial.println(F("Performing calibration calculations..."));
    
    float calHeadings[1000]; // Array of headings for initial readings

    // Find headings for each initial reading
    for(int k = 0; k < 1000; k++){
        calHeadings[k] = findHeading(calMag[k].magnetic.x,calMag[k].magnetic.y);
    }

    // Find the ranges of values for the headings
    for(int l = 0; l < 500; l++){
        if(calHeadings[l] <= 180){
            if((minHeadingOne == 0.0) && (calHeadings[l]>=0.0)){
                minHeadingOne = calHeadings[l];
            }
            if(maxHeadingOne == 180.0){
                maxHeadingOne = calHeadings[l];
            }
            if((minHeadingOne > calHeadings[l]) && (calHeadings[l]>=0.0)){
                minHeadingOne = calHeadings[l];
            }
            if(maxHeadingOne < calHeadings[l]){
                maxHeadingOne = calHeadings[l];
            }
        } else {
            if(minHeadingTwo == 180.0){
                minHeadingTwo = calHeadings[l];
            }
            if((maxHeadingTwo == 360.0) && (calHeadings[l]<=360.0)){
                maxHeadingTwo = calHeadings[l];
            }
            if(minHeadingTwo > calHeadings[l]){
                minHeadingTwo = calHeadings[l];
            }
            if((maxHeadingTwo < calHeadings[l]) && (calHeadings[l]<=360.0)){
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

    Serial.println(F("Ready to use.")); // Print that magnetometer has been calibrated
    delay(100);
    Serial.println();
    Serial.println();
};

void loop (){
    Serial.print(F("Data Ready: ")); Serial.println(magnetometer.dataReady());
    sample = magnetometer.getSample(); // Get sample

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

    // Set to Error by default in order to determine what information should be printed
    direction = "Error";

    // Determine if heading is valid and print general direction of compass
    if(heading < 0.0 || heading > 360.0){
        Serial.print("Error. Heading should be between 0 and 360 degrees. Your heading is: ");
        Serial.print(heading);
        Serial.println(" degrees");
    } else {
        // Print message to serial if compass is pointing exactly or almost exactly in an particular direction
        if((abs(heading-0.0) < 0.5) || (abs(heading-360.0) < 0.5)){
            Serial.println("Facing due north");
            direction = "north";
        } else if (abs(heading-45.0) < 0.5){
            Serial.println("Facing due northeast");
            direction = "northeast";
        } else if (abs(heading-90.0) < 0.5){
            Serial.println("Facing due east");
            direction = "east";
        } else if (abs(heading-135.0) < 0.5){
            Serial.println("Facing due southeast");
            direction = "southeast";
        } else if (abs(heading-180.0) < 0.5){
            Serial.println("Facing due south");
            direction = "south";
        } else if (abs(heading-225.0) < 0.5){
            Serial.println("Facing due southwest");
            direction = "southwest";
        } else if (abs(heading-270.0) < 0.5){
            Serial.println("Facing due west");
            direction = "west";
        } else if (abs(heading-315.0) < 0.5){
            Serial.println("Facing due northwest");
            direction = "northwest";
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

    // Print out direction and heading of compass regardless of its heading
    if(direction != "Error"){
        Serial.print("Direction: ");
        Serial.println(direction);
        Serial.print("Compass Heading: ");
        Serial.print(heading,5);
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

    delay(1000);

};
