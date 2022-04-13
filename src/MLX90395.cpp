// /*
// * Description: This is an Arduino (C++) .cpp file required for the MLX90395 Magnetometer
// * Project Info: Created For GLEE (The Great Lunar Expedition for Everyone)
// * Library Author: Adafruit
// * Library Contributors: Lawson Nerenberg, Matt Moran
// * Resources Used in Creation:
// * MLX90395 Datasheet
// *
// */

// #include "MLX90395.h"

// /**
//  * Constructor
//  * Parameters: {Sensor ID, Debug Flag}
//  * Returns: none
//  * This is the constructor for the MLX90395 class.
// **/
// MLX90395::MLX90395(int _id, bool _debug){
//     MLX90395::info.id = _id;
//     MLX90395::info.name = "MLX90395 Magnetometer";		// assign sensor name
//     MLX90395::info.address = MLX90395_DEFAULT_ADDR;		// assign sensor i2c address
//     MLX90395::sensorDebug= _debug;						// debug flag status
// }


// /**
//  * Parameters: i2c_addr - the I2C address to be used, and wire - the Wire object to be used for I2C connections
//  * Returns: True if initialization was successful, false if not
//  * This function sets uo the hardware and initializes I2C
// **/
// bool MLX90395::begin_I2C(uint8_t i2c_addr, TwoWire *wire) {
//     if (i2c_dev) {
//         delete i2c_dev;
//     }

//     if (!i2c_dev) {
//         i2c_dev = new Adafruit_I2CDevice(i2c_addr, wire);
//     }
//     //spi_dev = NULL;

//     if (!i2c_dev->begin()) {
//         return false;
//     }

//     //return _init();

//     if (!exitMode())
//         return false;

//     if (!reset())
//         return false;

//     /* Set gain and sensor config. */
//     if (!setGain(MLX90395_GAIN_1X)) {
//         return false;
//     }

//     /* Set resolution. */
//     if (!setResolution(MLX90395_X, MLX90395_RES_16))
//         return false;
//     if (!setResolution(MLX90395_Y, MLX90395_RES_16))
//         return false;
//     if (!setResolution(MLX90395_Z, MLX90395_RES_16))
//         return false;

//     /* Set oversampling. */
//     if (!setOversampling(MLX90395_OSR_3))
//         return false;

//     /* Set digital filtering. */
//     if (!setFilter(MLX90395_FILTER_7))
//         return false;

//     /* set INT pin to output interrupt */
//     if (!setTrigInt(false)) {
//         return false;
//     }

//     return true;
// }

// /*!
//  *    @brief  Sets up the hardware and initializes hardware SPI
//  *    @param  cs_pin The arduino pin # connected to chip select
//  *    @param  theSPI The SPI object to be used for SPI connections.
//  *    @return True if initialization was successful, otherwise false.
//  */
// /*boolean Adafruit_MLX90395::begin_SPI(uint8_t cs_pin, SPIClass *theSPI) {
//   i2c_dev = NULL;
//   if (!spi_dev) {
//     _cspin = cs_pin;
//     spi_dev = new Adafruit_SPIDevice(cs_pin,
//                                      1000000,               // frequency
//                                      SPI_BITORDER_MSBFIRST, // bit order
//                                      SPI_MODE3,             // data mode
//                                      theSPI);
//   }
//   if (!spi_dev->begin()) {
//     return false;
//   }
//   return _init();
// }*/

// /**
//  * Parameters: None
//  * Returns: True if initialization was successful, false if not
//  * This function...
// **/

// // bool MLX90395::_init(void) {

// //     if (!exitMode())
// //         return false;

// //     if (!reset())
// //         return false;

// //     /* Set gain and sensor config. */
// //     if (!setGain(MLX90395_GAIN_1X)) {
// //         return false;
// //     }

// //     /* Set resolution. */
// //     if (!setResolution(MLX90395_X, MLX90395_RES_16))
// //         return false;
// //     if (!setResolution(MLX90395_Y, MLX90395_RES_16))
// //         return false;
// //     if (!setResolution(MLX90395_Z, MLX90395_RES_16))
// //         return false;

