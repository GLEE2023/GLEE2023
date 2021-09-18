// Dependencies
#include <Arduino.h>
#include "GLEE_Sensor.h"
#include "MLX90393.h"
#include <math.h>
#include <String.h>

// Instantiate MLX90393 class with id of one and debugging set to true
MLX90393 magnetometer = MLX90393(1,true);

mlx_sample_t sample; // Sample of type mlx_sample_t to hold magnetometer data

float heading; // Magnetic compass heading (calculated later)

int LED = 4; // For storing output pin configuration of LED

String direction_str; // Direction of compass as determined by heading

float minX = 3400000.0; // Minimum heading for between 0 and 180 degrees
float maxX = -3400000.0; // Maximum heading for between 0 and 180 degrees
float minY = 3400000.0; // Minimum heading for between 180 and 360 degrees
float maxY = -3400000.0; // Maximum heading for between 180 and 360 degrees
float minZ = 3400000.0;
float maxZ = -3400000.0;
float minHeadingOne = 180.0;
float maxHeadingOne = 0.0;
float minHeadingTwo = 360.0;
float maxHeadingTwo = 180.0;

float xMag = 0.0; // Magnitude of magnetic field in x direction
float yMag = 0.0; // Magnitude of magnetic field in y direction
float zMag = 0.0; // Magnitude of magnetic field in z direction

float avg_delta_x = (maxX - minX) / 2;
float avg_delta_y = (maxY - minY) / 2;
float avg_delta_z = (maxZ - minZ) / 2;

//int tmp = 0; // Temporary variable

void setup (){
    // Begin Serial Communications (Set Lunasat baud rate set to 9600)
    Serial.begin(9600);

    delay(3000);

    magnetometer.begin_I2C();

    // Set gain
    magnetometer.setGain(MLX90393_GAIN_2_5X);

    // Set resolution
    magnetometer.setResolution(MLX90393_X, MLX90393_RES_19); // May need to be changed
    magnetometer.setResolution(MLX90393_Y, MLX90393_RES_19); // May need to be changed
    magnetometer.setResolution(MLX90393_Z, MLX90393_RES_16); // May need to be changed

    // Set oversampling
    magnetometer.setOversampling(MLX90393_OSR_2); // May need to be changed

    // Set digital filtering
    magnetometer.setFilter(MLX90393_FILTER_6); // May need to be changed

    // Set pin LED as output
    pinMode(LED, OUTPUT);

    Serial.println("Before using the magnetometer, it will be need to be calibrated by spinning it in all possible directions.");
    delay(3000);
    //Serial.println("To calibrate, orient the magnetometer and then send an input to the serial monitor. Do this repeatedly.");
    //delay(5000);
    Serial.println("Make sure no wires are loose before spinning the LunaSat.");
    delay(5000);
    Serial.print("Starting calibration in 3,");
    delay(1000);
    Serial.print("2,");
    delay(1000);
    Serial.print("1,");
    delay(1000);
    Serial.println("Go!");
    delay(1000);

    // Calibration Code

    int calibrationSize = 100; // Number of data points to be used for calibration

    for(int i = 0; i < calibrationSize; i++){
        delay(10);
        //while(!(Serial.available() > 0)){} // Rather than collecting data automatically, input can be used to gather data at chosen time by user
        //tmp = Serial.read();
        mlx_sample_t calMag = magnetometer.getSample();

        //Serial.println(calMag.magnetic.x);
        //Serial.println(calMag.magnetic.y);

        
        heading = atan2(calMag.magnetic.y,calMag.magnetic.x);
        heading = heading * (180/M_PI);

        while(heading < 0) {
            heading = 360.0 + heading;
        }
        
        while(heading > 360) {
            heading = heading - 360.0;
        }

        if(heading < minHeadingOne && heading >= 0.0){
            minHeadingOne = heading;
        }
        if(heading < minHeadingTwo && heading >= 180.0){
            minHeadingTwo = heading;
        }
        
        if(heading > maxHeadingOne && heading <= 180.0){
            maxHeadingOne = heading;
        }
        if(heading > maxHeadingTwo && heading <= 360.0){
            maxHeadingTwo = heading;
        }

        
        if(calMag.magnetic.x < minX){
          minX = calMag.magnetic.x;
        }

        if(calMag.magnetic.x > maxX){
          maxX = calMag.magnetic.x;
        }

        if(calMag.magnetic.y < minY){
          minY = calMag.magnetic.y;
        }
        if(calMag.magnetic.y > maxY){
          maxY = calMag.magnetic.y;
        }
        if(calMag.magnetic.z < minZ){
          minZ = calMag.magnetic.z;
        }
        if(calMag.magnetic.z > maxZ){
          maxZ = calMag.magnetic.z;
        }
        
    }

    Serial.println(minX);
    Serial.println(maxX);
    Serial.println(minY);
    Serial.println(maxY);
    Serial.println(minZ);
    Serial.println(maxZ);

    avg_delta_x = (maxX - minX) / 2;
    avg_delta_y = (maxY - minY) / 2;
    avg_delta_z = (maxZ - minZ) / 2;
    
    Serial.println("Completed calibration."); // Print that magnetometer has been calibrated
    delay(1000);
    Serial.println("Ready to use."); 
    delay(1000);
    Serial.println();
    Serial.println();
};



