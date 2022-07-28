#include "MPU6000_Lunar.h"
#include <Wire.h>

void MPU6000_Lunar::readAccData(uint8_t* buffer){
  Lunar_I2C::readBytes(MPU6000_I2CADDR_DEFAULT, MPU6000_ACCEL_OUT, 6, &buffer[0]);
}

void MPU6000_Lunar::setConfig(uint16_t config_string){
  Serial.print("Acc Config string: ");
  Serial.println(config_string,HEX);
  // Set limits to +/- 2g, don't change
  Lunar_I2C::writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_ACCEL_CONFIG, 0x00);

  // sets which clock the acc uses, don't change this
  Lunar_I2C::writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_1,0x01);

  //This switch sets the mode
  switch(config_string>>0 & 0xF){
    //gets the first 4 bits
    case 0x0:
      // sleep
      Lunar_I2C::writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_1,MPU6000_SLEEP_VAL);
      Serial.println(F("Mode Set to sleep"));
      break;
    case 0x1:
      // low power wakeup @ 1.25 Hz
      Lunar_I2C::writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_1,MPU6000_LP_WAKE_REG_6B_VAL);
      Lunar_I2C::writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_2,MPU6000_LP_WAKE_UP_REG_6C_1PT25HZ);
      Serial.println(F("Mode Set to MPU6000_LP_WAKE_UP_REG_6C_1PT25HZ"));
      break;
    case 0x2:
      // low power wakeup @ 5 Hz
      Lunar_I2C::writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_1,MPU6000_LP_WAKE_REG_6B_VAL);
      Lunar_I2C::writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_2,MPU6000_LP_WAKE_UP_REG_6C_5HZ);
      Serial.println(F("Mode Set to MPU6000_LP_WAKE_UP_REG_6C_5HZ"));
      break;
    case 0x3:
      // low power wakeup @ 20 Hz
      Lunar_I2C::writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_1,MPU6000_LP_WAKE_REG_6B_VAL);
      Lunar_I2C::writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_2,MPU6000_LP_WAKE_UP_REG_6C_20HZ);
      Serial.println(F("Mode Set to MPU6000_LP_WAKE_UP_REG_6C_20HZ"));
      break;
    case 0x4:
      // low power wakeup @ 40 Hz
      Lunar_I2C::writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_1,MPU6000_LP_WAKE_REG_6B_VAL);
      Lunar_I2C::writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_2,MPU6000_LP_WAKE_UP_REG_6C_40HZ);
      Serial.println(F("Mode Set to MPU6000_LP_WAKE_UP_REG_6C_40HZ"));
      break;
    case 0x5:
      //accerometer only
      Lunar_I2C::writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_2, MPU6000_ACC_ONLY);
      Serial.println(F("Mode Set to MPU6000_ACC_ONLY"));
      break;
    case 0x6:
      //gyro only
      Lunar_I2C::writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_2, MPU6000_GYRO_ONLY);
      Serial.println(F("Mode Set to MPU6000_GYRO_ONLY"));
      break;
    case 0x7:
      // Accelerometer & Gyro
      Lunar_I2C::writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_2, MPU6000_ACC_AND_GYRO);
      Serial.println(F("Mode Set to MPU6000_ACC_AND_GYRO"));
      break;
    default:
      // default to only accelerometer
      Lunar_I2C::writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_2, MPU6000_ACC_ONLY);
      Serial.println(F("Unknown Mode. Mode Set to MPU6000_ACC_ONLY"));
      break;
  }

  //set sampling rate div
  uint8_t sampling_rate_div = (uint8_t)(config_string>>4 & 0xFF);
  Lunar_I2C::writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_SMPLRT_DIV,sampling_rate_div);
  Serial.println("Sampling rate division set to "+String(sampling_rate_div));

  // Set digital low pass filter bandwidth
  uint8_t dlpf_value = (uint8_t)(config_string>>12 & 0x7);
  Lunar_I2C::writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_CONFIG,dlpf_value);
  Serial.println("Digtal low pass filter set to "+String(dlpf_value));
}
