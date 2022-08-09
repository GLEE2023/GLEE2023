#include "MPU6000_Lunar.h"
#include <Wire.h>

void MPU6000_Lunar::readAccData(uint8_t* buffer){
  Lunar_I2C::readBytes(MPU6000_I2CADDR_DEFAULT, MPU6000_ACCEL_OUT, 6, &buffer[0]);
}

//converts buffer to floats assuming the range is +-2g
void MPU6000_Lunar::convertAccToFloat(uint8_t* buffer, float* nums){
  nums[0] = (float)(buffer[0] << 8 | buffer[1]) / MPU6000_LSB_PER_G * -1;
  nums[1] = (float)(buffer[2] << 8 | buffer[3]) / MPU6000_LSB_PER_G * -1;
  nums[2] = (float)(buffer[4] << 8 | buffer[5]) / MPU6000_LSB_PER_G * -1;
}

void MPU6000_Lunar::readGyroData(uint8_t* buffer){
  Lunar_I2C::readBytes(MPU6000_I2CADDR_DEFAULT, MPU6000_GYRO_OUT, 6, &buffer[0]);
}

//might be an error in this. Put together hapazardly
void MPU6000_Lunar::convertGyroToFloat(uint8_t* buffer, float* nums){
  nums[0] = (float)(buffer[0] << 8 | buffer[1]) / MPU6000_LSB_PER_DEG * -1;
  nums[1] = (float)(buffer[2] << 8 | buffer[3]) / MPU6000_LSB_PER_DEG * -1;
  nums[2] = (float)(buffer[4] << 8 | buffer[5]) / MPU6000_LSB_PER_DEG * -1;
}

void MPU6000_Lunar::setConfig(uint16_t config_string){
  Serial.print("MPU Config string: ");
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
      #ifdef MPU6000_DEBUG
        Serial.println(F("MPU Mode Set to sleep"));
      #endif
      break;
    case 0x1:
      // low power wakeup @ 1.25 Hz
      Lunar_I2C::writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_1,MPU6000_LP_WAKE_REG_6B_VAL);
      Lunar_I2C::writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_2,MPU6000_LP_WAKE_UP_REG_6C_1PT25HZ);
      #ifdef MPU6000_DEBUG
        Serial.println(F("MPU Mode Set to MPU6000_LP_WAKE_UP_REG_6C_1PT25HZ"));
      #endif
      break;
    case 0x2:
      // low power wakeup @ 5 Hz
      Lunar_I2C::writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_1,MPU6000_LP_WAKE_REG_6B_VAL);
      Lunar_I2C::writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_2,MPU6000_LP_WAKE_UP_REG_6C_5HZ);
      #ifdef MPU6000_DEBUG
        Serial.println(F("MPU Mode Set to MPU6000_LP_WAKE_UP_REG_6C_5HZ"));
      #endif

      break;
    case 0x3:
      // low power wakeup @ 20 Hz
      Lunar_I2C::writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_1,MPU6000_LP_WAKE_REG_6B_VAL);
      Lunar_I2C::writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_2,MPU6000_LP_WAKE_UP_REG_6C_20HZ);
      #ifdef MPU6000_DEBUG
        Serial.println(F("MPU Mode Set to MPU6000_LP_WAKE_UP_REG_6C_20HZ"));
      #endif
      break;
    case 0x4:
      // low power wakeup @ 40 Hz
      Lunar_I2C::writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_1,MPU6000_LP_WAKE_REG_6B_VAL);
      Lunar_I2C::writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_2,MPU6000_LP_WAKE_UP_REG_6C_40HZ);
      #ifdef MPU6000_DEBUG
        Serial.println(F("MPU Mode Set to MPU6000_LP_WAKE_UP_REG_6C_40HZ"));
      #endif
      break;
    case 0x5:
      //accerometer only
      Lunar_I2C::writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_2, MPU6000_ACC_ONLY);
      #ifdef MPU6000_DEBUG
        Serial.println(F("MPU Mode Set to MPU6000_ACC_ONLY"));
      #endif
      break;
    case 0x6:
      //gyro only
      Lunar_I2C::writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_2, MPU6000_GYRO_ONLY);
      #ifdef MPU6000_DEBUG
        Serial.println(F("MPU Mode Set to MPU6000_GYRO_ONLY"));
      #endif
      break;
    case 0x7:
      // Accelerometer & Gyro
      Lunar_I2C::writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_2, MPU6000_ACC_AND_GYRO);
      #ifdef MPU6000_DEBUG
        Serial.println(F("MPU Mode Set to MPU6000_ACC_AND_GYRO"));
      #endif
      break;
    default:
      // default to only accelerometer
      Lunar_I2C::writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_2, MPU6000_ACC_ONLY);
      #ifdef MPU6000_DEBUG
        Serial.println(F("MPU Unknown Mode. Mode Set to MPU6000_ACC_ONLY"));
      #endif
      break;
  }

  //set sampling rate div
  #ifdef MPU6000_DEBUG
    uint8_t sampling_rate_div = (uint8_t)(config_string>>4 & 0xFF);
    // Lunar_I2C::writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_SMPLRT_DIV,sampling_rate_div);
    Serial.println("MPU Sampling rate division set to "+String(sampling_rate_div));

    // Set digital low pass filter bandwidth
    uint8_t dlpf_value = (uint8_t)(config_string>>12 & 0x7);
    // Lunar_I2C::writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_CONFIG,dlpf_value);
    Serial.println("MPU Digtal low pass filter set to "+String(dlpf_value));
  #endif
  Lunar_I2C::writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_SMPLRT_DIV,(uint8_t)(config_string>>4 & 0xFF));
  Lunar_I2C::writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_CONFIG,(uint8_t)(config_string>>12 & 0x7));

}
