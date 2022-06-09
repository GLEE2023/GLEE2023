#ifndef MLX90395_H
#define MLX90395_H

#include "Arduino.h"
#include "GLEE_Sensor.h"
#include <Adafruit_BusIO_Register.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_SPIDevice.h>
#include <Adafruit_Sensor.h>

#define MLX90395_STATUS_DRDY (0x01)
#define MLX90395_REG_0 (0x0)
#define MLX90395_REG_1 (0x2)
#define MLX90395_REG_2 (0x4)

#define MLX90395_DEFAULT_ADDR 0x0C

#define MLX90395_AXIS_ALL (0x0E)      /**< X+Y+Z axis bits for commands. */
#define MLX90395_CONF1 (0x00)         /**< Gain */
#define MLX90395_CONF2 (0x01)         /**< Burst, comm mode */
#define MLX90395_CONF3 (0x02)         /**< Oversampling, filter, res. */
#define MLX90395_CONF4 (0x03)         /**< Sensitivty drift. */
#define MLX90395_GAIN_SHIFT (0x04)       /**< Left-shift for gain bits. */
#define MLX90395_HALL_CONF (0x0C)     /**< Hall plate spinning rate adj. */
#define MLX90395_STATUS_OK (0x00)     /**< OK value for status response. */
#define MLX90395_STATUS_SMMODE (0x20) /**< SM Mode status response. */
#define MLX90395_STATUS_RESET (0x02)  /**< Reset value for status response. */
#define MLX90395_STATUS_ERROR (0xFF)  /**< OK value for status response. */
#define MLX90395_STATUS_MASK (0xFC)   /**< Mask for status OK checks. */

/** Register map. */

//enum {
//  MLX90395_REG_SM = (0x30), /**> Start single-meas mode. */
//  MLX90395_REG_EX = (0x80), /**> Exit mode. */
//  MLX90395_REG_RT = (0xF0), /**< Reset. */
//};

/** Register map. */
enum {
    MLX90395_REG_SB = (0x10),  /**< Start burst mode. */
    MLX90395_REG_SW = (0x20),  /**< Start wakeup on change mode. */
    MLX90395_REG_SM = (0x30),  /**> Start single-meas mode. */
    MLX90395_REG_RM = (0x40),  /**> Read measurement. */
    MLX90395_REG_RR = (0x50),  /**< Read register. */
    MLX90395_REG_WR = (0x60),  /**< Write register. */
    MLX90395_REG_EX = (0x80),  /**> Exit moode. */
    MLX90395_REG_HR = (0xD0),  /**< Memory recall. */
    MLX90395_REG_HS = (0x70),  /**< Memory store. */
    MLX90395_REG_RT = (0xF0),  /**< Reset. */
    MLX90395_REG_NOP = (0x00), /**< NOP. */
};

typedef enum mlx90395_osr {
  MLX90395_OSR_1,
  MLX90395_OSR_2,
  MLX90395_OSR_4,
  MLX90395_OSR_8,
} mlx90395_osr_t;

typedef enum mlx90395_res {
  MLX90395_RES_16,
  MLX90395_RES_17,
  MLX90395_RES_18,
  MLX90395_RES_19,
} mlx90395_res_t;

typedef struct{
     sensor_float_vec_t magnetic;
     //float strength;
     //float temp;
} mlx_sample_t;

/** Digital filter settings for CONF3 register (Modified) */
typedef enum mlx90395_filter {
    MLX90395_FILTER_6,
    MLX90395_FILTER_7,
} mlx90395_filter_t;

static const float gainMultipliers[16] = {
    0.2, 0.25,  0.3333, 0.4, 0.5,  0.6, 0.75,  1,
    0.1, 0.125, 0.1667, 0.2, 0.25, 0.3, 0.375, 0.5};

/** Class for interfacing with MLX90395 magnetometer */
class MLX90395 : public Adafruit_Sensor {
public:
  MLX90395(int id_, bool debug_ = false);
  bool begin_I2C(uint8_t i2c_addr = MLX90395_DEFAULT_ADDR,
                 TwoWire *wire = &Wire);
  bool reset(void);
  bool exitMode(void);
  bool startSingleMeasurement(void);
  bool readMeasurement(float *x, float *y, float *z);
  bool readData(float *x, float *y, float *z);

  mlx90395_osr_t getOSR(void);
  bool setOSR(mlx90395_osr_t osrval);
  mlx90395_res_t getResolution(void);
  bool setResolution(enum mlx90395_res resX, enum mlx90395_res resY, enum mlx90395_res resZ);
  uint8_t getGain(void);
  bool setGain(uint8_t gainval);

  void getSensor(sensor_t *sensor);
  bool getEvent(sensors_event_t *event);
  mlx_sample_t getSample(void);
  uint16_t uniqueID[3]; ///< 48 bits of unique identifier, read during init
  enum mlx90395_filter getFilter(void);
  bool setFilter(enum mlx90395_filter filter);

private:
  Adafruit_I2CDevice *i2c_dev = NULL;
  Adafruit_SPIDevice *spi_dev = NULL;

  bool _init(void);
  uint8_t command(uint8_t cmd);
  bool readRegister(uint8_t reg, uint16_t *data);

  enum mlx90395_res _resolutionX = MLX90395_RES_16;
  enum mlx90395_res _resolutionY = MLX90395_RES_16;
  enum mlx90395_res _resolutionZ = MLX90395_RES_16;
  uint8_t _gain = 0;
  float _uTLSB = 0;
  int32_t _sensorID = 90395;

  enum mlx90395_filter _dig_filt;
};

#endif