// //     /* Set oversampling. */
// //     if (!setOversampling(MLX90395_OSR_3))
// //         return false;

// //     /* Set digital filtering. */
// //     if (!setFilter(MLX90395_FILTER_7))
// //         return false;

// //     /* set INT pin to output interrupt */
// //     if (!setTrigInt(false)) {
// //         return false;
// //     }

// //     return true;
// // }


// /**
//  * Parameters: sensor - pointer to an Adafruit Unified sensor_t object
//  * Returns: None
//  * This function gets device data in the Adafruit Unified Sensor format
// **/
// void MLX90395::getSensor(sensor_t *sensor) {
//     /* Clear the sensor_t object */
//     memset(sensor, 0, sizeof(sensor_t));

//     /* Insert the sensor name in the fixed length char array */
//     strncpy(sensor->name, "MLX90395", sizeof(sensor->name) - 1);
//     sensor->name[sizeof(sensor->name) - 1] = 0;
//     sensor->version = 1;
//     sensor->sensor_id = 90395;
//     sensor->type = SENSOR_TYPE_MAGNETIC_FIELD;
//     sensor->min_delay = 0;
//     sensor->min_value = -50000; // -50 gauss in uTesla
//     sensor->max_value = 50000;  // +50 gauss in uTesla
//     sensor->resolution = 0.15;  // 100/16-bit uTesla per LSB
// }

// /**
//  * Parameters: None
//  * Returns: True if the exit mode operation completed sucessfully, false if not
//  * This function exits the current mode.
// **/
// bool MLX90395::exitMode(void) {
//     uint8_t tx[1] = {MLX90395_REG_EX};

//     /* Perform the transaction. */
//     return (transceive(tx, sizeof(tx), NULL, 0, 0) == MLX90395_STATUS_OK);
// }

// /**
//  * Parameters: None
//  * Returns: True if the soft reset was successful, false if not
//  * This function performs a soft reset
// **/
// bool MLX90395::reset(void) {
//     uint8_t tx[1] = {MLX90395_REG_RT};

//     /* Perform the transaction. */
//     if (transceive(tx, sizeof(tx), NULL, 0, 5) != MLX90395_STATUS_RESET) {
//         return false;
//     }
//     return true;
// }

// /**
//  * Parameters: gain - the gain level to set
//  * Returns: True if successful, false if not
//  * This functions sets the sensor gain to the specified level.
// **/
// bool MLX90395::setGain(mlx90395_gain_t gain) {
//     uint8_t g;

//     if(gain == MLX90395_GAIN_2_5X){
//         g = 0x03;
//     }

//     if(gain == MLX90395_GAIN_1X){
//         g = 0x07;
//     }
//     _gain = gain;

//     uint16_t data;
//     readRegister(MLX90395_CONF1, &data);

//     Serial.print("CONF1 Reg: "); Serial.println(data);
//     Serial.print("GAIN VAR: "); Serial.println(g);

//     // mask off gain bits
//     data &= ~0x0070;
//     // set gain bits
//     data |= g << MLX90395_GAIN_SHIFT;

//     return writeRegister(MLX90395_CONF1, data);
// }

// /**
//  * Parameters: None
//  * Returns: enum of the custom type mlx90395_gain_t containing the current gain level
//  * This function gets the current sensor gain.
// **/
// mlx90395_gain_t MLX90395::getGain(void) {
//     uint16_t data;
//     readRegister(MLX90395_CONF1, &data);

//     // mask off gain bits
//     data &= 0x0070;

//     return (mlx90395_gain_t)(data >> 4);
// }

// /**
//  * Parameters: axis - the axis to set, and resolution - the resolution level to set
//  * Returns: True if successful, false if not
//  * This function sets the sensor resolution to the specified level.
// **/
// bool MLX90395::setResolution(enum mlx90395_axis axis, enum mlx90395_resolution resolution) {
//     uint16_t data;
//     readRegister(MLX90395_CONF3, &data);

