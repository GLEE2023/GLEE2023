/*
* Description: This is an Arduino (C++) .cpp file required for the MLX90393 Magnetometer
* Project Info: Created For GLEE (The Great Lunar Expedition for Everyone)
* Library Author: Adafruit
* Library Contributors: Lawson Nerenberg, Matt Moran
* Resources Used in Creation:
* MLX90393 Datasheet
*
*/

#include "MLX90393.h"

/**
 * Constructor
 * Parameters: {Sensor ID, Debug Flag}
 * Returns: none
 * This is the constructor for the MLX90393 class. 
**/
MLX90393::MLX90393(int _id, bool _debug){
    MLX90393::info.id = _id;
    MLX90393::info.name = "MLX90393 Magnetometer";		// assign sensor name
    MLX90393::info.address = MLX90393_DEFAULT_ADDR;		// assign sensor i2c address
    MLX90393::sensorDebug= _debug;						// debug flag status
}


/**
 * Parameters: i2c_addr - the I2C address to be used, and wire - the Wire object to be used for I2C connections
 * Returns: True if initialization was successful, false if not
 * This function sets uo the hardware and initializes I2C
**/
bool MLX90393::begin_I2C(uint8_t i2c_addr, TwoWire *wire) {
    if (i2c_dev) {
        delete i2c_dev;
    }

    if (!i2c_dev) {
        i2c_dev = new Adafruit_I2CDevice(i2c_addr, wire);
    }
    //spi_dev = NULL;

    if (!i2c_dev->begin()) {
        return false;
    }

    //return _init();

    if (!exitMode())
        return false;

    if (!reset())
        return false;

    /* Set gain and sensor config. */
    if (!setGain(MLX90393_GAIN_1X)) {
        return false;
    }

    /* Set resolution. */
    if (!setResolution(MLX90393_X, MLX90393_RES_16))
        return false;
    if (!setResolution(MLX90393_Y, MLX90393_RES_16))
        return false;
    if (!setResolution(MLX90393_Z, MLX90393_RES_16))
        return false;

    /* Set oversampling. */
    if (!setOversampling(MLX90393_OSR_3))
        return false;

    /* Set digital filtering. */
    if (!setFilter(MLX90393_FILTER_7))
        return false;

    /* set INT pin to output interrupt */
    if (!setTrigInt(false)) {
        return false;
    }

    return true;
}

/*!
 *    @brief  Sets up the hardware and initializes hardware SPI
 *    @param  cs_pin The arduino pin # connected to chip select
 *    @param  theSPI The SPI object to be used for SPI connections.
 *    @return True if initialization was successful, otherwise false.
 */
/*boolean Adafruit_MLX90393::begin_SPI(uint8_t cs_pin, SPIClass *theSPI) {
  i2c_dev = NULL;
  if (!spi_dev) {
    _cspin = cs_pin;
    spi_dev = new Adafruit_SPIDevice(cs_pin,
                                     1000000,               // frequency
                                     SPI_BITORDER_MSBFIRST, // bit order
                                     SPI_MODE3,             // data mode
                                     theSPI);
  }
  if (!spi_dev->begin()) {
    return false;
  }
  return _init();
}*/

/**
 * Parameters: None
 * Returns: True if initialization was successful, false if not
 * This function...
**/

// bool MLX90393::_init(void) {
    
//     if (!exitMode())
//         return false;

//     if (!reset())
//         return false;

//     /* Set gain and sensor config. */
//     if (!setGain(MLX90393_GAIN_1X)) {
//         return false;
//     }

//     /* Set resolution. */
//     if (!setResolution(MLX90393_X, MLX90393_RES_16))
//         return false;
//     if (!setResolution(MLX90393_Y, MLX90393_RES_16))
//         return false;
//     if (!setResolution(MLX90393_Z, MLX90393_RES_16))
//         return false;

//     /* Set oversampling. */
//     if (!setOversampling(MLX90393_OSR_3))
//         return false;

//     /* Set digital filtering. */
//     if (!setFilter(MLX90393_FILTER_7))
//         return false;

//     /* set INT pin to output interrupt */
//     if (!setTrigInt(false)) {
//         return false;
//     }

//     return true;
// }


/**
 * Parameters: sensor - pointer to an Adafruit Unified sensor_t object
 * Returns: None
 * This function gets device data in the Adafruit Unified Sensor format
**/
void MLX90393::getSensor(sensor_t *sensor) {
    /* Clear the sensor_t object */
    memset(sensor, 0, sizeof(sensor_t));

    /* Insert the sensor name in the fixed length char array */
    strncpy(sensor->name, "MLX90393", sizeof(sensor->name) - 1);
    sensor->name[sizeof(sensor->name) - 1] = 0;
    sensor->version = 1;
    sensor->sensor_id = 90393;
    sensor->type = SENSOR_TYPE_MAGNETIC_FIELD;
    sensor->min_delay = 0;
    sensor->min_value = -50000; // -50 gauss in uTesla
    sensor->max_value = 50000;  // +50 gauss in uTesla
    sensor->resolution = 0.15;  // 100/16-bit uTesla per LSB
}

