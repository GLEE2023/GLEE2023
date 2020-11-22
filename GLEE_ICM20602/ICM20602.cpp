#include <Arduino.h>
#include "ICM20602.h"
#include <Wire.h>

bool ICM_DEBUG = false;

// Scale resolutions per LSB for the sensors
float aRes, gRes; 
//int Ascale = AFS_2G;
int Gscale = GFS_1000DPS;

ICM20602::ICM20602(int _id, bool _debug){
    ICM20602::info.name = "ICM20602 Inertial Measurement Unit";
  	ICM20602::info.address = ICM20602_slave_addr;
  	ICM20602::sensorDebug = _debug;
    ICM20602::scaleA = AFS_2G;
}

bool ICM20602::begin(void){
  Wire.begin();
  Serial.println("Beginning Communication With ICM20602");
  return true;
}

void ICM20602::initialize(void){
  Wire.setClock(100000);
  writeByte(ICM20602_PWR_MGMT_1, 0x01);   // Set clock set to auto
  writeByte(ICM20602_PWR_MGMT_2,0x07);    // Disable gyro
  writeByte(ICM20602_CONFIG, 0x01); 
  writeByte(ICM20602_GYRO_CONFIG, 0x00);    
  writeByte(ICM20602_ACCEL_CONFIG, 0x10);   
}

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

sensor_float_vec_t ICM20602::getMPSAccel(){
  float MPSScale = 8.0/32768.0;
  ICM20602::accelMPS.x = accelRaw.x * MPSScale;
  ICM20602::accelMPS.x = accelRaw.y * MPSScale;
  ICM20602::accelMPS.x = accelRaw.z * MPSScale;
  return ICM20602::accelMPS;    
}
//testing

float ICM20602::getSensitivity(enum Ascale scaleA){
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

sensor_float_vec_t ICM20602::getGAccel(enum Ascale scaleA){
  float factor = getSensitivity(scaleA);
  ICM20602::accelG.x = accelRaw.x/ factor;
  ICM20602::accelG.y = accelRaw.y/ factor;
  ICM20602::accelG.z = accelRaw.z/ factor;
	return ICM20602::accelG;
}


/*
void setScale(enum Ascale newScale){
   scaleA = newScale;
   //writebyte??
}
*/