//     switch (axis) {
//     case MLX90395_X:
//         _res_x = resolution;
//         data &= ~0x0060;
//         data |= resolution << 5;
//         break;
//     case MLX90395_Y:
//         _res_y = resolution;
//         data &= ~0x0180;
//         data |= resolution << 7;
//         break;
//     case MLX90395_Z:
//         _res_z = resolution;
//         data &= ~0x0600;
//         data |= resolution << 9;
//         break;
//     }

//     return writeRegister(MLX90395_CONF3, data);
// }

// /**
//  * Parameters: axis - the axis to get
//  * Returns: an enum of the custom data type mlx90395_resolution containing the current resolution
//  * This function gets the current sensor resolution
// **/
// enum mlx90395_resolution MLX90395::getResolution(enum mlx90395_axis axis) {
//     switch (axis) {
//     case MLX90395_X:
//         return _res_x;
//     case MLX90395_Y:
//         return _res_y;
//     case MLX90395_Z:
//         return _res_z;
//     }
// }

// /**
//  * Parameters: filter - the digital filter setting
//  * Returns: True if successful, false if not
//  * This function sets the digital filter.
// **/
// bool MLX90395::setFilter(enum mlx90395_filter filter) {
//     uint8_t f;

//     if(filter == MLX90395_FILTER_6){
//         f = 0x06;
//     }

//     if(filter == MLX90395_FILTER_7){
//         f = 0x07;
//     }

//     _dig_filt = filter;

//     uint16_t data;
//     readRegister(MLX90395_CONF3, &data);

//     data &= ~0x1C;
//     data |= f << 2;

//     return writeRegister(MLX90395_CONF3, data);
// }
// /**
//  * Parameters: None
//  * Returns: An enum of the custom type mlx90395_filter containing the current digital filter setting.
//  * This function gets the current digital filter setting.
// **/

// enum mlx90395_filter MLX90395::getFilter(void) { return _dig_filt; }

// /**
//  * Parameters: oversampling - the oversampling value to use
//  * Returns: True if successful, false if not
//  * This function sets the oversampling value.
// **/
// bool MLX90395::setOversampling(enum mlx90395_oversampling oversampling) {
//     _osr = oversampling;

//     uint16_t data;
//     readRegister(MLX90395_CONF3, &data);

//     data &= ~0x03;
//     data |= oversampling;

//     return writeRegister(MLX90395_CONF3, data);
// }

// /**
//  * Parameters: None
//  * Returns: An enum of the custom type mlx90395_oversampling containing the current oversampling setting.
//  * This function gets the current oversampling setting.
// **/
// enum mlx90395_oversampling MLX90395::getOversampling(void) { return _osr; }

// // To-do
// //bool MLX90395::getTemp(void){};

// /**
//  * Parameters: state - where true sets the pin to INT and false to TRIG
//  * Returns: True if successful, false if not
//  * This function sets the TRIG_INT pin to one of two specified functions.
// **/
// bool MLX90395::setTrigInt(bool state) {
//     uint16_t data;
//     readRegister(MLX90395_CONF2, &data);

//     // mask off trigint bit
//     data &= ~0x8000;

//     // set trigint bit if desired
//     if (state) {
//         /* Set the INT, highest bit */
//         data |= 0x8000;
//     }

//     return writeRegister(MLX90395_CONF2, data);
// }

// /**
//  * Parameters: None
//  * Returns: True if successful, false if not
//  * This function starts a single measurement (single measurement mode) on all 3 axes.
// **/
// bool MLX90395::startSingleMeasurement(void) {
//     uint8_t tx[1] = {MLX90395_REG_SM | MLX90395_AXIS_ALL};

//     /* Set the device to single measurement mode */
//     uint8_t stat = transceive(tx, sizeof(tx), NULL, 0, 0);
//     if ((stat == MLX90395_STATUS_OK) || (stat == MLX90395_STATUS_SMMODE)) {
//         return true;
//     }
//     return false;
// }

// /**
//  * Parameters: Pointers to the floats x,y,z, which are readings of the 3 axes
//  * Returns: The custom type sensor_float_vec_t containing the magnetic measurmeents
//  * This function combines the 3 magnetic axes readings into one data type.
// **/

