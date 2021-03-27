// GLEE 2023 TMP117 Library

#include "TMP117.h"

/**
 * Constructor
 * Parameters: the sensor ID as an integer and the debug mode as a booelan
 * Returns: none
 * This is the constructor for the TMP117 class. 
**/
TMP117::TMP117(int _id, bool _debug){
    TMP117::info.id = _id;
    TMP117::info.name = "TMP117 Temperature Sensor";
  	TMP117::info.address = 0x48;
    TMP117::sensorDebug = _debug;
    Wire.begin(TMP117::info.address);     // Might need to move this out of the initializer
}

/**
 * Parameters: none
 * Returns: TMP117 address
 * This function returns the TMP117 I2C address.
**/
uint8_t TMP117::getAddress(){
    return TMP117::info.address;
}

/**
 * Parameters: none
 * Returns: double final temperature in celcius 
 * This function pulls the bits to store in an array
 * then multiplies by resolution for digital to final temp. 
**/
double TMP117::getTemperatureC(){
    int16_t digitalTempC = read2Byte(TMP117_TEMP_REG);      // Calls to read registers to pull all the bits to store in an array
    double finalTempC = digitalTempC * TMP117_RESOLUTION;    // Multiplies by the resolution for digital to final temp
    return finalTempC;
}

/**
 * Parameters: none
 * Returns: double Fahrenheit temperature 
 * This function takes the celcius temperature and 
 * converts to fahrenheit.
**/
double TMP117::getTemperatureF(){
    return getTemperatureC() * 9.0 / 5.0 + 32.0;
}

/**
 * Parameters: inputTemperature
 * Returns: uncertainy
 * This function takes an input temperature then calculates 
 * the uncertainty. 
**/
float TMP117::getUncertainty(float inputTemperature){
    double temp = inputTemperature;
    if(temp >= -20 &&  temp <= 50){
        return 0.1;
    }
    else if(temp >= -40 &&  temp <= 70){
        return 0.15;
    }
    else if(temp >= -55 &&  temp <= 100){
        return 0.2;
    }
    else if(temp >= -55 &&  temp <= 125){
        return 0.25;
    }
    else if(temp >= -55 &&  temp <= 150){
        return 0.3;
    }
}

/**
 * Parameters: float low limit
 * Returns: none
 * This function sets the low limit.
**/
void TMP117::setLowLimit(float lowLimit){
    int16_t lLimit = lowLimit/TMP117_RESOLUTION;
    write2Byte(TMP117_LOW_LIMIT_REG, lLimit);
}

/**
 * Parameters: none
 * Returns: float low limit
 * This function calls all the bits to store in an array 
 * then multiplies the low limit by the resolution to return 
 * the low limit.
**/
float TMP117::getLowLimit(){
    int16_t LowLimit = read2Byte(TMP117_LOW_LIMIT_REG);
    float limit = (float)LowLimit * TMP117_RESOLUTION;
    return limit;
}

/**
 * Parameters: float high limit
 * Returns: none
 * This function sets the high limit
**/
void TMP117::setHighLimit(float highLimit){
    int16_t hLimit = highLimit/TMP117_RESOLUTION;
    write2Byte(TMP117_HIGH_LIMIT_REG, hLimit);
}

/**
 * Parameters: none
 * Returns: float high limit
 * This function calls all the bits to store in an array
 * then multiplies the low limit by the resolution to 
 * return the high limit.
**/
float TMP117::getHighLimit(){
    int16_t HighLimit = read2Byte(TMP117_HIGH_LIMIT_REG);
    float limit = (float)HighLimit * TMP117_RESOLUTION;
    return limit;
}

/**
 * Parameters: none
 * Returns: boolean true or false
 * This function passes the high alert register
 * after reading and returning true or false.
**/
bool TMP117::getHighAlert(){
    uint16_t config = read2Byte(TMP117_CONFIG_REG);
    uint8_t configBit = bitRead(config, 15);
    if(configBit == 1){
        return true;
    }
    return false;
}

/**
 * Parameters: none
 * Returns: boolean true or false
 * This function passes low alert register
 * after reading and returning true or false.
**/
bool TMP117::getLowAlert(){
    uint16_t config = read2Byte(TMP117_CONFIG_REG);
    uint8_t configBit = bitRead(config, 14);
    if(configBit == 1){
        return true;
    }
    return false;
}

/**
 * Parameters: none
 * Returns: none
 * This function resets the bits previously stored.
**/
void TMP117::reset(){
    uint16_t defaultConfig = 0;
    defaultConfig |= 1UL << 1; //0000000000000010
    write2Byte(TMP117_CONFIG_REG, defaultConfig);
}

/**
 * Parameters: none
 * Returns: boolen true or false
 * This function returns true or false on whether
 * the data is ready by looking at the 
 * configuration address.
**/
bool TMP117::dataReady(){
    uint16_t config = read2Byte(TMP117_CONFIG_REG);
    if(config & 1 << 13){
        return true;
    }
    return false;
}

/**
 * Parameters: registerAddress
 * Returns: 2 bytes 
 * This function returns the I2C address
 * of the temperature sensor.
**/
uint16_t TMP117::read2Byte(uint8_t registerAddress){
    uint16_t readByte;              // byte to store data that is read
    uint8_t data[2] = {0};			// Declares an array of length 2 to be empty
  	int16_t datac = 0;				// Declares the return variable to be 0
    
    Wire.beginTransmission(TMP117::info.address);           //begins comms with sensor specified
    Wire.write(registerAddress);                            //identifies register for data to be read from
    Wire.endTransmission();                                 //end transmission
    Wire.requestFrom(TMP117::info.address, uint8_t(2));     //request 2 bytes from the sensor address
  	
    if (Wire.available() <= 2){              // Don't read more than 2 bits
  		data[0] = Wire.read();			          // Reads the first set of bits (D15-D8)
  		data[1] = Wire.read();			         // Reads the second set of bits (D7-D0)
  		datac = ((data[0] << 8) | data[1]); // Swap the LSB and the MSB
  	}
  	return datac;   //return the read data byte
}

/**
 * Parameters: uint8 register address and uint16_t data
 * Returns: none
 * This function sets bits of data at 8 bit
 * register address.
**/
void TMP117::write2Byte(uint8_t reg, uint16_t data){    
	Wire.beginTransmission(TMP117::info.address);      
	Wire.write(reg);
	Wire.write(highByte(data));   // Write MSB (D15-D8)
	Wire.write(lowByte(data));   // Write LSB (D7-D0)
	Wire.endTransmission();	    // Stop transmitting data
}