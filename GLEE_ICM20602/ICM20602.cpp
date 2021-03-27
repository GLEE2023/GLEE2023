#include <Arduino.h>
#include "ICM20602.h"
#include <Wire.h>

bool ICM_DEBUG = false;

// Scale resolutions per LSB for the sensors
float aRes, gRes; 
//int Ascale = AFS_2G;
//int Gscale = GFS_1000DPS;

/*
Constructor
Parameters: sensor ID as an INT, debug mode as a BOOL
Returns: none
This is the constructor for this sensor that sets the
basic variables to get started.
*/
ICM20602::ICM20602(int _id, bool _debug){
	ICM20602::info.id = _id;
    ICM20602::info.name = "ICM20602 Inertial Measurement Unit";
  	ICM20602::info.address = ICM20602_SLAVE_ADDR;
  	ICM20602::sensorDebug = _debug;
    ICM20602::currentScale = AFS_2G;
    ICM20602::currentGyroScale = GFS_1000DPS;
    ICM20602::currentFactor = 16384.0;
    ICM20602::currentGyroFactor = 131.0;
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
  	// TODO: Dynamically initialize accel config with respect to sensitivity mode provided at initialization
}

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
Returns: The raw angular velocity in LSB/DPS as a struct of sensor_uint16_vec_t type
This function reads in the high bytes of the angular velocity for each of the three 
axis, performs a bitwise operation, then saves and returns the raw angular velocity
struct.
*/
sensor_int16_vec_t ICM20602::getRawAngVel(){
	ICM20602::angVelRaw.x = read2Byte(ICM20602_GYRO_XOUT_H);
	ICM20602::angVelRaw.y = read2Byte(ICM20602_GYRO_YOUT_H);
	ICM20602::angVelRaw.z = read2Byte(ICM20602_GYRO_ZOUT_H);
	return ICM20602::angVelRaw;
}

/*
Parameters: none
Returns: The raw acceleration in LSB/G as a struct of sensor_uint16_vec_t type
This function reads in the high bytes of the accel for each of the three 
axis, performs a bitwise operation, then saves and returns the raw acceleration
struct.
*/
sensor_int16_vec_t ICM20602::getRawAccel(){
	sensor_int16_vec_t RawAccel;
	RawAccel.x = read2Byte(ICM20602_ACCEL_XOUT_H);
	RawAccel.y = read2Byte(ICM20602_ACCEL_YOUT_H);
	RawAccel.z = read2Byte(ICM20602_ACCEL_ZOUT_H);
    return RawAccel;
}

/*
Parameters: none
Returns: The acceleration in m/s^2 as a struct of sensor_float_vec_t type
This function converts the raw accelerations in LSB/G to 
meters per second squared.
*/
sensor_float_vec_t ICM20602::getMPSAccel(sensor_float_vec_t GAccel){
	sensor_float_vec_t accelMPS;
	accelMPS.x = GAccel.x * IMU_ONE_G;
	accelMPS.y = GAccel.y * IMU_ONE_G;
	accelMPS.z = GAccel.z * IMU_ONE_G;
	return accelMPS;    
}
//testing collaberative work

/*
Parameters: current scale of the sensor as the Ascale enumeration
Returns: the accelerations in G's as a sensor_float_vec_t type
This function converts the raw acceleration in LSB/G to the acceleration in 
G's by dividing the sensitivity factor based on the current sensitivity scale.
*/
sensor_float_vec_t ICM20602::getGAccel(sensor_int16_vec_t rawAccel){
	sensor_float_vec_t accelG;
	accelG.x = (float) rawAccel.x / ICM20602::currentFactor;
    accelG.y = (float) rawAccel.y / ICM20602::currentFactor;
    accelG.z = (float) rawAccel.z / ICM20602::currentFactor;
	return accelG;
}

void ICM20602::updateRawAccel(sensor_int16_vec_t rawAccel){
	ICM20602::accelRaw = rawAccel;
}

void ICM20602::updateMPSAccel(sensor_float_vec_t MPSAccel){
	ICM20602::accelMPS = MPSAccel;
}

void ICM20602::updateGAccel(sensor_float_vec_t GAccel){
	ICM20602::accelG = GAccel;
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
Parameters: none
Returns: the angular velocity in degrees per second as a sensor_float_vec_t type 
This function converts the raw gyroscope reading in LSB/DPS to the angular
velocity in DPS by diving the sensitivity factor based on the current gyroscope
sensitivity scale.
*/
sensor_float_vec_t ICM20602::getDPSAngVel(){
    ICM20602::angVelDPS.x = (float) ICM20602::angVelRaw.x/ currentGyroFactor;
    ICM20602::angVelDPS.y = (float) ICM20602::angVelRaw.y/ currentGyroFactor;
    ICM20602::angVelDPS.z = (float) ICM20602::angVelRaw.z/ currentGyroFactor;
	return ICM20602::angVelDPS;
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

/*
Parameters: new scale of the sensor wished to be set to as an Gscale enumeration
Returns: none
This function allows a new scale to be passed in, with the global variable 
current gyroscope scale set to the new scale, and writing the gyroscope configuration
based on the new scale.
*/
void ICM20602::setGyroScale(enum Gscale newScale){
    currentGyroScale = newScale;
    switch (currentGyroScale) {
        case (GFS_250DPS):
            writeByte(ICM20602_GYRO_CONFIG, 0x00);
            break;
        case (GFS_500DPS):
            writeByte(ICM20602_GYRO_CONFIG, 0x01);
            break;
        case (GFS_1000DPS):
            writeByte(ICM20602_GYRO_CONFIG, 0x10);
            break;
        case (GFS_2000DPS):
            writeByte(ICM20602_GYRO_CONFIG, 0x11);
            break;
    }
    getGyroSensitivity();
}

/*
Parameters: current scale of the accelerometer as the Ascale enumeration
Returns: the accelerometer sensitivity scale factor as a FLOAT 
This function uses a switch statement to return the sensitivity scale factor
for the acceleration depending on the current sensing accuracy scale.
*/
float ICM20602::getSensitivity(){
  // TODO: Write setter for sensity a variable, possibly private
	switch (currentScale) {
		case (AFS_2G):
      		ICM20602::currentFactor = 16384.0;
      		break;
    	case (AFS_4G):
      		ICM20602::currentFactor = 8192.0;
      		break;
    	case (AFS_8G):
     		ICM20602::currentFactor = 4096.0;
      		break;
    	case (AFS_16G):
      		ICM20602::currentFactor = 2048.0;
      		break;
	}
	return ICM20602::currentFactor;  
}

/*
Parameters: current scale of the gyroscope as the Gscale enumeration
Returns: the gyroscope sensitivity scale factor as a FLOAT 
This function uses a switch statement to return the sensitivity scale factor
for the gyroscope depending on the current sensing accuracy scale.
*/
float ICM20602::getGyroSensitivity(){
    switch (currentGyroScale) {
    	case (GFS_250DPS):
      		ICM20602::currentGyroFactor = 131.0;
      		break;
    	case (GFS_500DPS):
      		ICM20602::currentGyroFactor = 65.5;
      		break;
    	case (GFS_1000DPS):
      		ICM20602::currentGyroFactor = 32.8;
      		break;
    	case (GFS_2000DPS):
      		ICM20602::currentGyroFactor = 16.4;
      		break;
	}
	return ICM20602::currentGyroFactor;  
}

