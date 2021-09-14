/*
*Description: This is an Arduino (C++) .h file required for the MPU6000 Accelerometer
*Project Info: Created For GLEE (The Great Lunar Expedition for Everyone)
*Library Author: Lawson Nerenberg (heavily based on Adafruit_MPU6000 Library: https://github.com/adafruit/Adafruit_MPU6000)
*Library Created on: Aug. 2 2021
*Last Modified on: Aug. 2 2021
*Resources Used in Creation:
*MPU6000 Datasheet
*Adafruit_MPU6000 Library: https://github.com/adafruit/Adafruit_MPU6000
*/
#ifndef MPU6000_H
#define MPU6000_H

#include <Arduino.h>
#include "GLEE_Sensor.h"

// Following are registers currently utilized
// MPU6000 Register Constants 

#define MPU6000_I2CADDR_DEFAULT 0x69    // MPU6000 default i2c address w/ AD0 high
#define MPU6000_DEVICE_ID 0x68          // The correct MPU6000_WHO_AM_I value
#define MPU6000_SELF_TEST_X 0x0D        // Self test factory calibrated values register
#define MPU6000_SELF_TEST_Y 0x0E 
#define MPU6000_SELF_TEST_Z 0x0F 
#define MPU6000_SELF_TEST_A 0x10 
#define MPU6000_SMPLRT_DIV 0x19         // sample rate divisor register
#define MPU6000_CONFIG 0x1A             // General configuration register
#define MPU6000_GYRO_CONFIG 0x1B        // Gyro specfic configuration register
#define MPU6000_ACCEL_CONFIG 0x1C       // Accelerometer specific configration register
#define MPU6000_INT_PIN_CONFIG 0x37     // Interrupt pin configuration register
#define MPU6000_WHO_AM_I 0x75           // Divice ID register
#define MPU6000_SIGNAL_PATH_RESET 0x68  // Signal path reset register
#define MPU6000_USER_CTRL 0x6A          // FIFO and I2C Master control register
#define MPU6000_PWR_MGMT_1 0x6B         // Primary power/sleep control register
#define MPU6000_PWR_MGMT_2 0x6C         // Secondary power/sleep control register
#define MPU6000_TEMP_H 0x41             // Temperature data high byte register
#define MPU6000_TEMP_L 0x42             // Temperature data low byte register
#define MPU6000_ACCEL_OUT 0x3B          // base raw accel address (6 bytes for 3 axis)
#define MPU6000_TEMP_OUT 0x41           // base raw temp address
#define MPU6000_GYRO_OUT 0x43           // base raw gyro address (6 bytes for 3 axis)

#define MPU6000_CONFIG_FS_SEL_BIT 4
#define MPU6000_CONFIG_FS_SEL_LEN 2

// Constants 
#define MPU_ONE_G 9.80665

extern float aRes, gRes; 

// Accl Full Scale Range  +/-2G, 4G, 8G, 16G, 
typedef enum{
    MPU6000_RANGE_2_G = 0b00,   // +/- 2g (default)
    MPU6000_RANGE_4_G = 0b01,   // +/- 4g
    MPU6000_RANGE_8_G = 0b10,   // +/- 8g
    MPU6000_RANGE_16_G = 0b11,  // +/- 16g
} mpu6000_accel_range_t;

// Gyro Full Scale Range  +/- 250, 500, 1000, 2000 deg/s 
typedef enum {
    MPU6000_RANGE_250_DEG = 0b00,  // +/- 250 deg/s (default value)
    MPU6000_RANGE_500_DEG = 0b01,  // +/- 500 deg/s
    MPU6000_RANGE_1000_DEG = 0b10, // +/- 1000 deg/s
    MPU6000_RANGE_2000_DEG = 0b11, // +/- 2000 deg/s
} mpu6000_gyro_range_t;

// From Adafruit Lib
typedef enum {
    MPU6000_BAND_260_HZ, ///< Docs imply this disables the filter
    MPU6000_BAND_184_HZ, ///< 184 Hz
    MPU6000_BAND_94_HZ,  ///< 94 Hz
    MPU6000_BAND_44_HZ,  ///< 44 Hz
    MPU6000_BAND_21_HZ,  ///< 21 Hz
    MPU6000_BAND_10_HZ,  ///< 10 Hz
    MPU6000_BAND_5_HZ,   ///< 5 Hz
} mpu6000_bandwidth_t;

class MPU6000:public Sensor{
    public:
        MPU6000(int _id, bool _debug = false);
        
        mpu6000_accel_range_t accel_range; //g-force range
        mpu6000_gyro_range_t gyro_range; //DPS range
        
        float accel_scale;
        float gyro_scale;
        
        bool gyroOn;

        bool begin();
        void initialize();
        // void disableGyro(bool disableGyro);
        // int16_t read2Byte(uint8_t registerAddress);
        sensor_int16_vec_t getRawAcc();
        sensor_int16_vec_t getRawGyro();

        sensor_float_vec_t getAcc(sensor_int16_vec_t rawAcc);
        sensor_float_vec_t getGyro(sensor_int16_vec_t rawGyro);
        
        sensor_float_vec_t getMPSAccel(sensor_float_vec_t GAccel);
        sensor_float_vec_t getGAccel(sensor_int16_vec_t rawAccel);
        
        // sensor_float_vec_t getDPSAngVel(sensor_int16_vec_t rawAngVel);
        // sensor_float_vec_t getGAccel_fuzzed();

        // void updateRawAccel(sensor_int16_vec_t rawAccel);
        // void updateMPSAccel(sensor_float_vec_t MPSAccel);
        // void updateGAccel(sensor_float_vec_t GAccel);

        // void updateRawAngVel(sensor_int16_vec_t rawAngVel);
        // void updateDPSAngVel(sensor_float_vec_t DPSAngVel);

        void setAccelRange(mpu6000_accel_range_t new_range);
        void setGyroRange(mpu6000_gyro_range_t new_range);

        float getAccelSensitivity();
        // float getGyroSensitivity();

        sensor_float_vec_t getSample();
        sensor_float_vec_t getGyroSample();

        void reset();
        void resetSigPath();

        void setSampleRateDivisor(uint8_t divisor);
        void setFilterBandwidth(mpu6000_bandwidth_t bandwidth)

        // setFilterBandwidth
        // setGyroRange
        // setAccelerometerRange
        // setPowerMgmt_1

    private:
        
        sensor_int16_vec_t accelRaw;
        sensor_float_vec_t accelMPS;
        sensor_float_vec_t accelG;

        sensor_int16_vec_t angVelRaw;
        sensor_float_vec_t angVelDPS;

    protected:
        float temperature;

};
#endif