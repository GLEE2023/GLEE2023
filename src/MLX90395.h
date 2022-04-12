/**
 * Description: This is an Arduino (C++) Header file required for the MLX90395 Magnetometer
 * Project Info: Created For GLEE (The Great Lunar Expedition for Everyone)
 * Library Contributors: Matt Moran, Lawson Nerenberg
 * Primary Resources Used in Creation:
 * MLX90395 Datasheet (https://media.digikey.com/pdf/Data%20Sheets/AKM%20Semiconductor%20Inc.%20PDFs/MLX90395_Prelim_DS_11-2-18.pdf)
 * Arduino Wire Library Reference Guide
 *
**/

#ifndef MLX90395_H
#define MLX90395_H

#include <Arduino.h>
#include "GLEE_Sensor.h"
#include <Adafruit_I2CDevice.h>
#include <Adafruit_SPIDevice.h>
#include <Adafruit_Sensor.h>

//Registers

// MLX90395 Sensor ADDRESS as given by data sheet: 0x0C
#define MLX90395_DEFAULT_ADDR 0x0C

#define MLX90395_STATUS_RESET 0x02
#define MLX90395_STATUS_SMMODE 0x20
#define MLX90395_STATUS_DRDY 0x01
#define MLX90395_REG_0 0x0
#define MLX90395_REG_1 0x2
#define MLX90395_REG_2 0x4

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

/** Gain settings for CONF1 register. */
/*
typedef enum mlx90395_gain {
    MLX90395_GAIN_5X = (0x00),
    MLX90395_GAIN_4X,
    MLX90395_GAIN_3X,
    MLX90395_GAIN_2_5X,
    MLX90395_GAIN_2X,
    MLX90395_GAIN_1_67X,
    MLX90395_GAIN_1_33X,
    MLX90395_GAIN_1X
} mlx90395_gain_t;
*/

/** Gain settings for CONF1 register. (Modified) */
typedef enum mlx90395_gain {
    MLX90395_GAIN_2_5X = (0x00),
    MLX90395_GAIN_1X
} mlx90395_gain_t;

/** Resolution settings for CONF3 register. */
typedef enum mlx90395_resolution {
    MLX90395_RES_16,
    MLX90395_RES_17,
    MLX90395_RES_18,
    MLX90395_RES_19,
} mlx90395_resolution_t;

/** Axis designator. */
typedef enum mlx90395_axis {
    MLX90395_X,
    MLX90395_Y,
    MLX90395_Z
} mlx90395_axis_t;

/** Digital filter settings for CONF3 register. */
/*
typedef enum mlx90395_filter {
    MLX90395_FILTER_0,
    MLX90395_FILTER_1,
    MLX90395_FILTER_2,
    MLX90395_FILTER_3,
    MLX90395_FILTER_4,
    MLX90395_FILTER_5,
    MLX90395_FILTER_6,
    MLX90395_FILTER_7,
} mlx90395_filter_t;
*/

/** Digital filter settings for CONF3 register (Modified) */
typedef enum mlx90395_filter {
    MLX90395_FILTER_6,
    MLX90395_FILTER_7,
} mlx90395_filter_t;

/** Oversampling settings for CONF3 register. */
typedef enum mlx90395_oversampling {
    MLX90395_OSR_0,
    MLX90395_OSR_1,
    MLX90395_OSR_2,
    MLX90395_OSR_3,
} mlx90395_oversampling_t;

/** Lookup table to convert raw values to uT based on [HALLCONF][GAIN_SEL][RES].
 */
