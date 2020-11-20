#include <Arduino.h>
#include <Wire.h>

float resolution;
uint8_t registerAddress;
uint8_t i2cAddress;

uint16_t readByte;          // byte to store data that is read
uint8_t data[2];			// Declares an array of length 2 to be empty
int16_t datac;

int digitalTempC;
float finalTempC;

float startTime;
float endTime;

void setup(){
    // Initialize sensor
    // Initialize sample rate
    resolution = 0.0078125;
    registerAddress = 0x00;
    i2cAddress = 0x49;

    data[2] = {0};			// Declares an array of length 2 to be empty
    datac = 0;				// Declares the return variable to be 0
    Serial.begin(9600);
}

void loop(){
    // Take and record sample every second
    startTime = micros();  // Record start time for letter offseting of delay to make the loop run once per second

    data[2] = {0};			// Reset data variables at the start of every loop
    datac = 0;				

    Wire.beginTransmission(i2cAddress);                
    Wire.write(registerAddress);                         
    Wire.endTransmission();                                 
    Wire.requestFrom(i2cAddress, uint8_t(2) );      //request 2 bytes from the sensor address

    if (Wire.available() <= 2){                     // Don't read more than 2 bits
        data[0] = Wire.read();			                // Reads the first set of bits (D15-D8)
        data[1] = Wire.read();			                // Reads the second set of bits (D7-D0)
        datac = ((data[0] << 8) | data[1]);         // Swap the LSB and the MSB
    }

    digitalTempC = datac; 
    finalTempC = digitalTempC * resolution;         // Multiplies by the resolution for digital to final temp

    Serial.println(finalTempC);
    endTime = micros();

    delay(1000 - (endTime - startTime));
}