// /*
// sensor_float_vec_t MLX90395::getMagnetic(float *x, float *y, float *z){
//     sensor_float_vec_t magnetic;
//     /*
//     int32_t magX = *x;
//     int32_t magY = *y;
//     int32_t magZ = *z;
//     magnetic.x = magX;
//     magnetic.y = magY;
//     magnetic.z = magZ;
//     */
//     /*
//     magnetic.x = (int32_t) *x;
//     magnetic.y = (int32_t) *y;
//     magnetic.z = (int32_t) *z;
//     return magnetic;

// }
// */



// /**
//  * Parameters: Pointers to the floats x,y,z, which are readings of the 3 axes
//  * Returns: True if successful, false if not
//  * This function reads data from the data register and returns the readings.
// **/
// bool MLX90395::readMeasurement(float *x, float *y, float *z) {
//     uint8_t tx[1] = {MLX90395_REG_RM | MLX90395_AXIS_ALL};
//     uint8_t rx[6] = {0};

//     /* Read a single data sample. */
//     if (transceive(tx, sizeof(tx), rx, sizeof(rx), 0) != MLX90395_STATUS_OK) {
//         return false;
//     }

//     int16_t xi, yi, zi;

//     /* Convert data to uT and float. */
//     xi = (rx[0] << 8) | rx[1];
//     yi = (rx[2] << 8) | rx[3];
//     zi = (rx[4] << 8) | rx[5];

//     if (_res_x == MLX90395_RES_18)
//         xi -= 0x8000;
//     if (_res_x == MLX90395_RES_19)
//         xi -= 0x4000;
//     if (_res_y == MLX90395_RES_18)
//         yi -= 0x8000;
//     if (_res_y == MLX90395_RES_19)
//         yi -= 0x4000;
//     if (_res_z == MLX90395_RES_18)
//         zi -= 0x8000;
//     if (_res_z == MLX90395_RES_19)
//         zi -= 0x4000;


//     /*
//     *x = (float)xi * mlx90395_lsb_lookup[0][_gain][_res_x][0];
//     *y = (float)yi * mlx90395_lsb_lookup[0][_gain][_res_y][0];
//     *z = (float)zi * mlx90395_lsb_lookup[0][_gain][_res_z][1];
//     */

//     *x = (float)xi * mlx90395_lsb_lookup[0][_gain][_res_x][0];
//     *y = (float)yi * mlx90395_lsb_lookup[0][_gain][_res_y][0];
//     *z = (float)zi * mlx90395_lsb_lookup[0][_gain][_res_z][1];

//     return true;
// }

// /**
//  * Parameters: Pointers to the floats x,y,z, which are readings of the 3 axes
//  * Returns: True if successful, false if not
//  * This function performs a X/Y/Z conversion and returns the readings
// **/
// bool MLX90395::readData(float *x, float *y, float *z) {
//     if (!startSingleMeasurement())
//         return false;
//     // See MLX90395 Getting Started Guide for fancy formula
//     // tconv = f(OSR, DIG_FILT, OSR2, ZYXT)
//     // For now, using Table 18 from datasheet
//     // Without +10ms delay measurement doesn't always seem to work
//     delay(mlx90395_tconv[_dig_filt][_osr] + 10);
//     return readMeasurement(x, y, z);
// }

// /**
//  * Parameters: reg - the register being written to, and data - the data being written to the register
//  * Returns: True if successful, false if not
//  * This function writes data to registers.
// **/
// bool MLX90395::writeRegister(uint8_t reg, uint16_t data) {
//     uint8_t tx[4] = {MLX90395_REG_WR,
//                       data >> 8,   // high byte
//                       data & 0xFF, // low byte
//                       reg << 2};   // the register itself, shift up by 2 bits!

//     /* Perform the transaction. */
//     return (transceive(tx, sizeof(tx), NULL, 0, 0) == MLX90395_STATUS_OK);
// }

// /**
//  * Parameters: reg - the register being read from, and data - a pointer to where the data is being stored
//  * Returns: True if successful, false if not
//  * This function reads data from registers.
// **/
// bool MLX90395::readRegister(uint8_t reg, uint16_t *data) {
//     uint8_t tx[2] = {MLX90395_REG_RR,
//                       reg << 2}; // the register itself, shift up by 2 bits!

