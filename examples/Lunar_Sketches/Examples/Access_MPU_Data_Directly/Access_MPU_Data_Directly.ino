#include <Wire.h>

#define MPU6000_I2CADDR_DEFAULT 0x69    // MPU6000 default i2c address w/ AD0 high
#define MPU6000_ACCEL_CONFIG 0x1C       // Accelerometer specific configration register
#define MPU6000_SMPLRT_DIV 0x19         // sample rate divisor register
#define MPU6000_CONFIG 0x1A             // General configuration register
#define MPU6000_PWR_MGMT_1 0x6B         // Primary power/sleep control register
#define MPU6000_LSB_PER_G 16384.0       // How many bits per LSB
#define MPU6000_ACCEL_OUT 0x3B          // base raw accel address (6 bytes for 3 axis)
#define MPU6000_BANDWIDTH 0

void writeByte (uint8_t registerAddress, uint8_t writeData);
void readBytes(uint8_t registerAddress, uint8_t nBytes, uint8_t * data);

void setup(){
  Serial.begin(9600);

  Wire.begin();

  // Set limits to +/- 2g
  writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_ACCEL_CONFIG, 0b00000000);

  // Set accelerometer division
  writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_SMPLRT_DIV, 0);

  // Set bandwidth
  writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_CONFIG, MPU6000_BANDWIDTH);

  //
  writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_1, 0x01);


}

void loop(){

  long timestamp = micros();

  float x,y,z;
  uint8_t buffer[6];
  readBytes(MPU6000_I2CADDR_DEFAULT, MPU6000_ACCEL_OUT, 6, &buffer[0]);
  x = (float)(buffer[0] << 8 | buffer[1]) / MPU6000_LSB_PER_G * -1;
  y = (float)(buffer[2] << 8 | buffer[3]) / MPU6000_LSB_PER_G * -1;
  z = (float)(buffer[4] << 8 | buffer[5]) / MPU6000_LSB_PER_G * -1;

  timestamp = micros()-timestamp;
  Serial.println(String(timestamp)+" "+String(x)+" "+String(y)+" "+String(z));
}


void readBytes(uint8_t I2CsensorAddress, uint8_t registerAddress, uint8_t nBytes, uint8_t * data){
    Wire.beginTransmission(I2CsensorAddress);           // begins forming transmission to sensor
    Wire.write(registerAddress);                     // Add register address to transmission
    Wire.endTransmission();
    Wire.requestFrom(I2CsensorAddress, nBytes);         // Request and listen for response
    // Record response, wire will be available until nBytes are read
    int i = 0;
    while(Wire.available()){
        data[i] = Wire.read();
        i++;
    }
}

/**
 * Parameters: Register Address, Write Data
 * Returns: None
 * This function writes data to specified address
**/
void writeByte (uint8_t I2CsensorAddress, uint8_t registerAddress, uint8_t writeData){
    Wire.beginTransmission(I2CsensorAddress);               // begin communication with the sensor
    Wire.write(registerAddress);                                // point to address to be written to
    Wire.write(writeData);                                      // write data to adress specificed above
    Wire.endTransmission();                                     // end communication
}
