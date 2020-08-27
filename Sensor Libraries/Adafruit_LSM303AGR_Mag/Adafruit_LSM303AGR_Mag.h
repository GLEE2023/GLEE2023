/***************************************************************************
  This is a library for the LSM303AGR  magnentometer/compass

  Designed specifically to work with the Adafruit LSM303DLHC Breakout

  These displays use I2C to communicate, 2 pins are required to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Bryan Siepert for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/
#ifndef LSM303AGR_MAG_H
#define LSM303AGR_MAG_H

#include <Adafruit_BusIO_Register.h>

#include <Adafruit_I2CDevice.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

/*=========================================================================
    I2C ADDRESS/BITS
    -----------------------------------------------------------------------*/
#define _ADDRESS_MAG 0x1E //< Default address
#define _CHIP_ID 0x40     //< Chip ID from WHO_AM_I register
#define LSM303AGR_MAG_LSB_XY 4.3
#define LSM303AGR_MAG_LSB_Z 4.9
#define LSM303AGR_MILLIGAUSS_TO_MICROTESLA 0.1
/*=========================================================================*/

typedef enum {
  LSM303AGR_OFFSET_X_REG_L = 0x45,
  LSM303AGR_OFFSET_X_REG_H = 0x46,
  LSM303AGR_OFFSET_Y_REG_L = 0x47,
  LSM303AGR_OFFSET_Y_REG_H = 0x48,
  LSM303AGR_OFFSET_Z_REG_L = 0x49,
  LSM303AGR_OFFSET_Z_REG_H = 0x4A,
  LSM303AGR_WHO_AM_I = 0x4F,
  LSM303AGR_CFG_REG_A = 0x60,
  LSM303AGR_CFG_REG_B = 0x61,
  LSM303AGR_CFG_REG_C = 0x62,
  LSM303AGR_INT_CRTL_REG = 0x63,
  LSM303AGR_INT_SOURCE_REG = 0x64,
  LSM303AGR_INT_THS_L_REG = 0x65,
  LSM303AGR_STATUS_REG = 0x67,
  LSM303AGR_OUTX_L_REG = 0x68,
  LSM303AGR_OUTX_H_REG = 0x69,
  LSM303AGR_OUTY_L_REG = 0x6A,
  LSM303AGR_OUTY_H_REG = 0x6B,
  LSM303AGR_OUTZ_L_REG = 0x6C,
  LSM303AGR_OUTZ_H_REG = 0x6D,
} lsm303AGRMagRegisters_t;
/*=========================================================================*/

/*=========================================================================
    MAGNETOMETER UPDATE RATE SETTINGS
    -----------------------------------------------------------------------*/
typedef enum {
  LSM303AGR_RATE_10_HZ,  //< 10 Hz
  LSM303AGR_RATE_20_HZ,  //< 20 Hz
  LSM303AGR_RATE_50_HZ,  //< 50 Hz
  LSM303AGR_RATE_100_HZ, //< 100 Hz
} lsm303AGRMagRate;
/*=========================================================================*/

/**************************************************************************/
/*!
    @brief  a data thing
*/
/**************************************************************************/
typedef struct lsm303AGRMagData_s {
  int16_t x; ///< x-axis raw data
  int16_t y; ///< y-axis raw data
  int16_t z; ///< z-axis raw data
} lsm303AGRMagData;
/*=========================================================================*/

/**************************************************************************/
/*!
    @brief  Unified sensor driver for the magnetometer
*/
/**************************************************************************/
class Adafruit_LSM303AGR_Mag_Unified : public Adafruit_Sensor {
public:
  Adafruit_LSM303AGR_Mag_Unified(int32_t sensorID = -1);

  bool begin(uint8_t i2c_addr = _ADDRESS_MAG, TwoWire *wire = &Wire);

  lsm303AGRMagRate getDataRate();
  void setDataRate(lsm303AGRMagRate rate);
  bool getEvent(sensors_event_t *);
  void getSensor(sensor_t *);
  void reset(void);

  void enableInterrupts(bool);
  void interruptsActiveHigh(bool);
  lsm303AGRMagData raw; ///< struct instance to hold raw data

private:
  int32_t _sensorID;
  Adafruit_BusIO_Register *config_a;
  void writeByte(uint8_t LSM303_reg,uint8_t LSM303_data);
  void read(void);
  Adafruit_I2CDevice *i2c_dev;
};

#endif