/**
 * Parameters: None
 * Returns: True if the exit mode operation completed sucessfully, false if not
 * This function exits the current mode.
**/
bool MLX90393::exitMode(void) {
    uint8_t tx[1] = {MLX90393_REG_EX};

    /* Perform the transaction. */
    return (transceive(tx, sizeof(tx), NULL, 0, 0) == MLX90393_STATUS_OK);
}

/**
 * Parameters: None
 * Returns: True if the soft reset was successful, false if not
 * This function performs a soft reset
**/
bool MLX90393::reset(void) {
    uint8_t tx[1] = {MLX90393_REG_RT};

    /* Perform the transaction. */
    if (transceive(tx, sizeof(tx), NULL, 0, 5) != MLX90393_STATUS_RESET) {
        return false;
    }
    return true;
}

/**
 * Parameters: gain - the gain level to set
 * Returns: True if successful, false if not
 * This functions sets the sensor gain to the specified level.
**/
bool MLX90393::setGain(mlx90393_gain_t gain) {
    uint8_t g;

    if(gain == MLX90393_GAIN_2_5X){
        g = 0x03;
    }

    if(gain == MLX90393_GAIN_1X){
        g = 0x07;
    }
    _gain = gain;

    uint16_t data;
    readRegister(MLX90393_CONF1, &data);

    Serial.print("CONF1 Reg: "); Serial.println(data);
    Serial.print("GAIN VAR: "); Serial.println(g);

    // mask off gain bits
    data &= ~0x0070;
    // set gain bits
    data |= g << MLX90393_GAIN_SHIFT;

    return writeRegister(MLX90393_CONF1, data);
}

/**
 * Parameters: None
 * Returns: enum of the custom type mlx90393_gain_t containing the current gain level
 * This function gets the current sensor gain.
**/
mlx90393_gain_t MLX90393::getGain(void) {
    uint16_t data;
    readRegister(MLX90393_CONF1, &data);

    // mask off gain bits
    data &= 0x0070;

    return (mlx90393_gain_t)(data >> 4);
}

/**
 * Parameters: axis - the axis to set, and resolution - the resolution level to set
 * Returns: True if successful, false if not
 * This function sets the sensor resolution to the specified level.
**/
bool MLX90393::setResolution(enum mlx90393_axis axis, enum mlx90393_resolution resolution) {
    uint16_t data;
    readRegister(MLX90393_CONF3, &data);

    switch (axis) {
    case MLX90393_X:
        _res_x = resolution;
        data &= ~0x0060;
        data |= resolution << 5;
        break;
    case MLX90393_Y:
        _res_y = resolution;
        data &= ~0x0180;
        data |= resolution << 7;
        break;
    case MLX90393_Z:
        _res_z = resolution;
        data &= ~0x0600;
        data |= resolution << 9;
        break;
    }

    return writeRegister(MLX90393_CONF3, data);
}

/**
 * Parameters: axis - the axis to get
 * Returns: an enum of the custom data type mlx90393_resolution containing the current resolution
 * This function gets the current sensor resolution
**/
enum mlx90393_resolution MLX90393::getResolution(enum mlx90393_axis axis) {
    switch (axis) {
    case MLX90393_X:
        return _res_x;
    case MLX90393_Y:
        return _res_y;
    case MLX90393_Z:
        return _res_z;
    }
}

/**
 * Parameters: filter - the digital filter setting
 * Returns: True if successful, false if not
 * This function sets the digital filter.
**/
bool MLX90393::setFilter(enum mlx90393_filter filter) {
    uint8_t f;

    if(filter == MLX90393_FILTER_6){
        f = 0x06;
    }

    if(filter == MLX90393_FILTER_7){
        f = 0x07;
    }
    
    _dig_filt = filter;

    uint16_t data;
    readRegister(MLX90393_CONF3, &data);

    data &= ~0x1C;
    data |= f << 2;

    return writeRegister(MLX90393_CONF3, data);
}
/**
 * Parameters: None
 * Returns: An enum of the custom type mlx90393_filter containing the current digital filter setting.
 * This function gets the current digital filter setting.
**/

enum mlx90393_filter MLX90393::getFilter(void) { return _dig_filt; }

/**
 * Parameters: oversampling - the oversampling value to use
 * Returns: True if successful, false if not
 * This function sets the oversampling value.
**/
bool MLX90393::setOversampling(enum mlx90393_oversampling oversampling) {
    _osr = oversampling;

    uint16_t data;
    readRegister(MLX90393_CONF3, &data);

    data &= ~0x03;
    data |= oversampling;

    return writeRegister(MLX90393_CONF3, data);
}

