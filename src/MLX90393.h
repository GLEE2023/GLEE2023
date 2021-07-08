/**
 * Description: This is an Arduino (C++) Header file required for the MLX90393 Magnetometer
 * Project Info: Created For GLEE (The Great Lunar Expedition for Everyone)
 * Library Contributors: John Walker Johnson, Lawson Nerenberg
 * Primary Resources Used in Creation:
 * MLX90393 Datasheet (https://media.digikey.com/pdf/Data%20Sheets/AKM%20Semiconductor%20Inc.%20PDFs/MLX90393_Prelim_DS_11-2-18.pdf)
 * Arduino Wire Library Reference Guide
 *
**/

#ifndef MLX90393_H
#define MLX90393_H

#include "GLEE_Sensor.h"

//Registers

//#define MLX90393_DEFAULT_ADDR (0x0C) /* Can also be 0x18, depending on IC */

#define MLX90393_AXIS_ALL (0x0E)      /**< X+Y+Z axis bits for commands. */
#define MLX90393_CONF1 (0x00)         /**< Gain */
#define MLX90393_CONF2 (0x01)         /**< Burst, comm mode */
#define MLX90393_CONF3 (0x02)         /**< Oversampling, filter, res. */
#define MLX90393_CONF4 (0x03)         /**< Sensitivty drift. */
#define MLX90393_GAIN_SHIFT (4)       /**< Left-shift for gain bits. */
#define MLX90393_HALL_CONF (0x0C)     /**< Hall plate spinning rate adj. */
#define MLX90393_STATUS_OK (0x00)     /**< OK value for status response. */
#define MLX90393_STATUS_SMMODE (0x08) /**< SM Mode status response. */
#define MLX90393_STATUS_RESET (0x01)  /**< Reset value for status response. */
#define MLX90393_STATUS_ERROR (0xFF)  /**< OK value for status response. */
#define MLX90393_STATUS_MASK (0xFC)   /**< Mask for status OK checks. */

/** Register map. */
enum {
  MLX90393_REG_SB = (0x10),  /**< Start burst mode. */
  MLX90393_REG_SW = (0x20),  /**< Start wakeup on change mode. */
  MLX90393_REG_SM = (0x30),  /**> Start single-meas mode. */
  MLX90393_REG_RM = (0x40),  /**> Read measurement. */
  MLX90393_REG_RR = (0x50),  /**< Read register. */
  MLX90393_REG_WR = (0x60),  /**< Write register. */
  MLX90393_REG_EX = (0x80),  /**> Exit moode. */
  MLX90393_REG_HR = (0xD0),  /**< Memory recall. */
  MLX90393_REG_HS = (0x70),  /**< Memory store. */
  MLX90393_REG_RT = (0xF0),  /**< Reset. */
  MLX90393_REG_NOP = (0x00), /**< NOP. */
};

//#define MLX90393_WAI_1 0x00		    // WAI Company ID (Typically 0x48)
//#define MLX90393_WAI_2 0x01		    // WAI Device ID (Typically)

//#define MLX90393_ST_1 0x10			// Status Register - Data Status 1
//#define MLX90393_HX 0x11				// Magnetic Field Measurement Data - X-Axis Low Byte
//#define MLX90393_HY 0x14				// Magnetic Field Measurement Data - Y-Axis Low Byte
//#define MLX90393_HZ 0x17				// Magnetic Field Measurement Data - Z-Axis Low Byte
//#define MLX90393_TMPS 0x1A			// Temperature Measurement Data

//#define MLX90393_ST_2 0x1B		    // Status Register 2

// Control settings registers (all 1xByte)
//#define MLX90393_CNTL_1 0x30				// Watermark settings 		3 bit value D[0:2] = [WM[2],WM[1],WM[0]] (page 34 datasheet)
//#define MLX90393_CNTL_2 0x31				// Temperature settings 	1 bit value D[6] = TEM[0] (temperature enable)
//#define MLX90393_CNTL_3 0x32				// Operational settings 	8 bit value D[0:7] = [FIFO, MT[1], MT[0], MODE[4], MODE[3], MODE[2], MODE[1], MODE[0]]
//#define MLX90393_CNTL_4 0x33				// Reset control			1 bit value D[0] = SRST (soft reset)

// MLX90393 Sensor ADDRESS as given by data sheet: 0x0C
#define MLX90393_DEFAULT_ADDR 0x0C				

// The sensor Class defines the following data types
// This structure represents raw sensor readings: temp, and magnetic x,y,z output 

