#include "MPU6000.h"

bool ICM_DEBUG = false;

// Scale resolutions per LSB for the sensors
float aRes, gRes; 
//int Ascale = AFS_2G;
//int Gscale = GFS_1000DPS;

/*
Constructor
Parameters: sensor ID as an INT, gyroscope enabled as a BOOL, debug mode as a BOOL
Returns: none
This is the constructor for this sensor that sets the
basic variables to get started.
*/
MPU6000::MPU6000(int _id, bool _debug){
	MPU6000::info.id = _id;
    MPU6000::info.address = MPU6000_I2CADDR_DEFAULT;
	MPU6000::info.name = "MPU6000 Inertial Measurement Unit";
  	MPU6000::sensorDebug = _debug;
    MPU6000::accel_range = MPU6000_RANGE_2_G;
    MPU6000::gyro_range = MPU6000_RANGE_250_DEG;
    MPU6000::accel_scale = 16384.0; 
    MPU6000::gyro_scale = 131.0;
	//MPU6000::gyroOn = _gyroOn;
}

/*
Parameters: none
Returns: whether communication with sensor was successful as a BOOL
This function begins a transmission to the I2C slave 
device with the given address and notifies the user
that communication with the sensor has begun.
*/
bool MPU6000::begin(void){
    Wire.begin();
    if(MPU6000::sensorDebug){
		Serial.println("Beginning Communication With MPU6000");
	}
    return true;
}

/*
Parameters: none
Returns: none
This function sets the clock to auto, disables or enables the gyroscope, and sets a
couple of the other registers.
*/
void MPU6000::initialize(void){
    Wire.setClock(100000);



    // writeByte(MPU6000_PWR_MGMT_1, 0x01);   // Set clock set to auto
    // writeByte(MPU6000_CONFIG, 0x01); 
    // writeByte(MPU6000_GYRO_CONFIG, 0x10);    
    // writeByte(MPU6000_ACCEL_CONFIG, 0x00);   
	// if(gyroOn){
	// 	disableGyro(false);    // Configuration with Gyro enabled 
	// }else{
	// 	disableGyro(true);  // Configuration with Gyro disabled 
	// }
  // TODO: Dynamically initialize accel config with respect to sensitivity mode provided at initialization
}


/*
Parameters: none
Returns: The raw acceleration in LSB/G as a struct of sensor_uint16_vec_t type
This function reads in the high bytes of the accel for each of the three 
axis, performs a bitwise operation, then saves and returns the raw acceleration
struct.
*/
sensor_int16_vec_t MPU6000::getRawAcc(){
	sensor_int16_vec_t rawAcc;
	uint8_t buffer[6];

	// Read 6 bytes at accel_out register
	readBytes(MPU6000_ACCEL_OUT, 6, &buffer[0]);
	
	// 2xBytes per raw axis reading
	rawAcc.x = buffer[0] << 8 | buffer[1];
	rawAcc.x = buffer[2] << 8 | buffer[3];
	rawAcc.x = buffer[4] << 8 | buffer[5];

    return rawAcc;
}

sensor_int16_vec_t MPU6000::getRawGyro(){
	sensor_int16_vec_t rawGyro;
	uint8_t buffer[6];

	// Read 6 bytes at accel_out register
	readBytes(MPU6000_GYRO_OUT, 6, &buffer[0]);
	
	// 2xBytes per raw axis reading
	rawGyro.x = buffer[0] << 8 | buffer[1];
	rawGyro.x = buffer[2] << 8 | buffer[3];
	rawGyro.x = buffer[4] << 8 | buffer[5];

    return rawGyro;
}

/*
	Parameters: the raw acceleration as a struct of sensor_int16_vec_t type
	Returns: the accelerations in G's as a sensor_float_vec_t type
	This function converts the raw acceleration in LSB/G to the acceleration in 
	G's by dividing the sensitivity factor based on the current sensitivity scale.
*/
sensor_float_vec_t MPU6000::getAcc(sensor_int16_vec_t rawAcc){
	sensor_float_vec_t acc;
	acc.x = (float) rawAcc.x / accel_scale;
    acc.y = (float) rawAcc.y / accel_scale;
    acc.z = (float) rawAcc.z / accel_scale;
	return acc;
}

/*
	Parameters: the raw angular velocity as a struct of sensor_int16_vec_t type
	Returns: the angular velocity in degrees per second as a sensor_float_vec_t type 
	This function converts the raw gyroscope reading in LSB/DPS to the angular
	velocity in DPS by diving the sensitivity factor based on the current gyroscope
	sensitivity scale.
*/
sensor_float_vec_t MPU6000::getGyro(sensor_int16_vec_t rawGyro){
	sensor_float_vec_t gyro;
    gyro.x = (float) rawGyro.x / gyro_scale;
    gyro.y = (float) rawGyro.y / gyro_scale;
    gyro.z = (float) rawGyro.z / gyro_scale;
	return gyro;
}