void loop (){
    sample = magnetometer.getSample(); // Get sample

    // Set aside magnitude of magnetic field in x and y directions for calculations
    xMag = sample.magnetic.x;
    yMag = sample.magnetic.y;
    zMag = sample.magnetic.z;
    
    float avg_delta = (avg_delta_x + avg_delta_y + avg_delta_z) / 3;
    
    float scale_x = avg_delta / avg_delta_x;
    float scale_y = avg_delta / avg_delta_y;
    float scale_z = avg_delta / avg_delta_z;
    
    xMag = xMag * scale_x;
    yMag = yMag * scale_y;
    zMag = zMag * scale_z;
    
    // Determine compass heading
    /*
    float rads = atan2(xMag,yMag);
    heading = rads;
    heading = heading * (180/M_PI);
    
    if (yMag > 0){
        heading = 90 - heading;
        //heading = 180 + ((180 - 0) / (maxHeadingOne - minHeadingOne)) * (heading - minHeadingOne); // Use calibration values
    } else if (yMag < 0){
        heading = 270 - heading;
        //heading = 0 + ((360 - 180) / (maxHeadingTwo - minHeadingTwo)) * (heading - minHeadingTwo); // Use calibration values
    } else if (xMag < 0){
      heading = 180.0;
    } else {
      heading = 0.0;
    }
    */

    
    

    

        
    if(yMag > 0){
        heading = 90 - (atan(xMag/yMag))*(180/M_PI); // M_PI = 3.141...
        heading = 180 + ((360 - 180) / (maxHeadingOne - minHeadingOne)) * (heading - minHeadingOne); // Use calibration values   
    } else if (yMag < 0){
        heading = 270 - (atan(xMag/yMag))*(180/M_PI);
        heading = 0 + ((180 - 0) / (maxHeadingTwo - minHeadingTwo)) * (heading - minHeadingTwo); // Use calibration values  
    } else {
        if(xMag < 0){
            heading = 180;
        } else {
            heading = 0.0;
        }
    }
    

    while(heading < 0) {
        heading = 360.0 + heading;
    }
    
    while(heading > 360) {
        heading = heading - 360.0;
    }

    // Set to Error by default in order to determine what information should be printed
    direction_str = "Error";

     // Determine if heading is valid and print general direction of compass
     if(heading < 0.0 || heading > 360.0){
        Serial.print("Error. Heading should be between 0 and 360 degrees. Your heading is: ");
        Serial.print(heading);
        Serial.println(" degrees");
     } else {
        // Print message to serial if compass is pointing exactly or almost exactly in an particular direction
        if((abs(heading-0.0) < 0.5) || (abs(heading-360.0) < 0.5)){
            Serial.println("Facing due north");
            direction_str = "north";
        } else if (abs(heading-45.0) < 0.5){
            Serial.println("Facing due northeast");
            direction_str = "northeast";
        } else if (abs(heading-90.0) < 0.5){
            Serial.println("Facing due east");
            direction_str = "east";
        } else if (abs(heading-135.0) < 0.5){
            Serial.println("Facing due southeast");
            direction_str = "southeast";
        } else if (abs(heading-180.0) < 0.5){
            Serial.println("Facing due south");
            direction_str = "south";
        } else if (abs(heading-225.0) < 0.5){
            Serial.println("Facing due southwest");
            direction_str = "southwest";
        } else if (abs(heading-270.0) < 0.5){
            Serial.println("Facing due west");
            direction_str = "west";
        } else if (abs(heading-315.0) < 0.5){
            Serial.println("Facing due northwest");
            direction_str = "northwest";
        } else if (heading <= 22.5 || heading > 337.5){
            direction_str = "north";
        } else if (heading <= 67.5 && heading > 22.5){
            direction_str = "northeast";
        } else if (heading <= 112.5 && heading > 67.5){   
            direction_str = "east";
        } else if (heading <= 157.5 && heading > 112.5){  
            direction_str = "southeast";
        } else if (heading <= 202.5 && heading > 157.5){    
            direction_str = "south";
        } else if (heading <= 247.5 && heading > 202.5){   
            direction_str = "southwest";
        } else if (heading <= 292.5 && heading > 247.5){    
            direction_str = "west";
        } else if (heading <= 337.5 && heading > 292.5){   
            direction_str = "northwest";
        }
     }

    // Print out direction and heading of compass if the heading value is valid
    if(direction_str != "Error"){
        Serial.print("Direction: ");
        Serial.println(direction_str);
        Serial.print("Compass Heading: ");
        Serial.print(heading,5);
        Serial.println(" degrees");
    }
    // Delay blink of LED based on value of heading. LED will blink faster when closer to magnetic North
    if(heading<=180.0){
        digitalWrite(LED, HIGH);
        delay(50 + (heading*4)); 
        digitalWrite(LED, LOW);
    } else {
        digitalWrite(LED, HIGH);
        delay(50 + ((360-heading)*4)); 
        digitalWrite(LED, LOW);
    }

    delay(1000);

};