/** Gain settings for CONF1 register. */
typedef enum mlx90393_gain {
  MLX90393_GAIN_5X = (0x00),
  MLX90393_GAIN_4X,
  MLX90393_GAIN_3X,
  MLX90393_GAIN_2_5X,
  MLX90393_GAIN_2X,
  MLX90393_GAIN_1_67X,
  MLX90393_GAIN_1_33X,
  MLX90393_GAIN_1X
} mlx90393_gain_t;

/** Resolution settings for CONF3 register. */
typedef enum mlx90393_resolution {
  MLX90393_RES_16,
  MLX90393_RES_17,
  MLX90393_RES_18,
  MLX90393_RES_19,
} mlx90393_resolution_t;

/** Axis designator. */
typedef enum mlx90393_axis {
  MLX90393_X,
  MLX90393_Y,
  MLX90393_Z
} mlx90393_axis_t;

/** Digital filter settings for CONF3 register. */
typedef enum mlx90393_filter {
  MLX90393_FILTER_0,
  MLX90393_FILTER_1,
  MLX90393_FILTER_2,
  MLX90393_FILTER_3,
  MLX90393_FILTER_4,
  MLX90393_FILTER_5,
  MLX90393_FILTER_6,
  MLX90393_FILTER_7,
} mlx90393_filter_t;

/** Oversampling settings for CONF3 register. */
typedef enum mlx90393_oversampling {
  MLX90393_OSR_0,
  MLX90393_OSR_1,
  MLX90393_OSR_2,
  MLX90393_OSR_3,
} mlx90393_oversampling_t;

/** Lookup table to convert raw values to uT based on [HALLCONF][GAIN_SEL][RES].
 */
const float mlx90393_lsb_lookup[2][8][4][2] = {

    /* HALLCONF = 0xC (default) */
    {
        /* GAIN_SEL = 0, 5x gain */
        {{0.751, 1.210}, {1.502, 2.420}, {3.004, 4.840}, {6.009, 9.680}},
        /* GAIN_SEL = 1, 4x gain */
        {{0.601, 0.968}, {1.202, 1.936}, {2.403, 3.872}, {4.840, 7.744}},
        /* GAIN_SEL = 2, 3x gain */
        {{0.451, 0.726}, {0.901, 1.452}, {1.803, 2.904}, {3.605, 5.808}},
        /* GAIN_SEL = 3, 2.5x gain */
        {{0.376, 0.605}, {0.751, 1.210}, {1.502, 2.420}, {3.004, 4.840}},
        /* GAIN_SEL = 4, 2x gain */
        {{0.300, 0.484}, {0.601, 0.968}, {1.202, 1.936}, {2.403, 3.872}},
        /* GAIN_SEL = 5, 1.667x gain */
        {{0.250, 0.403}, {0.501, 0.807}, {1.001, 1.613}, {2.003, 3.227}},
        /* GAIN_SEL = 6, 1.333x gain */
        {{0.200, 0.323}, {0.401, 0.645}, {0.801, 1.291}, {1.602, 2.581}},
        /* GAIN_SEL = 7, 1x gain */
        {{0.150, 0.242}, {0.300, 0.484}, {0.601, 0.968}, {1.202, 1.936}},
    },

    /* HALLCONF = 0x0 */
    {
        /* GAIN_SEL = 0, 5x gain */
        {{0.787, 1.267}, {1.573, 2.534}, {3.146, 5.068}, {6.292, 10.137}},
        /* GAIN_SEL = 1, 4x gain */
        {{0.629, 1.014}, {1.258, 2.027}, {2.517, 4.055}, {5.034, 8.109}},
        /* GAIN_SEL = 2, 3x gain */
        {{0.472, 0.760}, {0.944, 1.521}, {1.888, 3.041}, {3.775, 6.082}},
        /* GAIN_SEL = 3, 2.5x gain */
        {{0.393, 0.634}, {0.787, 1.267}, {1.573, 2.534}, {3.146, 5.068}},
        /* GAIN_SEL = 4, 2x gain */
        {{0.315, 0.507}, {0.629, 1.014}, {1.258, 2.027}, {2.517, 4.055}},
        /* GAIN_SEL = 5, 1.667x gain */
        {{0.262, 0.422}, {0.524, 0.845}, {1.049, 1.689}, {2.097, 3.379}},
        /* GAIN_SEL = 6, 1.333x gain */
        {{0.210, 0.338}, {0.419, 0.676}, {0.839, 1.352}, {1.678, 2.703}},
        /* GAIN_SEL = 7, 1x gain */
        {{0.157, 0.253}, {0.315, 0.507}, {0.629, 1.014}, {1.258, 2.027}},
    }};

/** Lookup table for conversion time based on [DIF_FILT][OSR].
 */