//     uint8_t rx[2];

//     /* Perform the transaction. */
//     if (transceive(tx, sizeof(tx), rx, sizeof(rx), 0) != MLX90395_STATUS_OK) {
//         return false;
//     }

//     *data = ((uint16_t)rx[0] << 8) | rx[1];

//     return true;
// }
// /**
//  * Parameters: magnetic - magnetic axes readings as a sensor_float_vec_t
//  * Returns: magnetic field strength as a float
//  * This function calculates the magnetic field strength based on the individual magnetic readings of all 3 axes.
// **/
// /*
// float MLX90395::getMagFieldStrength(sensor_float_vec_t magnetic){
//     return sqrt(pow(magnetic.x,2) + pow(magnetic.y,2) + pow(magnetic.z,2)); // L2 norm
// }
// */

// /**
//  * Parameters: None
//  * Returns: Sample as the custom type mlx_sample_t
//  * This function gets the magnetic readings and magnetic field strength and returns them as a part of the mlx_sample_t data type.
// **/
// mlx_sample_t MLX90395::getSample(void){
//     float x,y,z;
//     mlx_sample_t sample;

//     //MLX90395::setOpMode(true, LOW_POWER_1, SINGLE_MEASURE); // Only single measurement mode is currently supported
//     //MLX90395::startSingleMeasurement();
//     //delay(5); // Wait for single measurement to be taken

//     if(MLX90395::readData(&x,&y,&z)){
//         //MLX90395::setOpMode(true, LOW_POWER_1, POWER_DOWN); // No true equivalent function
//         MLX90395::exitMode();

//         /*
//         sample.magnetic = MLX90395::getMagnetic(&x,&y,&z);
//         sample.strength = MLX90395::getMagFieldStrength(sample.magnetic);
//         */

//         sample.magnetic.x = x;
//         sample.magnetic.y = y;
//         sample.magnetic.z = z;
//         //sample.strength = sqrt(pow(sample.magnetic.x,2) + pow(sample.magnetic.y,2) + pow(sample.magnetic.z,2));
//     }else{
//         Serial.println(F("Waiting for data to get ready"));
//     }

//     return sample;
// }

// /**
//  * Parameters: txbuf - a pointer to the buffer containing the data to write, txlen - the number of bytes to write,
//  * rxbuf - a pointer to a large bugger where data read back will be written,
//  * rxlen - the number of bytes to read back (not including the status byte), and interdelay - a delay value
//  * Returns: The status byte from the IC as the type uint8_t
//  * This function performs a read/write transcation with the sensor
// **/
// uint8_t MLX90395::transceive(uint8_t *txbuf, uint8_t txlen, uint8_t *rxbuf, uint8_t rxlen, uint8_t interdelay) {
//     uint8_t status = 0;
//     uint8_t i;
//     uint8_t rxbuf2[rxlen + 2];

//     if (i2c_dev) {
//         /* Write stage */
//         if (!i2c_dev->write(txbuf, txlen)) {
//             return MLX90395_STATUS_ERROR;
//         }
//         delay(interdelay);

//         /* Read status byte plus any others */
//         if (!i2c_dev->read(rxbuf2, rxlen + 1)) {
//             return MLX90395_STATUS_ERROR;
//         }
//         status = rxbuf2[0];
//         for (i = 0; i < rxlen; i++) {
//             rxbuf[i] = rxbuf2[i + 1];
//         }
//     }
//     /*
//     if (spi_dev) {
//         spi_dev->write_then_read(txbuf, txlen, rxbuf2, rxlen + 1, 0x00);
//         status = rxbuf2[0];
//         for (i = 0; i < rxlen; i++) {
//             rxbuf[i] = rxbuf2[i + 1];
//         }
//         delay(interdelay);
//     }
//     */

//     /* Mask out bytes available in the status response. */
//     return (status >> 2);
// }

