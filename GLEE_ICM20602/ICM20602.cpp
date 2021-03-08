#include <Arduino.h>
#include "ICM20602.h"
#include <Wire.h>

bool ICM_DEBUG = false;

// Scale resolutions per LSB for the sensors
float aRes, gRes; 
//int Ascale = AFS_2G;
int Gscale = GFS_1000DPS;

/*
Constructor
Parameters: sensor ID as an INT, debug mode as a BOOL
Returns: none
This is the constructor for this sensor that sets the
basic variables to get started.
*/
ICM20602::ICM20602(int _id, bool _debug){
    ICM20602::info.name = "ICM20602 Inertial Measurement Unit";
    ICM20602::info.address = ICM20602_SLAVE_ADDR;
    ICM20602::sensorDebug = _debug;
    ICM20602::currentScale = AFS_2G;
    ICM20602::currentFactor = 16384.0;
}

/*
Parameters: none
Returns: whether communication with sensor was successful as a BOOL
This function begins a transmission to the I2C slave 
device with the given address and notifies the user
that communication with the sensor has begun.
*/
bool ICM20602::begin(void){
    Wire.begin();
    Serial.println("Beginning Communication With ICM20602");
    return true;
}

/*
Parameters: none
Returns: none
This function sets the clock to auto, disables the gyroscope, and sets a
couple of the other registers.
*/
void ICM20602::initialize(void){
    Wire.setClock(100000);
    writeByte(ICM20602_PWR_MGMT_1, 0x01);   // Set clock set to auto
    writeByte(ICM20602_PWR_MGMT_2,0x07);    // Disable gyro
    writeByte(ICM20602_CONFIG, 0x01); 
    writeByte(ICM20602_GYRO_CONFIG, 0x00);    
    writeByte(ICM20602_ACCEL_CONFIG, 0x00);   
    // TODO: 0x10 - 8G sensitivity mode? 
    // 0x00 - 2G Sensitivity mode?
    // TODO: Dynamically initialize accel config with respect to sensitivity mode provided at initialization
}

/*
Parameters: Register address as an unsigned byte
Returns: The data byte being read
This function requests 2 bytes from the sensor register address and returns an unsigned integer that represents the read data bytes
*/
int16_t ICM20602::read2Byte(uint8_t registerAddress){
    uint8_t data[2] = {0};			// Declares an array of length 2 to be empty
  	int16_t datac = 0;				// Declares the return variable to be 0
    
    Wire.beginTransmission(ICM20602::info.address);          //begins comms with sensor specified
    Wire.write(registerAddress);                            //identifies register for data to be read from
    Wire.endTransmission();                                 //end transmission
    Wire.requestFrom(ICM20602::info.address, uint8_t(2) );   //request 2 bytes from the sensor address
  	
    if (Wire.available() <= 2){             // Don't read more than 2 bits
  		data[0] = Wire.read();			    // Reads the first set of bits (D15-D8)
  		data[1] = Wire.read();			    // Reads the second set of bits (D7-D0)
  		datac = ((data[0] << 8) | data[1]); // Swap the LSB and the MSB
  	}
  	return datac;   //return the read data byte
}

/*
Parameters: none
Returns: The raw acceleration in LSB/G as a struct of sensor_uint16_vec_t type
This function reads in the high and low bytes of the accel for each of the three 
axis, performs a bitwise operation, then saves and returns the raw acceleration
struct.
*/
sensor_int16_vec_t ICM20602::getRawAccel(){
    ICM20602::accelRaw.x = read2Byte(ICM20602_ACCEL_XOUT_H);
    ICM20602::accelRaw.y = read2Byte(ICM20602_ACCEL_YOUT_H);
    ICM20602::accelRaw.z = read2Byte(ICM20602_ACCEL_ZOUT_H);

    return ICM20602::accelRaw;
}

/*
Parameters: none
Returns: The acceleration in m/s^2 as a struct of sensor_float_vec_t type
This function converts the raw accelerations in LSB/G to 
meters per second squared.
*/
sensor_float_vec_t ICM20602::getMPSAccel(){
    // TODO: Apply sensitivity factor, currently hard coded
    ICM20602::accelMPS.x = ICM20602::accelG.x * IMU_ONE_G;
    ICM20602::accelMPS.y = ICM20602::accelG.y * IMU_ONE_G;
    ICM20602::accelMPS.z = ICM20602::accelG.z * IMU_ONE_G;
    return ICM20602::accelMPS;    
}
//testing collaberative work

/*
Parameters: current scale of the sensor as the Ascale enumeration
Returns: the sensitivity scale factor as a FLOAT 
This function uses a switch statement to return the sensitivity scale factor
depending on the current sensing accuracy scale.
*/
float ICM20602::getSensitivity(){
    // TODO: Write setter for sensity a variable, possibly private
    // TODO: Set sensitivity on initialization
    float factor;
    switch (currentScale) {
        case (AFS_2G):
            factor = 16384.0;
            break;
        case (AFS_4G):
            factor = 8192.0;
            break;
        case (AFS_8G):
            factor = 4096.0;
            break;
        case (AFS_16G):
            factor = 2048.0;
            break;
    }
    currentFactor = factor;
    return factor;  
}

/*
Parameters: current scale of the sensor as the Ascale enumeration
Returns: the accelerations in G's as a sensor_float_vec_t type
This function converts the raw acceleration in LSB/G to the acceleration in 
G's by dividing the sensitivity factor based on the current sensitivity scale.
*/
sensor_float_vec_t ICM20602::getGAccel(){
    ICM20602::accelG.x = (float) ICM20602::accelRaw.x/ currentFactor;
    ICM20602::accelG.y = (float) ICM20602::accelRaw.y/ currentFactor;
    ICM20602::accelG.z = (float) ICM20602::accelRaw.z/ currentFactor;
    return ICM20602::accelG;
}

/*
Parameters: current scale of the sensor as the Ascale enumeration
Returns: the accelerations in 
debug mode set to true, if true data interfacing
classes and everything working and responds to output for datalog

*/
sensor_float_vec_t ICM20602::getGAccel_fuzzed(){
    ICM20602::accelG.x = currentFactor*1;
    ICM20602::accelG.y = currentFactor*2;
    ICM20602::accelG.z = currentFactor*3;
    return ICM20602::accelG;
}


/*
Parameters: new scale of the sensor wished to be set to as an Ascale enumeration
Returns: none
This function allows a new scale to be passed in, with the global variable 
current scale set to the new scale, and writing the accelration configuration
based on the new scale.
*/
void ICM20602::setScale(enum Ascale newScale){
    currentScale = newScale;
    switch (currentScale) {
        case (AFS_2G):
            writeByte(ICM20602_ACCEL_CONFIG, 0x00);
            break;
        case (AFS_4G):
            writeByte(ICM20602_ACCEL_CONFIG, 0x01);
            break;
        case (AFS_8G):
            writeByte(ICM20602_ACCEL_CONFIG, 0x10);
            break;
        case (AFS_16G):
            writeByte(ICM20602_ACCEL_CONFIG, 0x11);
            break;
    }
    getSensitivity();
}

