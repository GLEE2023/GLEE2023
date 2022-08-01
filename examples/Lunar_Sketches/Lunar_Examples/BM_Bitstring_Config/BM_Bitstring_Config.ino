/*
Bitstring to define config of Magnetometer Sensor
Bits from LSB to MSB:
(bits): description
0-1: mode
    
2-4: Conversion Time
    000
    001
    010
    011
    100
    101
    110
    111
5-6: Averaging
    00: No averaging
    01: 8 Averaged conversions
    10: 32 averaged conversions
    11: 64 averaged conversions
*/

#include <Wire.h>

#define BM_CONFIG_STRING 0b0010100


void writeByte (uint8_t registerAddress, uint8_t writeData);
void readBytes(uint8_t registerAddress, uint8_t nBytes, uint8_t * data);

void setup(){
    Serial.begin(9600);
    Wire.begin();
    delay(1000);

    uint16_t first = 0b0010;
    uint16_t second = ((uint16_t)TMP_CONFIG_STRING);
    uint16_t result = ((0b0010 << 12) | (TMP_CONFIG_STRING << 5)); 

    //Serial.println(result, BIN);
    Wire.beginTransmission(TMP117_TEMP_I2C);
    Wire.write(TMP117_CONFIG_REG);
    Wire.write((uint8_t)(result >> 8));
    Wire.write((uint8_t)result);
    Wire.endTransmission();

    //Serial.println("setup done");
}

void loop(){
    
    uint8_t buffer[2];
    readBytes(TMP117_TEMP_I2C, TMP117_TEMP_REG, 2, &buffer[0]);
    float temp = (float)(buffer[0] << 8 | buffer[1]) * TMP117_RESOLUTION;
    
    Serial.println(String(temp));
    //delay(500);
}


void readBytes(uint8_t I2CsensorAddress, uint8_t registerAddress, uint8_t nBytes, uint8_t * data){
    Wire.beginTransmission(I2CsensorAddress);           // begins forming transmission to sensor
    Wire.write(registerAddress);                     // Add register address to transmission
    Wire.endTransmission();
    Wire.requestFrom(I2CsensorAddress, nBytes);         // Request and listen for response
    // Record response, wire will be available until nBytes are read
    int i = 0;
    while(Wire.available()){
        data[i] = Wire.read();
        i++;
    }
}

/**
 * Parameters: Register Address, Write Data
 * Returns: None
 * This function writes data to specified address
**/
void writeByte (uint8_t I2CsensorAddress, uint8_t registerAddress, uint8_t writeData){
    Wire.beginTransmission(I2CsensorAddress);               // begin communication with the sensor
    Wire.write(registerAddress);                                // point to address to be written to
    Wire.write(writeData);                                      // write data to adress specificed above
    Wire.endTransmission();                                     // end communication
    Serial.println(writeData, BIN);
    Serial.println("Out of writebyte");
}

