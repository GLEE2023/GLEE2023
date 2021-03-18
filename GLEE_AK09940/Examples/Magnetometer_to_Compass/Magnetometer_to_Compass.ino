//Dependencies
#include <Arduino.h>
#include "GLEE_Sensor.h"
#include "AK09940.h"
#include <math.h>
#include <String.h>

// Instantiate AKO9940 class with id of one and debugging set to true
AK09940 ak09940 = AK09940(1,true);

//Magnetic compass heading (calculated later)
float heading;

//For storing output pin configuration of LED
int LED = 4;

//Direction of compass as determined by heading
String direction;

void setup (){

    // Begin Serial Communications (Lunasat baud rate set to 9600)
    Serial.begin(9600);
    
    ak09940.setDriveMode(LOW_NOISE_1);
    ak09940.setMeasurementMode(POWER_DOWN);

    //Set pin LED as output
    pinMode(LED, OUTPUT);

};

void loop (){
    if (ak09940.getMeasurementMode() == POWER_DOWN){

        //Set measurement mode to SINGLE_MEASURE
        ak09940.setMeasurementMode(SINGLE_MEASURE);

        //Retrieve data from magnetometer
        ak09940.updateRawData();
        ak09940.updateCalculatedData();

        //Set aside magnitude of magnetic field in x and y directions for calculations
        float xMag = ak09940.calculatedData.xMag;
        float yMag = ak09940.calculatedData.yMag;

        //Determine compass heading (relative to magnetic north)
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

        //Set to Error in order to determine what information should be printed
        direction = "Error";

        //Determine if heading is valid and print general direction of compass
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
            delay(50 + (heading*4)); //Delay blink of LED based on value of heading
            digitalWrite(LED, LOW);

        } else {

            digitalWrite(LED, HIGH);
            delay(50 + ((360-heading)*4)); 
            digitalWrite(LED, LOW);

        }

    }

    delay(1000);
  
};