const float mlx90395_lsb_lookup[1][2][4][2] = {
    /* HALLCONF = 0xC (default) */
    {
        /* GAIN_SEL = 0, 5x gain */
        //{{0.751, 1.210}, {1.502, 2.420}, {3.004, 4.840}, {6.009, 9.680}},
        /* GAIN_SEL = 1, 4x gain */
        //{{0.601, 0.968}, {1.202, 1.936}, {2.403, 3.872}, {4.840, 7.744}},
        /* GAIN_SEL = 2, 3x gain */
        //{{0.451, 0.726}, {0.901, 1.452}, {1.803, 2.904}, {3.605, 5.808}},
        /* GAIN_SEL = 3, 2.5x gain */
        {{0.376, 0.605}, {0.751, 1.210}, {1.502, 2.420}, {3.004, 4.840}},
        /* GAIN_SEL = 4, 2x gain */
        //{{0.300, 0.484}, {0.601, 0.968}, {1.202, 1.936}, {2.403, 3.872}},
        /* GAIN_SEL = 5, 1.667x gain */
        //{{0.250, 0.403}, {0.501, 0.807}, {1.001, 1.613}, {2.003, 3.227}},
        /* GAIN_SEL = 6, 1.333x gain */
        //{{0.200, 0.323}, {0.401, 0.645}, {0.801, 1.291}, {1.602, 2.581}},
        /* GAIN_SEL = 7, 1x gain */
        {{0.150, 0.242}, {0.300, 0.484}, {0.601, 0.968}, {1.202, 1.936}},
    },
    
    /* HALLCONF = 0x0 */
    // {
    //     /* GAIN_SEL = 0, 5x gain */
    //     {{0.787, 1.267}, {1.573, 2.534}, {3.146, 5.068}, {6.292, 10.137}},
    //     /* GAIN_SEL = 1, 4x gain */
    //     {{0.629, 1.014}, {1.258, 2.027}, {2.517, 4.055}, {5.034, 8.109}},
    //     /* GAIN_SEL = 2, 3x gain */
    //     {{0.472, 0.760}, {0.944, 1.521}, {1.888, 3.041}, {3.775, 6.082}},
    //     /* GAIN_SEL = 3, 2.5x gain */
    //     {{0.393, 0.634}, {0.787, 1.267}, {1.573, 2.534}, {3.146, 5.068}},
    //     /* GAIN_SEL = 4, 2x gain */
    //     {{0.315, 0.507}, {0.629, 1.014}, {1.258, 2.027}, {2.517, 4.055}},
    //     /* GAIN_SEL = 5, 1.667x gain */
    //     {{0.262, 0.422}, {0.524, 0.845}, {1.049, 1.689}, {2.097, 3.379}},
    //     /* GAIN_SEL = 6, 1.333x gain */
    //     {{0.210, 0.338}, {0.419, 0.676}, {0.839, 1.352}, {1.678, 2.703}},
    //     /* GAIN_SEL = 7, 1x gain */
    //     {{0.157, 0.253}, {0.315, 0.507}, {0.629, 1.014}, {1.258, 2.027}},
    // }
    
    };

/** Lookup table for conversion time based on [DIF_FILT][OSR].
 */
const float mlx90395_tconv[2][4] = {
    /* DIG_FILT = 0 */
    //{1.27, 1.84, 3.00, 5.30},
    /* DIG_FILT = 1 */
    //{1.46, 2.23, 3.76, 6.84},
    /* DIG_FILT = 2 */
    //{1.84, 3.00, 5.30, 9.91},
    /* DIG_FILT = 3 */
    //{2.61, 4.53, 8.37, 16.05},
    /* DIG_FILT = 4 */
    //{4.15, 7.60, 14.52, 28.34},
    /* DIG_FILT = 5 */
    //{7.22, 13.75, 26.80, 52.92},
    /* DIG_FILT = 6 */
    {13.36, 26.04, 51.38, 102.07},
    /* DIF_FILT = 7 */
    {25.65, 50.61, 100.53, 200.37},
};

typedef struct{
    sensor_float_vec_t magnetic;
    //float strength;
    //float temp;
} mlx_sample_t;

// Primary Magnetometor class inharents parent sensor class variables and methods
class MLX90395: public Sensor{
    // Initialization for Sensor Data, Sensor Info and Data Structures 
    public:
        MLX90395(int _id, bool _debug = false);
            //float getMagFieldStrength(sensor_float_vec_t magnetic);
            mlx_sample_t getSample(void);

            bool begin_I2C(uint8_t i2c_addr = MLX90395_DEFAULT_ADDR, TwoWire *wire = &Wire);
            //bool begin_SPI(uint8_t cs_pin, SPIClass *theSPI = &SPI);

            bool reset(void);
            bool exitMode(void);

            bool readMeasurement(float *x, float *y, float *z);
            bool startSingleMeasurement(void);

            bool setGain(enum mlx90395_gain gain);
            enum mlx90395_gain getGain(void);

            bool setResolution(enum mlx90395_axis, enum mlx90395_resolution resolution);
            enum mlx90395_resolution getResolution(enum mlx90395_axis);

            bool setFilter(enum mlx90395_filter filter);
            enum mlx90395_filter getFilter(void);

            bool setOversampling(enum mlx90395_oversampling oversampling);
            enum mlx90395_oversampling getOversampling(void);

            bool setTrigInt(bool state);
            bool readData(float *x, float *y, float *z);

            //sensor_float_vec_t getMagnetic(float *x, float *y, float *z);

            //bool getEvent(sensors_event_t *event);
            void getSensor(sensor_t *sensor);

      private:
          Adafruit_I2CDevice *i2c_dev = NULL;
          //Adafruit_SPIDevice *spi_dev = NULL;

          bool readRegister(uint8_t reg, uint16_t *data);
          bool writeRegister(uint8_t reg, uint16_t data);
          //bool _init(void);
          uint8_t transceive(uint8_t *txbuf, uint8_t txlen, uint8_t *rxbuf = NULL, uint8_t rxlen = 0, uint8_t interdelay = 10);

          enum mlx90395_gain _gain;
          enum mlx90395_resolution _res_x, _res_y, _res_z;
          enum mlx90395_filter _dig_filt;
          enum mlx90395_oversampling _osr;

          //int _cspin;
};

#endif