/**
 * Parameters: None
 * Returns: An enum of the custom type mlx90393_oversampling containing the current oversampling setting.
 * This function gets the current oversampling setting.
**/
enum mlx90393_oversampling MLX90393::getOversampling(void) { return _osr; }

// To-do
//bool MLX90393::getTemp(void){};

/**
 * Parameters: state - where true sets the pin to INT and false to TRIG
 * Returns: True if successful, false if not
 * This function sets the TRIG_INT pin to one of two specified functions.
**/
bool MLX90393::setTrigInt(bool state) {
    uint16_t data;
    readRegister(MLX90393_CONF2, &data);

    // mask off trigint bit
    data &= ~0x8000;

    // set trigint bit if desired
    if (state) {
        /* Set the INT, highest bit */
        data |= 0x8000;
    }

    return writeRegister(MLX90393_CONF2, data);
}

/**
 * Parameters: None
 * Returns: True if successful, false if not
 * This function starts a single measurement (single measurement mode) on all 3 axes.
**/
bool MLX90393::startSingleMeasurement(void) {
    uint8_t tx[1] = {MLX90393_REG_SM | MLX90393_AXIS_ALL};

    /* Set the device to single measurement mode */
    uint8_t stat = transceive(tx, sizeof(tx), NULL, 0, 0);
    if ((stat == MLX90393_STATUS_OK) || (stat == MLX90393_STATUS_SMMODE)) {
        return true;
    }
    return false;
}

/**
 * Parameters: Pointers to the floats x,y,z, which are readings of the 3 axes
 * Returns: The custom type sensor_float_vec_t containing the magnetic measurmeents
 * This function combines the 3 magnetic axes readings into one data type.
**/

/*
sensor_float_vec_t MLX90393::getMagnetic(float *x, float *y, float *z){
    sensor_float_vec_t magnetic;
    /*
    int32_t magX = *x;
    int32_t magY = *y;
    int32_t magZ = *z;

    magnetic.x = magX;
    magnetic.y = magY;
    magnetic.z = magZ;
    */
    /*
    magnetic.x = (int32_t) *x;
    magnetic.y = (int32_t) *y;
    magnetic.z = (int32_t) *z;

    return magnetic;
    
}
*/



/**
 * Parameters: Pointers to the floats x,y,z, which are readings of the 3 axes
 * Returns: True if successful, false if not
 * This function reads data from the data register and returns the readings.
**/
bool MLX90393::readMeasurement(float *x, float *y, float *z) {
    uint8_t tx[1] = {MLX90393_REG_RM | MLX90393_AXIS_ALL};
    uint8_t rx[6] = {0};

    /* Read a single data sample. */
    if (transceive(tx, sizeof(tx), rx, sizeof(rx), 0) != MLX90393_STATUS_OK) {
        return false;
    }

    int16_t xi, yi, zi;

    /* Convert data to uT and float. */
    xi = (rx[0] << 8) | rx[1];
    yi = (rx[2] << 8) | rx[3];
    zi = (rx[4] << 8) | rx[5];

    if (_res_x == MLX90393_RES_18)
        xi -= 0x8000;
    if (_res_x == MLX90393_RES_19)
        xi -= 0x4000;
    if (_res_y == MLX90393_RES_18)
        yi -= 0x8000;
    if (_res_y == MLX90393_RES_19)
        yi -= 0x4000;
    if (_res_z == MLX90393_RES_18)
        zi -= 0x8000;
    if (_res_z == MLX90393_RES_19)
        zi -= 0x4000;


    /*
    *x = (float)xi * mlx90393_lsb_lookup[0][_gain][_res_x][0];
    *y = (float)yi * mlx90393_lsb_lookup[0][_gain][_res_y][0];
    *z = (float)zi * mlx90393_lsb_lookup[0][_gain][_res_z][1];
    */

    *x = (float)xi * mlx90393_lsb_lookup[0][_gain][_res_x][0];
    *y = (float)yi * mlx90393_lsb_lookup[0][_gain][_res_y][0];
    *z = (float)zi * mlx90393_lsb_lookup[0][_gain][_res_z][1];

    return true;
}

/**
 * Parameters: Pointers to the floats x,y,z, which are readings of the 3 axes
 * Returns: True if successful, false if not
 * This function performs a X/Y/Z conversion and returns the readings
**/
bool MLX90393::readData(float *x, float *y, float *z) {
    if (!startSingleMeasurement())
        return false;
    // See MLX90393 Getting Started Guide for fancy formula
    // tconv = f(OSR, DIG_FILT, OSR2, ZYXT)
    // For now, using Table 18 from datasheet
    // Without +10ms delay measurement doesn't always seem to work
    delay(mlx90393_tconv[_dig_filt][_osr] + 10);
    return readMeasurement(x, y, z);
}