/******************************************************************************
  This is a library for the MLX90395 magnetometer.

  Designed specifically to work with the MLX90395 breakout from Adafruit:

  ----> https://www.adafruit.com/products/4022

  These sensors use I2C to communicate, 2 pins are required to interface.

  Adafruit invests time and resources providing this open source code, please
  support Adafruit and open-source hardware by purchasing products from
  Adafruit!

  Written by Kevin Townsend/ktown for Adafruit Industries.

  MIT license, all text above must be included in any redistribution
 *****************************************************************************/

#include "MLX90395.h"

MLX90395::MLX90395(int _id, bool _debug){
     //MLX90395::info.id = _id;
     //MLX90395::info.name = "MLX90395 Magnetometer";		// assign sensor name
     //MLX90395::info.address = MLX90395_DEFAULT_ADDR;		// assign sensor i2c address
     //MLX90395::sensorDebug = _debug;						// debug flag status
}

/*!
 *    @brief  Sets up the hardware and initializes I2C
 *    @param  i2c_addr
 *            The I2C address to be used.
 *    @param  wire
 *            The Wire object to be used for I2C connections.
 *    @return True if initialization was successful, otherwise false.
 */

bool MLX90395::begin_I2C(uint8_t i2c_addr, TwoWire *wire) {
  if (!i2c_dev) {
    i2c_dev = new Adafruit_I2CDevice(i2c_addr, wire);
  }
  spi_dev = NULL;

  if (!i2c_dev->begin()) {
    return false;
  }

  return _init();
}

bool MLX90395::_init(void) {
  if (!exitMode())
    return false;

  delay(10);

  if (!reset())
    return false;

  delay(10);

  _gain = getGain();
  if (_gain == 8) { // default high field gain
    _uTLSB = 7.14;
  } else {
    _uTLSB = 2.5; // medium field gain
  }

  _resolution = getResolution();

  if (!readRegister(0x26, &uniqueID[0]) || !readRegister(0x27, &uniqueID[1]) ||
      !readRegister(0x28, &uniqueID[2])) {
    return false;
  }

  return true;
}

/**
 * Performs a single X/Y/Z conversion and returns the results.
 *
 * @param x     Pointer to where the 'x' value should be stored.
 * @param y     Pointer to where the 'y' value should be stored.
 * @param z     Pointer to where the 'z' value should be stored.
 *
 * @return True if the operation succeeded, otherwise false.
 */
bool MLX90395::readData(float *x, float *y, float *z) {
  if (!startSingleMeasurement())
    return false;
  while (!readMeasurement(x, y, z))
    delay(1);

  return true;
}

/**
 * Reads data from data register & returns the results.
 *
 * @param x     Pointer to where the 'x' value should be stored.
 * @param y     Pointer to where the 'y' value should be stored.
 * @param z     Pointer to where the 'z' value should be stored.
 *
 * @return True on command success
 */
bool MLX90395::readMeasurement(float *x, float *y, float *z) {
  uint8_t tx[1] = {0x80}; // Read memory command
  uint8_t rx[12] = {0};   // status, crc, X16, Y16, Z16, T16, V16

  /* Perform the transaction. */
  if (i2c_dev) {
    if (!i2c_dev->write_then_read(tx, 1, rx, 12)) {
      return false;
    }
  }

  // check status
  // Serial.print("Status: "); Serial.println(rx[0], HEX);
  if (rx[0] != MLX90395_STATUS_DRDY) {
    return false;
  }

  int16_t xi, yi, zi;

  // Convert data to uT and float.
  xi = (rx[2] << 8) | rx[3];
  yi = (rx[4] << 8) | rx[5];
  zi = (rx[6] << 8) | rx[7];

  *x = xi;
  *y = yi;
  *z = zi;

  // multiply by gain & LSB
  *x *= gainMultipliers[_gain] * _uTLSB;
  *y *= gainMultipliers[_gain] * _uTLSB;
  *z *= gainMultipliers[_gain] * _uTLSB;

  return true;
}

/**
 * Perform a soft reset
 * @return True if the operation succeeded, otherwise false.
 */
bool MLX90395::reset(void) {
  return (command(MLX90395_REG_RT) == MLX90395_STATUS_RESET);
}

