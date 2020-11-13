#include <Arduino.h>
#include "ICM20602.h"
#include <Wire.h>

bool ICM_DEBUG = false;

// Scale resolutions per LSB for the sensors
float aRes, gRes; 
//int Ascale = AFS_2G;
int Gscale = GFS_1000DPS;

ICM20602::ICM20602(void){
    ICM20602::sensorName = "ICM20602 Inertial Measurement Unit";
  	ICM20602::sensorAddress = 0x48;
  	ICM20602::sensorDebug = false;
    scaleA = AFS_2G;
}

bool ICM20602::begin(void){
  Wire.begin(ICM20602_slave_addr);
  Serial.println("Beginning Communication With ICM20602");
  return true;
}

void ICM20602::initialize(void){
  Wire.setClock(400000);
  writeByte(ICM20602_PWR_MGMT_1, 0x01);   //set clock set to auto
  writeByte(ICM20602_PWR_MGMT_2,0x07);    //disable gyro
  writeByte(ICM20602_CONFIG, 0x01); //176Hz     // set TEMP_OUT_L, DLPF=3 (Fs=1KHz):0x03
  writeByte(ICM20602_GYRO_CONFIG, 0x00);    // bit[4:3] 0=+-250d/s,1=+-500d/s,2=+-1000d/s,3=+-2000d/s :0x18
  writeByte(ICM20602_ACCEL_CONFIG, 0x10);   // bit[4:3] 0=+-2g,1=+-4g,2=+-8g,3=+-16g, ACC_HPF=On (5Hz):0x01
}

sensor_uint16_vec_t ICM20602::getRawAccel(){
  int hByteX,lByteX, hByteY,lByteY, hByteZ,lByteZ;
  hByteX = readByte(ICM20602_ACCEL_XOUT_H);
  lByteX = readByte(ICM20602_ACCEL_XOUT_L);
  hByteY = readByte(ICM20602_ACCEL_YOUT_H);
  lByteY = readByte(ICM20602_ACCEL_YOUT_L);
  hByteZ = readByte(ICM20602_ACCEL_ZOUT_H);
  lByteZ = readByte(ICM20602_ACCEL_ZOUT_L);
  accelRaw.x = (hByteX<<8|lByteX);
  accelRaw.y = (hByteY<<8|lByteY);
  accelRaw.z = (hByteZ<<8|lByteZ);
  return accelRaw;
}

sensor_float_vec_t ICM20602::getMPSAccel(){
  float MPSScale = 8.0/32768.0;
  accelMPS.x = accelRaw.x* MPSScale;
  accelMPS.x = accelRaw.y* MPSScale;
  accelMPS.x = accelRaw.z* MPSScale;
  return accelMPS;    
}

/*
void setScale(enum Ascale newScale){
   scaleA = newScale;
   //writebyte??
}
*/

float getSensitivity(enum Ascale scaleA){
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
  accelG.x = accelRaw.x/ factor;
  accelG.y = accelRaw.y/ factor;
  accelG.z = accelRaw.z/ factor;
	return accelG;
}