const float mlx90393_tconv[8][4] = {
    /* DIG_FILT = 0 */
    {1.27, 1.84, 3.00, 5.30},
    /* DIG_FILT = 1 */
    {1.46, 2.23, 3.76, 6.84},
    /* DIG_FILT = 2 */
    {1.84, 3.00, 5.30, 9.91},
    /* DIG_FILT = 3 */
    {2.61, 4.53, 8.37, 16.05},
    /* DIG_FILT = 4 */
    {4.15, 7.60, 14.52, 28.34},
    /* DIG_FILT = 5 */
    {7.22, 13.75, 26.80, 52.92},
    /* DIG_FILT = 6 */
    {13.36, 26.04, 51.38, 102.07},
    /* DIF_FILT = 7 */
    {25.65, 50.61, 100.53, 200.37},
};

typedef struct{
    sensor_float_vec_t magnetic;
    float strength;
}AK_Sample_t;

/*
typedef struct{
    uint32_t x;	
    uint32_t y;
    uint32_t z;
    uint8_t temp;	//Temperature output - Temperature byte
}AK_RawData_t;

// Sensor Modes From DataSheet
enum MLX90393_Measurement_Mode_t{
    POWER_DOWN = 0b00000,						// These sensor modes and values
    SINGLE_MEASURE = 0b00001,					// represent bits [4:0] of the CNTL3 Byte
    CONT_MEASURE_1 = 0b00010,
    CONT_MEASURE_2 = 0b00100,
    CONT_MEASURE_3 = 0b00110,
    CONT_MEASURE_4 = 0b01000,
    CONT_MEASURE_5 = 0b01010,
    CONT_MEASURE_6 = 0b01100,
    SELF_TEST = 0b10000,
};

// Drive mode options from data sheet
// TODO: Write readme documentation for drive mode (what each bit represents)
enum MLX90393_Drive_Mode_t{
	LOW_POWER_1 = 0b00,
	LOW_POWER_2 = 0b01,
	LOW_NOISE_1 = 0b10,
	LOW_NOISE_2 = 0b11,
};*/

// Primary Magnetometor class inharents parent sensor class variables and methods
class MLX90393: public Sensor{
	// Initialization for Sensor Data, Sensor Info and Data Structures 
	public:
		MLX90393(int _id, bool _debug = false);

        /*void begin(void);
        void readWAI(void);
        void setOpMode(bool fifo, MLX90393_Drive_Mode_t driveMode, MLX90393_Measurement_Mode_t measureMode);
        bool dataReady(void);

        // Getters
        AK_RawData_t getRawData(void);
        
        float getTemperature(uint8_t rawTemp);
        sensor_float_vec_t getMagnetic(AK_RawData_t rawData);
        float getMagFieldStrength(sensor_float_vec_t magnetic);
        */
        AK_Sample_t getSample(void);

        bool begin_I2C(uint8_t i2c_addr = MLX90393_DEFAULT_ADDR, TwoWire *wire = &Wire);
        bool begin_SPI(uint8_t cs_pin, SPIClass *theSPI = &SPI);

        bool reset(void);
        bool exitMode(void);

        bool readMeasurement(float *x, float *y, float *z);
        bool startSingleMeasurement(void);

        bool setGain(enum mlx90393_gain gain);
        enum mlx90393_gain getGain(void);

        bool setResolution(enum mlx90393_axis, enum mlx90393_resolution resolution);
        enum mlx90393_resolution getResolution(enum mlx90393_axis);

        bool setFilter(enum mlx90393_filter filter);
        enum mlx90393_filter getFilter(void);

        bool setOversampling(enum mlx90393_oversampling oversampling);
        enum mlx90393_oversampling getOversampling(void);

        bool setTrigInt(bool state);
        bool readData(float *x, float *y, float *z);

        bool getEvent(sensors_event_t *event);
        void getSensor(sensor_t *sensor);

    private:
        /*bool FIFO;
        uint8_t watermarkLevel;  
        MLX90393_Measurement_Mode_t measurementMode;
        MLX90393_Drive_Mode_t driveMode;*/

        Adafruit_I2CDevice *i2c_dev = NULL;
        Adafruit_SPIDevice *spi_dev = NULL;

        bool readRegister(uint8_t reg, uint16_t *data);
        bool writeRegister(uint8_t reg, uint16_t data);
        bool _init(void);
        uint8_t transceive(uint8_t *txbuf, uint8_t txlen, uint8_t *rxbuf = NULL,
                            uint8_t rxlen = 0, uint8_t interdelay = 10);

        enum mlx90393_gain _gain;
        enum mlx90393_resolution _res_x, _res_y, _res_z;
        enum mlx90393_filter _dig_filt;
        enum mlx90393_oversampling _osr;

        int32_t _sensorID = 90393;
        int _cspin;
};

#endif