/**
 * Perform a mode exit
 * @return True if the operation succeeded, otherwise false.
 */
bool MLX90395::exitMode(void) {
  // do once and ignore status
  command(MLX90395_REG_EX);

  return command(MLX90395_REG_EX) == 0;
}

/**
 * Begin a single measurement on all axes
 *
 * @return True on command success
 */
bool MLX90395::startSingleMeasurement(void) {
  return (command(MLX90395_REG_SM | 0x0F) == MLX90395_STATUS_SMMODE);
}

/**
 * Get the current oversampling setting
 * @return MLX90395_OSR_1, MLX90395_OSR_2, MLX90395_OSR_4, or MLX90395_OSR_8
 */
mlx90393_osr_t MLX90395::getOSR(void) {
  Adafruit_BusIO_Register reg2 =
      Adafruit_BusIO_Register(i2c_dev, MLX90395_REG_2, 2, MSBFIRST);
  Adafruit_BusIO_RegisterBits osr_bits =
      Adafruit_BusIO_RegisterBits(&reg2, 2, 0);
  return (mlx90393_osr_t)osr_bits.read();
}

/**
 * Set the current oversampling setting
 * @param osrval MLX90395_OSR_1, MLX90395_OSR_2, MLX90395_OSR_4,
 *  or MLX90395_OSR_8
 * @return True on command success
 */
bool MLX90395::setOSR(mlx90393_osr_t osrval) {
  Adafruit_BusIO_Register reg2 =
      Adafruit_BusIO_Register(i2c_dev, MLX90395_REG_2, 2, MSBFIRST);
  Adafruit_BusIO_RegisterBits osr_bits =
      Adafruit_BusIO_RegisterBits(&reg2, 2, 0);
  return osr_bits.write(osrval);
}

/**
 * Get the current gainsel value, see DS 18.6. Sensitivity for values
 * @return 0-15 gain selection offset
 */
uint8_t MLX90395::getGain(void) {
  Adafruit_BusIO_Register reg0 =
      Adafruit_BusIO_Register(i2c_dev, MLX90395_REG_0, 2, MSBFIRST);
  Adafruit_BusIO_RegisterBits gain_bits =
      Adafruit_BusIO_RegisterBits(&reg0, 4, 4);
  return gain_bits.read();
}

/**
 * Get the current gainsel value, see DS 18.6. Sensitivity for values
 * @param gainval 0-15 gain selection offset
 * @return True on command success
 */
bool MLX90395::setGain(uint8_t gainval) {
  gainval &= 0xF;
  _gain = gainval; // cache it

  Adafruit_BusIO_Register reg0 =
      Adafruit_BusIO_Register(i2c_dev, MLX90395_REG_0, 2, MSBFIRST);
  Adafruit_BusIO_RegisterBits gain_bits =
      Adafruit_BusIO_RegisterBits(&reg0, 4, 4);
  return gain_bits.write(gainval);
}

/**
 * Get the resolution, note that its till 16 bits just offset within the 19
 * bit ADC output.
 * @return MLX90395_RES_16, MLX90395_RES_17, MLX90395_RES_18 or
 *  MLX90395_RES_19
 */
mlx90393_res_t MLX90395::getResolution(void) {
  Adafruit_BusIO_Register reg2 =
      Adafruit_BusIO_Register(i2c_dev, MLX90395_REG_2, 2, MSBFIRST);
  Adafruit_BusIO_RegisterBits resX_bits =
      Adafruit_BusIO_RegisterBits(&reg2, 2, 5);
  return (mlx90393_res_t)resX_bits.read();
}

/**
 * Set the resolution, note that its till 16 bits just offset within the 19
 * bit ADC output.
 * @param resval MLX90395_RES_16, MLX90395_RES_17, MLX90395_RES_18 or
 *  MLX90395_RES_19
 * @return True on command success
 */
