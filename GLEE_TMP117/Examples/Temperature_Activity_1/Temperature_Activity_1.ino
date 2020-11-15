// Include libraries
// This sketch is only dependent on Arduino and Wire librarys
#include <Arduino.h>
#include <Wire.h>

// Sensor configuration variables
float resolution;           // Resolution for conversion from binary representation
uint8_t registerAddress;    // Address of register on sensor containing representation of temperature
uint8_t i2cAddress;         // I2C address of sensor 

// Data processing variables
uint16_t readByte;          // byte to store data that is read
uint8_t data[2];			// Declares an array of length 2 to be empty
int16_t datac;

int digitalTempC;           // Digital value representing temperature from sensor
float finalTempC;           // Processed value representing temerature in degrees C (base 10)

// Timing Variables
float startTime;
float endTime;

void setup(){
    resolution = 0.0078125;     // Set resolution as defined by datasheet
    registerAddress = 0x00;     // Set register address for temperature value
    i2cAddress = 0x49;          // Set I2C address of temperature sensor

    data[2] = {0};			    // Initialize empty array for reading 2 bits of data over I2C
    digitalTemp = 0;    		// Initialize variable to store digital temperature after reading bits
    
    Serial.begin(9600);         // Begin serial communication with computer or datalogger
}

void loop(){
    // Take and record sample every second
    startTime = micros();  // Record start time for letter offseting of delay to make the loop run once per second

    data[2] = {0};			// Reset data variables at the start of every loop				

    Wire.beginTransmission(i2cAddress);             // Begin transmission with temperature sensor                
    Wire.write(registerAddress);                    // Tell the sensor we want to read the temperature value
    Wire.endTransmission();                         // End preamble transmission
    Wire.requestFrom(i2cAddress, uint8_t(2) );      // Request 2 unsigned 8 bit integers from sensorytes from the sensor address

    if (Wire.available() <= 2){                     // Wait until 2 8-Bit 
        data[0] = Wire.read();			            // Read the first 8 bit integer (D15-D8)
        data[1] = Wire.read();			            // Read the second 8 bit integer (D7-D0)
        digitalTemp = ((data[0] << 8) | data[1]);   // Swap order of 8 bit integers and concatnate to form digital temperature representation
    }
 
    finalTempC = digitalTempC * resolution;         // Final temperature calculated by multiplying digital representation by the sensor resolution

    Serial.println(finalTempC);     // Print the processed temperature in degrees C via serial (recorder to be later stored in file or displayed in serial monitor
    
    endTime = micros();     // Record the end time at which temperature recording is complete

    delay(1000 - (endTime - startTime));    // Offset delay to insure 1s intervals between observations *Note delay in milliseconds (1000 => 1s)
}
