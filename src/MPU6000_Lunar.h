#include <Arduino.h>
#include "Lunar_I2C.h"

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
#define MPU6000_ACCEL_OUT 0x3B          // base raw accel address (6 bytes for 3 axis)
#define MPU6000_GYRO_OUT 0x43           // location of gyro output
#define MPU6000_ACC_ONLY 0x07           //write this to MPU6000_PWR_MGMT_2 to put gyro to sleep
#define MPU6000_GYRO_ONLY 0x38          //write this to MPU6000_PWR_MGMT_2 to put acc to sleep
#define MPU6000_ACC_AND_GYRO 0x3F       ////write this to MPU6000_PWR_MGMT_2 to use both acc and gyro

//see register 6B and 6C in the register map for more info
#define MPU6000_LP_WAKE_REG_6B_VAL 0b00101001    // To turn on Low Power Wake up
#define MPU6000_LP_WAKE_UP_REG_6C_1PT25HZ 0b00000111
#define MPU6000_LP_WAKE_UP_REG_6C_5HZ 0b01000111
#define MPU6000_LP_WAKE_UP_REG_6C_20HZ 0b10000111
#define MPU6000_LP_WAKE_UP_REG_6C_40HZ 0b11000111

#define MPU6000_SLEEP_VAL 0x40 //  write to MPU6000_PWR_MGMT_1 to put into sleep
#define MPU6000_RESET_VAL 0x80 // write to MPU6000_PWR_MGMT_1 to reset



namespace MPU6000_Lunar{

  void readAccData(uint8_t* buffer){
    readBytes(MPU6000_I2CADDR_DEFAULT, MPU6000_ACCEL_OUT, 6, &buffer[0]);
  }

  void setMPU(uint16_t config_string){
    Serial.print("Acc Config string: ");
    Serial.println(config_string,HEX);
    // Set limits to +/- 2g, don't change
    writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_ACCEL_CONFIG, 0x00);

    // sets which clock the acc uses, don't change this
    writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_1,0x01);

    //This switch sets the mode
    switch(config_string>>0 & 0xF){
      //gets the first 4 bits
      case 0x0:
        // sleep
        writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_1,MPU6000_SLEEP_VAL);
        Serial.println(F("Mode Set to sleep"));
        break;
      case 0x1:
        // low power wakeup @ 1.25 Hz
        writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_1,MPU6000_LP_WAKE_REG_6B_VAL);
        writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_2,MPU6000_LP_WAKE_UP_REG_6C_1PT25HZ);
        Serial.println(F("Mode Set to MPU6000_LP_WAKE_UP_REG_6C_1PT25HZ"));
        break;
      case 0x2:
        // low power wakeup @ 5 Hz
        writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_1,MPU6000_LP_WAKE_REG_6B_VAL);
        writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_2,MPU6000_LP_WAKE_UP_REG_6C_5HZ);
        Serial.println(F("Mode Set to MPU6000_LP_WAKE_UP_REG_6C_5HZ"));
        break;
      case 0x3:
        // low power wakeup @ 20 Hz
        writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_1,MPU6000_LP_WAKE_REG_6B_VAL);
        writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_2,MPU6000_LP_WAKE_UP_REG_6C_20HZ);
        Serial.println(F("Mode Set to MPU6000_LP_WAKE_UP_REG_6C_20HZ"));
        break;
      case 0x4:
        // low power wakeup @ 40 Hz
        writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_1,MPU6000_LP_WAKE_REG_6B_VAL);
        writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_2,MPU6000_LP_WAKE_UP_REG_6C_40HZ);
        Serial.println(F("Mode Set to MPU6000_LP_WAKE_UP_REG_6C_40HZ"));
        break;
      case 0x5:
        //accerometer only
        writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_2, MPU6000_ACC_ONLY);
        Serial.println(F("Mode Set to MPU6000_ACC_ONLY"));
        break;
      case 0x6:
        //gyro only
        writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_2, MPU6000_GYRO_ONLY);
        Serial.println(F("Mode Set to MPU6000_GYRO_ONLY"));
        break;
      case 0x7:
        // Accelerometer & Gyro
        writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_2, MPU6000_ACC_AND_GYRO);
        Serial.println(F("Mode Set to MPU6000_ACC_AND_GYRO"));
        break;
      default:
        // default to only accelerometer
        writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_2, MPU6000_ACC_ONLY);
        Serial.println(F("Unknown Mode. Mode Set to MPU6000_ACC_ONLY"));
        break;
    }

    //set sampling rate div
    uint8_t sampling_rate_div = (uint8_t)(config_string>>4 & 0xFF);
    writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_SMPLRT_DIV,sampling_rate_div);
    Serial.println("Sampling rate division set to "+String(sampling_rate_div));

    // Set digital low pass filter bandwidth
    uint8_t dlpf_value = (uint8_t)(config_string>>12 & 0x7);
    writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_CONFIG,dlpf_value);
    Serial.println("Digtal low pass filter set to "+String(dlpf_value));
  }
}