bool MLX90395::setResolution(mlx90393_res_t resval) {
  _resolution = resval; // cache it

  Adafruit_BusIO_Register reg2 =
      Adafruit_BusIO_Register(i2c_dev, MLX90395_REG_2, 2, MSBFIRST);
  Adafruit_BusIO_RegisterBits resX_bits =
      Adafruit_BusIO_RegisterBits(&reg2, 2, 5);
  resX_bits.write(resval);
  Adafruit_BusIO_RegisterBits resY_bits =
      Adafruit_BusIO_RegisterBits(&reg2, 2, 7);
  resY_bits.write(resval);
  Adafruit_BusIO_RegisterBits resZ_bits =
      Adafruit_BusIO_RegisterBits(&reg2, 2, 9);
  return resZ_bits.write(resval);
}

/****************************************************************/

uint8_t MLX90395::command(uint8_t cmd) {
  uint8_t tx[2] = {0x80, cmd};
  uint8_t status = 0xFF;

  /* Perform the transaction. */
  if (i2c_dev) {
    if (!i2c_dev->write_then_read(tx, 2, &status, 1)) {
      // Serial.println("Failed command");
      return 0xFF;
    }
  }
  return status;
}

bool MLX90395::readRegister(uint8_t reg, uint16_t *data) {
  /* Perform the transaction. */
  if (i2c_dev) {
    Adafruit_BusIO_Register regis =
        Adafruit_BusIO_Register(i2c_dev, reg << 1, 2);
    *data = regis.read();
  }

  return true;
}

/**************************************************************************/
/*!
    @brief  Gets the sensor_t device data, Adafruit Unified Sensor format
    @param  sensor Pointer to an Adafruit Unified sensor_t object that we'll
   fill in
*/
/**************************************************************************/
void MLX90395::getSensor(sensor_t *sensor) {
  /* Clear the sensor_t object */
  memset(sensor, 0, sizeof(sensor_t));

  /* Insert the sensor name in the fixed length char array */
  strncpy(sensor->name, "MLX90395", sizeof(sensor->name) - 1);
  sensor->name[sizeof(sensor->name) - 1] = 0;
  sensor->version = 1;
  sensor->sensor_id = _sensorID;
  sensor->type = SENSOR_TYPE_MAGNETIC_FIELD;
  sensor->min_delay = 0;
  sensor->min_value = -120000; // -120 gauss in uTesla
  sensor->max_value = 120000;  // +120 gauss in uTesla
  sensor->resolution = _uTLSB; // 240/16-bit uTesla per LSB
}

/**************************************************************************/
/*!
    @brief  Gets the most recent sensor event, Adafruit Unified Sensor format
    @param  event Pointer to an Adafruit Unified sensor_event_t object that
   we'll fill in
    @returns True on successful read
*/
/**************************************************************************/
bool MLX90395::getEvent(sensors_event_t *event) {
  /* Clear the event */
  memset(event, 0, sizeof(sensors_event_t));

  event->version = 1;
  event->sensor_id = _sensorID;
  event->type = SENSOR_TYPE_MAGNETIC_FIELD;
  event->timestamp = millis();

  return readData(&event->magnetic.x, &event->magnetic.y, &event->magnetic.z);
}

mlx_sample_t MLX90395::getSample(void){
     float x,y,z;
     mlx_sample_t sample;

     //MLX90395::setOpMode(true, LOW_POWER_1, SINGLE_MEASURE); // Only single measurement mode is currently supported
     //MLX90395::startSingleMeasurement();
     //delay(5); // Wait for single measurement to be taken

     if(MLX90395::readData(&x,&y,&z)){
         //MLX90395::setOpMode(true, LOW_POWER_1, POWER_DOWN); // No true equivalent function
         MLX90395::exitMode();

        /*
         sample.magnetic = MLX90395::getMagnetic(&x,&y,&z);
         sample.strength = MLX90395::getMagFieldStrength(sample.magnetic);
         */

         sample.magnetic.x = x;
         sample.magnetic.y = y;
         sample.magnetic.z = z;
         //sample.strength = sqrt(pow(sample.magnetic.x,2) + pow(sample.magnetic.y,2) + pow(sample.magnetic.z,2));
     }else{
         Serial.println(F("Waiting for data to get ready"));
     }

     return sample;
}