/*
	Parameters: none
	Returns: 3 axis acceleration in G's
*/
sensor_float_vec_t MPU6000::getSample(){
	sensor_int16_vec_t rawAcc = getRawAcc();
	sensor_float_vec_t acc = getAcc(rawAcc); 
	return(acc);
}

/*
	Parameters: none
	Returns: 3 axis acceleration in G's
*/
sensor_float_vec_t MPU6000::getGyroSample(){
	sensor_int16_vec_t rawGyro = getRawGyro();
	sensor_float_vec_t gyro = getGyro(rawGyro); 
	return(gyro);
}


/*
Parameters: new scale of the sensor wished to be set to as an Ascale enumeration
Returns: none
This function allows a new scale to be passed in, with the global variable 
current scale set to the new scale, and writing the accelration configuration
based on the new scale.
*/
void MPU6000::setAccelRange(mpu6000_accel_range_t new_range){
    accel_range = new_range;

	writeBits(MPU6000_ACCEL_CONFIG, MPU6000_CONFIG_FS_SEL_BIT, MPU6000_CONFIG_FS_SEL_LEN, new_range);

	if (accel_range == MPU6000_RANGE_2_G) accel_scale = 16384;
	if (accel_range == MPU6000_RANGE_4_G) accel_scale = 8192;
	if (accel_range == MPU6000_RANGE_8_G) accel_scale = 4096;
	if (accel_range == MPU6000_RANGE_16_G) accel_scale = 2048;
}

/*
Parameters: new scale of the sensor wished to be set to as an Gscale enumeration
Returns: none
This function allows a new scale to be passed in with the global variable 
current gyroscope scale set to the new scale and writing the gyroscope configuration
based on the new scale.
*/
void MPU6000::setGyroRange(mpu6000_gyro_range_t new_range){
    gyro_range = new_range;

	writeBits(MPU6000_GYRO_CONFIG, MPU6000_CONFIG_FS_SEL_BIT, MPU6000_CONFIG_FS_SEL_LEN, new_range);

	if (gyro_range == MPU6000_RANGE_250_DEG) gyro_scale = 131;
	if (gyro_range == MPU6000_RANGE_500_DEG) gyro_scale = 65.5;
	if (gyro_range == MPU6000_RANGE_1000_DEG) gyro_scale = 32.8;
	if (gyro_range == MPU6000_RANGE_2000_DEG) gyro_scale = 16.4;
}

/*
Parameters: to disable or enable the gyroscope as a boolean
Returns: none
This function writes the configuration of the sensor depending on whether the 
gyroscope is to be enabled or disabled.
*/


// int16_t MPU6000::read2Byte(uint8_t registerAddress){
//     uint8_t data[2] = {0};			// Declares an array of length 2 to be empty
//   	int16_t datac = 0;				// Declares the return variable to be 0
    
//     Wire.beginTransmission(MPU6000::info.address);          //begins comms with sensor specified
//     Wire.write(registerAddress);                            //identifies register for data to be read from
//     Wire.endTransmission();                                 //end transmission
//     Wire.requestFrom(MPU6000::info.address, uint8_t(2) );   //request 2 bytes from the sensor address
  	
//     if (Wire.available() <= 2){             // Don't read more than 2 bits
//   		data[0] = Wire.read();			    // Reads the first set of bits (D15-D8)
//   		data[1] = Wire.read();			    // Reads the second set of bits (D7-D0)
//   		datac = ((data[0] << 8) | data[1]); // Swap the LSB and the MSB
//   	}
//   	return datac;   //return the read data byte
// }




/*
Parameters: the current raw acceleration to be saved as a struct of sensor_int16_vec_t type
Returns: none
This function saves the the raw acceleration given by the argument into
the private type sensor_int16_vec_t accelRaw struct. 
*/
// void MPU6000::updateRawAccel(sensor_int16_vec_t rawAccel){
// 	MPU6000::accelRaw = rawAccel;
// }

/*
Parameters: the accleration in G's as a struct of sensor_float_vec_t type
Returns: The acceleration in m/s^2 as a struct of sensor_float_vec_t type
This function converts the raw accelerations in LSB/G to 
meters per second squared.
*/
// sensor_float_vec_t MPU6000::getMPSAccel(sensor_float_vec_t GAccel){
// 	sensor_float_vec_t accelMPS;
// 	accelMPS.x = GAccel.x * IMU_ONE_G;
// 	accelMPS.y = GAccel.y * IMU_ONE_G;
// 	accelMPS.z = GAccel.z * IMU_ONE_G;
// 	return accelMPS;    
// }

