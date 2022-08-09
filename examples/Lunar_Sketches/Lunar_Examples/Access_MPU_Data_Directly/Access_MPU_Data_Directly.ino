#include <Wire.h>


//Things that john is letting users change - MPU6000_SMPLRT_DIV, DLPF

#define MPU6000_I2CADDR_DEFAULT 0x69    // MPU6000 default i2c address w/ AD0 high
#define MPU6000_ACCEL_CONFIG 0x1C       // Accelerometer specific configration register

#define MPU6000_SMPLRT_DIV 0x19         // sample rate divisor register
#define MPU6000_SMPLRT_DIV_VAL 0x0      // afffects the sampling rate, ranges from 0x00 to 0xFF

#define MPU6000_CONFIG 0x1A             // General configuration register
#define MPU6000_CONFIG_DLPF_BW 0x0      // changes bandwidth for digital low pass filter. Ranges from 0x0 to 0x7

#define MPU6000_PWR_MGMT_1 0x6B         // Primary power/sleep control register
#define MPU6000_PWR_MGMT_2 0x6C
#define MPU6000_LSB_PER_G 16384.0       // How many bits per LSB
#define MPU6000_ACCEL_OUT 0x3B          // base raw accel address (6 bytes for 3 axis)
#define MPU6000_GYRO_OUT 0x43           // location of gyro output
#define MPU6000_ACC_ONLY 0x07           //write this to MPU6000_PWR_MGMT_2 to put gyro to sleep
#define MPU6000_GYRO_ONLY 0x38          //write this to MPU6000_PWR_MGMT_2 to put acc to sleep

//see register 6B and 6C in the register map for more info
#define MPU6000_LP_WAKE_REG_6B_VAL 0b00101001    // To turn on Low Power Wake up, this bit in MPU6000_PWR_MGMT_1 must be set high
#define MPU6000_LP_WAKE_UP_REG_6C_1PT25HZ 0b00000111
#define MPU6000_LP_WAKE_UP_REG_6C_5HZ 0b01000111
#define MPU6000_LP_WAKE_UP_REG_6C_20HZ 0b10000111
#define MPU6000_LP_WAKE_UP_REG_6C_40HZ 0b11000111

#define MPU6000_SLEEP_VAL 0x40 //  write to MPU6000_PWR_MGMT_1 to put into sleep
#define MPU6000_RESET_VAL 0x80 // write to MPU6000_PWR_MGMT_1 to reset

void writeByte (uint8_t registerAddress, uint8_t writeData);
void readBytes(uint8_t registerAddress, uint8_t nBytes, uint8_t * data);

void setup(){
  Serial.begin(9600);

  Wire.begin();

  // Set limits to +/- 2g, don't change
  writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_ACCEL_CONFIG, 0x00);

  // sets which clock the acc uses, don't change this
  writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_1,0x01);

  // Set accelerometer division
  writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_SMPLRT_DIV,MPU6000_SMPLRT_DIV_VAL); //last argument

  // Set digital low pass filter bandwidth
  writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_CONFIG,MPU6000_CONFIG_DLPF_BW);


  // Only one of the following sections (code after '------') should be uncommented

  //-----accelerometer only
  writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_2, MPU6000_ACC_ONLY);

  //-----Gyro only
  // writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_2, MPU6000_GYRO_ONLY);

  //-----Low power acc only wakup mode
  // writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_1,MPU6000_LP_WAKE_REG_6B_VAL);
  // writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_2,MPU6000_LP_WAKE_UP_REG_6C_1PT25HZ); //change the last argument to change frequncy of wakeups

  //-----sleep
  // writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_1,MPU6000_SLEEP_VAL);
}

void loop(){

  long timestamp = micros();

  float acc_x,acc_y,acc_z,gyro_x,gyro_y,gyro_z;

  uint8_t buffer[6];
  readBytes(MPU6000_I2CADDR_DEFAULT, MPU6000_ACCEL_OUT, 6, &buffer[0]);
  acc_x = (float)(buffer[0] << 8 | buffer[1]) / MPU6000_LSB_PER_G * -1;
  acc_y = (float)(buffer[2] << 8 | buffer[3]) / MPU6000_LSB_PER_G * -1;
  acc_z = (float)(buffer[4] << 8 | buffer[5]) / MPU6000_LSB_PER_G * -1;

  readBytes(MPU6000_I2CADDR_DEFAULT, MPU6000_GYRO_OUT, 6, &buffer[0]);
  gyro_x = (float)(buffer[0] << 8 | buffer[1]) / 131 * -1; //131 deg/LSB
  gyro_y = (float)(buffer[2] << 8 | buffer[3]) / 131 * -1;
  gyro_z = (float)(buffer[4] << 8 | buffer[5]) / 131 * -1;

  timestamp = micros()-timestamp;
  Serial.println(String(timestamp)+" "+String(acc_x)+" "+String(acc_y)+" "+String(acc_z)+" "+String(gyro_x)+" "+String(gyro_y)+" "+String(gyro_z));
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

// /**
//  * Parameters: Register Address, Starting Bit, Length, Data
//  * Returns: None
//  * This function  writes data byte for specified length
// **/
// void writeBits(uint8_t registerAddress, uint8_t startBit, uint8_t length, uint8_t data){
//     uint8_t buff;
//     if (readByte(registerAddress) != 0){
//         uint8_t mask = ((1 << length) - 1) << (startBit - length + 1);
//         data <<= (startBit - length + 1);
//         data &= mask;
//         buff &= ~(mask);
//         buff |= data;
//         writeByte(registerAddress, buff);
//     } else {
// 	//TODO
//     }
// }
