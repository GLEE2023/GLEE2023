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
  	ICM20602::info.address = ICM20602_slave_addr;
  	ICM20602::sensorDebug = _debug;
    ICM20602::scaleA = AFS_2G;
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
Parameters: none
Returns: The raw acceleration in LSB/G as a struct of sensor_uint16_vec_t type
This function reads in the high and low bytes of the accerl foreach of the three 
axis, performs a bitwise operation, then saves and returns the raw acceleration
struct.
*/
sensor_uint16_vec_t ICM20602::getRawAccel(){
  uint8_t hByteX,lByteX, hByteY,lByteY, hByteZ,lByteZ;
  hByteX = readByte(ICM20602_ACCEL_XOUT_H);
  lByteX = readByte(ICM20602_ACCEL_XOUT_L);
  hByteY = readByte(ICM20602_ACCEL_YOUT_H);
  lByteY = readByte(ICM20602_ACCEL_YOUT_L);
  hByteZ = readByte(ICM20602_ACCEL_ZOUT_H);
  lByteZ = readByte(ICM20602_ACCEL_ZOUT_L);
  ICM20602::accelRaw.x = (hByteX<<8|lByteX);
  ICM20602::accelRaw.y = (hByteY<<8|lByteY);
  ICM20602::accelRaw.z = (hByteZ<<8|lByteZ);
  return ICM20602::accelRaw;
}

/*
Parameters: none
Returns: The acceleration in m/s^2 as a struct of sensor_float_vec_t type
This function converts the raw acceleration in LSB/G to 
meters per second squared.
*/
sensor_float_vec_t ICM20602::getMPSAccel(){
  // TODO: Apply sensitivity factor, currently hard coded
  float MPSScale = 8.0/32768.0;
  ICM20602::accelMPS.x = accelRaw.x * MPSScale;
  ICM20602::accelMPS.y = accelRaw.y * MPSScale;
  ICM20602::accelMPS.z = accelRaw.z * MPSScale;
  return ICM20602::accelMPS;    
}
//testing colaberative work

/*
Parameters: current scale of the sensor as the Ascale enumeration
Returns: the sensitivity scale factor as a FLOAT 
This function uses a switch statement to return the sensitivity scale facor
depending on the current sensing accuracy scale.
*/
float ICM20602::getSensitivity(enum Ascale scaleA){
  // TODO: Write setter for sensity a variable, possibly private
  // TODO: Set sensitivity on initialization
  float factor;
  switch (scaleA) {
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
  return factor;
}

/*
Parameters: current scale of the sensor as the Ascale enumeration
Returns: the acceleration in G's as a sensor_float_vec_t type
This function converts the raw acceleration in LSB/G to the acceleration in 
G's by dividing the sensitivity factor based on the current sensitivity scale.
*/
sensor_float_vec_t ICM20602::getGAccel(enum Ascale scaleA){
  float factor = getSensitivity(scaleA);
  ICM20602::accelG.x = accelRaw.x/ factor;
  ICM20602::accelG.y = accelRaw.y/ factor;
  ICM20602::accelG.z = accelRaw.z/ factor;
	return ICM20602::accelG;
}

sensor_float_vec_t ICM20602::getGAccel_fuzzed(enum Ascale scaleA){
  float factor = getSensitivity(scaleA);
  ICM20602::accelG.x = factor*1;
  ICM20602::accelG.y = factor*2;
  ICM20602::accelG.z = factor*3;
	return ICM20602::accelG;
}


/*
void setScale(enum Ascale newScale){
   scaleA = newScale;
   //writebyte??
}
*/