/*
Parameters: the current accleration in meters per second to be saved as a struct of sensor_float_vec_t type 
Returns: none
This function saves the acceleration in meters per second given by the argument into
the private type sensor_float_vec_t accelMPS struct.
*/
// void MPU6000::updateMPSAccel(sensor_float_vec_t MPSAccel){
// 	MPU6000::accelMPS = MPSAccel;
// }

/*
Parameters: current scale of the sensor as the Ascale enumeration
Returns: the accelerations in 
debug mode set to true, if true data interfacing
classes and everything working and responds to output for datalog
*/
// sensor_float_vec_t MPU6000::getGAccel_fuzzed(){
//     MPU6000::accelG.x = currentAccelFactor*1;
//     MPU6000::accelG.y = currentAccelFactor*2;
//     MPU6000::accelG.z = currentAccelFactor*3;
// 	return MPU6000::accelG;
// }

/*
Parameters: the current accleration in G's to be saved as a struct of sensor_float_vec_t type
Returns: none
This function saves the acceleration in G's given by the argument into 
the private type sensor_float_vec_t accelG struct
*/
// void MPU6000::updateGAccel(sensor_float_vec_t GAccel){
// 	MPU6000::accelG = GAccel;
// }

/*
Parameters: none
Returns: The raw angular velocity in LSB/DPS as a struct of sensor_uint16_vec_t type
This function reads in the high bytes of the angular velocity for each of the three 
axis, performs a bitwise operation, then saves and returns the raw angular velocity
struct.
*/
// sensor_int16_vec_t MPU6000::getRawAngVel(){
// 	sensor_int16_vec_t RawAngVel;
// 	RawAngVel.x = read2Byte(MPU6000_GYRO_XOUT_H);
// 	RawAngVel.y = read2Byte(MPU6000_GYRO_YOUT_H);
// 	RawAngVel.z = read2Byte(MPU6000_GYRO_ZOUT_H);
// 	return RawAngVel;
// }



/*
Parameters: the current raw angular velocity to be saved as a struct of sensor_int16_vec_t type
Returns: none
This function saves the raw angular velocity given by the argument into 
the private type sensor_int16_vec_t angVelRaw struct.
*/
// void MPU6000::updateRawAngVel(sensor_int16_vec_t rawAngVel){
// 	MPU6000::angVelRaw = rawAngVel;
// }



/*
Parameters: the current angular velocity in degrees per second to be saved as a struct of sensor_float_vec_t type
Returns: none
This function saves the angular velocity in degrees per second given by the argument into
the private type sensor_float_vec_t angVelDPS struct.
*/
// void MPU6000::updateDPSAngVel(sensor_float_vec_t DPSAngVel){
// 	MPU6000::angVelDPS = DPSAngVel;
// }



/*
Parameters: none
Returns: the accelerometer sensitivity scale factor as a FLOAT 
This function uses a switch statement to return the sensitivity scale factor
for the acceleration depending on the current sensing accuracy scale.
*/
// float MPU6000::getAccelSensitivity(){
//   // TODO: Write setter for sensity a variable, possibly private
//   	float factor;
// 	switch (currentAccelScale) {
// 		case (AFS_2G):
//       		factor = 16384.0;
//       		break;
//     	case (AFS_4G):
//       		factor = 8192.0;
//       		break;
//     	case (AFS_8G):
//      		factor = 4096.0;
//       		break;
//     	case (AFS_16G):
//       		factor = 2048.0;
//       		break;
// 	}
// 	currentAccelFactor = factor;
// 	return factor;  
// }



/*
Parameters: none
Returns: the gyroscope sensitivity scale factor as a FLOAT 
This function uses a switch statement to return the sensitivity scale factor
for the gyroscope depending on the current sensing accuracy scale.
*/
// float MPU6000::getGyroSensitivity(){
//     float factor;
//     switch (currentGyroScale) {
//     	case (GFS_250DPS):
//       		factor = 131.0;
//       		break;
//     	case (GFS_500DPS):
//       		factor = 65.5;
//       		break;
//     	case (GFS_1000DPS):
//       		factor = 32.8;
//       		break;
//     	case (GFS_2000DPS):
//       		factor = 16.4;
//       		break;
// 	}
// 	currentGyroFactor = factor;
// 	return factor;  
// }

