#include <Arduino.h>
#include "Lunar_I2C.h"

#define MPU6000_DEBUG
//Things that john is letting users change - MPU6000_SMPLRT_DIV, DLPF

#define MPU6000_I2CADDR_DEFAULT 0x69    // MPU6000 default i2c address w/ AD0 high
#define MPU6000_ACCEL_CONFIG 0x1C       // Accelerometer specific configration register

#define MPU6000_SMPLRT_DIV 0x19         // sample rate divisor register
#define MPU6000_SMPLRT_DIV_VAL 0x0      // afffects the sampling rate, ranges from 0x00 to 0xFF

#define MPU6000_CONFIG 0x1A             // General configuration register
#define MPU6000_CONFIG_DLPF_BW 0x0        // changes bandwidth for digital low pass filter. Ranges from 0x0 to 0x7


#define MPU6000_PWR_MGMT_1 0x6B         // Primary power/sleep control register
#define MPU6000_PWR_MGMT_2 0x6C
#define MPU6000_LSB_PER_G 16384.0       // How many bits per LSB
#define MPU6000_LSB_PER_DEG 131         // divide gyro output by this to get in deg/s
#define MPU6000_ACCEL_OUT 0x3B          // base raw accel address (6 bytes for 3 axis)
#define MPU6000_GYRO_OUT 0x43           // location of gyro output
#define MPU6000_ACC_ONLY 0x07           //write this to MPU6000_PWR_MGMT_2 to put gyro to sleep
#define MPU6000_GYRO_ONLY 0x38          //write this to MPU6000_PWR_MGMT_2 to put acc to sleep
#define MPU6000_ACC_AND_GYRO 0x00       ////write this to MPU6000_PWR_MGMT_2 to use both acc and gyro

//see register 6B and 6C in the register map for more info
#define MPU6000_LP_WAKE_REG_6B_VAL 0b00101001    // To turn on Low Power Wake up
#define MPU6000_LP_WAKE_UP_REG_6C_1PT25HZ 0b00000111
#define MPU6000_LP_WAKE_UP_REG_6C_5HZ 0b01000111
#define MPU6000_LP_WAKE_UP_REG_6C_20HZ 0b10000111
#define MPU6000_LP_WAKE_UP_REG_6C_40HZ 0b11000111

#define MPU6000_SLEEP_VAL 0x40 //  write to MPU6000_PWR_MGMT_1 to put into sleep
#define MPU6000_RESET_VAL 0x80 // write to MPU6000_PWR_MGMT_1 to reset



namespace MPU6000_Lunar{

  void setConfig(uint16_t config_string);
  void readAccData(uint8_t* buffer);
  void convertAccToFloat(uint8_t* buffer, float* nums);
  void readGyroData(uint8_t* buffer);
  void convertGyroToFloat(uint8_t* buffer, float* nums);
}
