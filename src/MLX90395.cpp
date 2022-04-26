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
    //  MLX90395::info.id = _id;
    //  MLX90395::info.name = "MLX90395 Magnetometer";		// assign sensor name
    //  MLX90395::info.address = MLX90395_DEFAULT_ADDR;		// assign sensor i2c address
    //  MLX90395::sensorDebug = _debug;						// debug flag status
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

  _resolutionX = getResolution();

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
mlx90395_osr_t MLX90395::getOSR(void) {
  Adafruit_BusIO_Register reg2 =
      Adafruit_BusIO_Register(i2c_dev, MLX90395_REG_2, 2, MSBFIRST);
  Adafruit_BusIO_RegisterBits osr_bits =
      Adafruit_BusIO_RegisterBits(&reg2, 2, 0);
  return (mlx90395_osr_t)osr_bits.read();
}

/**
 * Set the current oversampling setting
 * @param osrval MLX90395_OSR_1, MLX90395_OSR_2, MLX90395_OSR_4,
 *  or MLX90395_OSR_8
 * @return True on command success
 */
bool MLX90395::setOSR(mlx90395_osr_t osrval) {
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
mlx90395_res_t MLX90395::getResolution(void) {
  Adafruit_BusIO_Register reg2 =
      Adafruit_BusIO_Register(i2c_dev, MLX90395_REG_2, 2, MSBFIRST);
  Adafruit_BusIO_RegisterBits resX_bits =
      Adafruit_BusIO_RegisterBits(&reg2, 2, 5);
  return (mlx90395_res_t)resX_bits.read();
}

/**
 * Set the resolution, note that its till 16 bits just offset within the 19
 * bit ADC output.
 * @param resval MLX90395_RES_16, MLX90395_RES_17, MLX90395_RES_18 or
 *  MLX90395_RES_19
 * @return True on command success
 */
bool MLX90395::setResolution(enum mlx90395_res resX, enum mlx90395_res resY, enum mlx90395_res resZ) {
  _resolutionX = resX; // cache it
  _resolutionY = resY;
  _resolutionZ = resZ;

  Adafruit_BusIO_Register reg2 =
      Adafruit_BusIO_Register(i2c_dev, MLX90395_REG_2, 2, MSBFIRST);
  Adafruit_BusIO_RegisterBits resX_bits =
      Adafruit_BusIO_RegisterBits(&reg2, 2, 5);
  resX_bits.write(resX);
  Adafruit_BusIO_RegisterBits resY_bits =
      Adafruit_BusIO_RegisterBits(&reg2, 2, 7);
  resY_bits.write(resY);
  Adafruit_BusIO_RegisterBits resZ_bits =
      Adafruit_BusIO_RegisterBits(&reg2, 2, 9);
  return resZ_bits.write(resZ);
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

enum mlx90395_filter MLX90395::getFilter(void) { return _dig_filt; }
/**
 * Parameters: filter - the digital filter setting
 * Returns: True if successful, false if not
 * This function sets the digital filter.
**/
bool MLX90395::setFilter(enum mlx90395_filter filter) {
    _dig_filt = filter; // cache it

  Adafruit_BusIO_Register reg2 =
      Adafruit_BusIO_Register(i2c_dev, MLX90395_REG_2, 2, MSBFIRST);
  Adafruit_BusIO_RegisterBits filterX_bits =
      Adafruit_BusIO_RegisterBits(&reg2, 3, 2);
  filterX_bits.write(filter);
  return filterX_bits.write(filter);
}