/**
 * Parameters: reg - the register being written to, and data - the data being written to the register
 * Returns: True if successful, false if not
 * This function writes data to registers.
**/
bool MLX90393::writeRegister(uint8_t reg, uint16_t data) {
    uint8_t tx[4] = {MLX90393_REG_WR,
                      data >> 8,   // high byte
                      data & 0xFF, // low byte
                      reg << 2};   // the register itself, shift up by 2 bits!

    /* Perform the transaction. */
    return (transceive(tx, sizeof(tx), NULL, 0, 0) == MLX90393_STATUS_OK);
}

/**
 * Parameters: reg - the register being read from, and data - a pointer to where the data is being stored
 * Returns: True if successful, false if not
 * This function reads data from registers.
**/
bool MLX90393::readRegister(uint8_t reg, uint16_t *data) {
    uint8_t tx[2] = {MLX90393_REG_RR,
                      reg << 2}; // the register itself, shift up by 2 bits!

    uint8_t rx[2];

    /* Perform the transaction. */
    if (transceive(tx, sizeof(tx), rx, sizeof(rx), 0) != MLX90393_STATUS_OK) {
        return false;
    }

    *data = ((uint16_t)rx[0] << 8) | rx[1];

    return true;
}
/**
 * Parameters: magnetic - magnetic axes readings as a sensor_float_vec_t
 * Returns: magnetic field strength as a float
 * This function calculates the magnetic field strength based on the individual magnetic readings of all 3 axes.
**/
/*
float MLX90393::getMagFieldStrength(sensor_float_vec_t magnetic){
    return sqrt(pow(magnetic.x,2) + pow(magnetic.y,2) + pow(magnetic.z,2)); // L2 norm
}
*/

/**
 * Parameters: None
 * Returns: Sample as the custom type mlx_sample_t
 * This function gets the magnetic readings and magnetic field strength and returns them as a part of the mlx_sample_t data type.
**/
mlx_sample_t MLX90393::getSample(void){
    float x,y,z;
    mlx_sample_t sample;
    
    //MLX90393::setOpMode(true, LOW_POWER_1, SINGLE_MEASURE); // Only single measurement mode is currently supported
    //MLX90393::startSingleMeasurement();
    //delay(5); // Wait for single measurement to be taken

    if(MLX90393::readData(&x,&y,&z)){
        //MLX90393::setOpMode(true, LOW_POWER_1, POWER_DOWN); // No true equivalent function
        MLX90393::exitMode();

        /*
        sample.magnetic = MLX90393::getMagnetic(&x,&y,&z);
        sample.strength = MLX90393::getMagFieldStrength(sample.magnetic);
        */

        sample.magnetic.x = x;
        sample.magnetic.y = y;
        sample.magnetic.z = z;
        sample.strength = sqrt(pow(sample.magnetic.x,2) + pow(sample.magnetic.y,2) + pow(sample.magnetic.z,2));
    }else{
        Serial.println(F("Waiting for data to get ready"));
    }

    return sample;
}   

/**
 * Parameters: txbuf - a pointer to the buffer containing the data to write, txlen - the number of bytes to write,
 * rxbuf - a pointer to a large bugger where data read back will be written, 
 * rxlen - the number of bytes to read back (not including the status byte), and interdelay - a delay value
 * Returns: The status byte from the IC as the type uint8_t
 * This function performs a read/write transcation with the sensor
**/
uint8_t MLX90393::transceive(uint8_t *txbuf, uint8_t txlen, uint8_t *rxbuf, uint8_t rxlen, uint8_t interdelay) {
    uint8_t status = 0;
    uint8_t i;
    uint8_t rxbuf2[rxlen + 2];

    if (i2c_dev) {
        /* Write stage */
        if (!i2c_dev->write(txbuf, txlen)) {
            return MLX90393_STATUS_ERROR;
        }
        delay(interdelay);

        /* Read status byte plus any others */
        if (!i2c_dev->read(rxbuf2, rxlen + 1)) {
            return MLX90393_STATUS_ERROR;
        }
        status = rxbuf2[0];
        for (i = 0; i < rxlen; i++) {
            rxbuf[i] = rxbuf2[i + 1];
        }
    }
    /*
    if (spi_dev) {
        spi_dev->write_then_read(txbuf, txlen, rxbuf2, rxlen + 1, 0x00);
        status = rxbuf2[0];
        for (i = 0; i < rxlen; i++) {
            rxbuf[i] = rxbuf2[i + 1];
        }
        delay(interdelay);
    }
    */

    /* Mask out bytes available in the status response. */
    return (status >> 2);
}