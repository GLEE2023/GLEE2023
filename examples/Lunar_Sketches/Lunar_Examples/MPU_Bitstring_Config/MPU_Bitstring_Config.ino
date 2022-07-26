/*
Authors: Luke Roberson

Made to take in a bitstring from GLEE2023 ModeUserInterface Model and setup
MPU6000 accordingly
*/

/*
Bitstring to define config of accelerometer
Bits from LSB to MSB:
(bits): description

0-3: mode
  modes:
    0x00: sleep
    0x01: low_power_wakeup_1.25
    0x02  low_power_wakeup_5
    0x03  low_power_wakeup_20
    0x04  low_power_wakeup_40
    0x05  accelerometer_only
    0x06  gyroscope_only
    0x07  gyroscope_accelerometer
4-11: sample rate div, 0 to 255
12-14: digital low pass filter (DLPF), 0-6
    0x0: 260Hz, DLPF off
    0x1: 184 Hz
    0x2: 94 Hz
    0x3: 44 Hz
    0x4: 21 Hz
    0x5: 10 Hz
    0x6: 5 Hz
    0x7: invalid
*/

#include <Wire.h>
#include <EEPROM.h>

#define ACC_CONFIG_STRING 0b000000000000010
#define DURATION 0xA //100 seconds


//Things that john is letting users change - MPU6000_SMPLRT_DIV, DLPF

#define MPU6000_I2CADDR_DEFAULT 0x69    // MPU6000 default i2c address w/ AD0 high
#define MPU6000_ACCEL_CONFIG 0x1C       // Accelerometer specific configration register

#define MPU6000_SMPLRT_DIV 0x19         // sample rate divisor register
#define MPU6000_SMPLRT_DIV_VAL 0x0      // afffects the sampling rate, ranges from 0x00 to 0xFF

#define MPU6000_CONFIG 0x1A             // General configuration register
#define MPU6000_CONFIG_DLPF_BW 0x0        // changes bandwidth for digital low pass filter. Ranges from 0x0 to 0x7


#define MPU6000_PWR_MGMT_1 0x6B         // Primary power/sleep control register
#define MPU6000_PWR_MGMT_2 0x6C
#define MPU6000_LSB_PER_G 16384.0       // How many bits per LSB
#define MPU6000_ACCEL_OUT 0x3B          // base raw accel address (6 bytes for 3 axis)
#define MPU6000_GYRO_OUT 0x43           // location of gyro output
#define MPU6000_ACC_ONLY 0x07           //write this to MPU6000_PWR_MGMT_2 to put gyro to sleep
#define MPU6000_GYRO_ONLY 0x38          //write this to MPU6000_PWR_MGMT_2 to put acc to sleep
#define MPU6000_ACC_AND_GYRO 0x3F       ////write this to MPU6000_PWR_MGMT_2 to use both acc and gyro

//see register 6B and 6C in the register map for more info
#define MPU6000_LP_WAKE_REG_6B_VAL 0b00101001    // To turn on Low Power Wake up
#define MPU6000_LP_WAKE_UP_REG_6C_1PT25HZ 0b00000111
#define MPU6000_LP_WAKE_UP_REG_6C_5HZ 0b01000111
#define MPU6000_LP_WAKE_UP_REG_6C_20HZ 0b10000111
#define MPU6000_LP_WAKE_UP_REG_6C_40HZ 0b11000111

#define MPU6000_SLEEP_VAL 0x40 //  write to MPU6000_PWR_MGMT_1 to put into sleep
#define MPU6000_RESET_VAL 0x80 // write to MPU6000_PWR_MGMT_1 to reset

#define ACC_MEM_LOC 0x000
#define GYRO_MEM_LOC 0x200

void writeByte (uint8_t registerAddress, uint8_t writeData);
void readBytes (uint8_t registerAddress, uint8_t nBytes, uint8_t * data);

void setup(){
  Serial.begin(9600);

  Wire.begin();

  // Set limits to +/- 2g, don't change
  writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_ACCEL_CONFIG, 0x00);

  // sets which clock the acc uses, don't change this
  writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_1,0x01);

  //This switch sets the mode
  switch(ACC_CONFIG_STRING<<0 & 0xF){
    //gets the first 4 bits
    case 0x0:
      // sleep
      writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_1,MPU6000_SLEEP_VAL);
      Serial.println(F("Mode Set to sleep"));
      break;
    case 0x1:
      // low power wakeup @ 1.25 Hz
      writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_1,MPU6000_LP_WAKE_REG_6B_VAL);
      writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_2,MPU6000_LP_WAKE_UP_REG_6C_1PT25HZ);
      Serial.println(F("Mode Set to MPU6000_LP_WAKE_UP_REG_6C_1PT25HZ"));
      break;
    case 0x2:
      // low power wakeup @ 5 Hz
      writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_1,MPU6000_LP_WAKE_REG_6B_VAL);
      writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_2,MPU6000_LP_WAKE_UP_REG_6C_5HZ);
      Serial.println(F("Mode Set to MPU6000_LP_WAKE_UP_REG_6C_5HZ"));
      break;
    case 0x3:
      // low power wakeup @ 20 Hz
      writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_1,MPU6000_LP_WAKE_REG_6B_VAL);
      writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_2,MPU6000_LP_WAKE_UP_REG_6C_20HZ);
      Serial.println(F("Mode Set to MPU6000_LP_WAKE_UP_REG_6C_20HZ"));
      break;
    case 0x4:
      // low power wakeup @ 40 Hz
      writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_1,MPU6000_LP_WAKE_REG_6B_VAL);
      writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_2,MPU6000_LP_WAKE_UP_REG_6C_40HZ);
      Serial.println(F("Mode Set to MPU6000_LP_WAKE_UP_REG_6C_40HZ"));
      break;
    case 0x5:
      //accerometer only
      writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_2, MPU6000_ACC_ONLY);
      Serial.println(F("Mode Set to MPU6000_ACC_ONLY"));
      break;
    case 0x6:
      //gyro only
      writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_2, MPU6000_GYRO_ONLY);
      Serial.println(F("Mode Set to MPU6000_GYRO_ONLY"));
      break;
    case 0x7:
      // Accelerometer & Gyro
      writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_2, MPU6000_ACC_AND_GYRO);
      Serial.println(F("Mode Set to MPU6000_ACC_AND_GYRO"));
      break;
    default:
      // default to only accelerometer
      writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_2, MPU6000_ACC_ONLY);
      Serial.println(F("Unknown Mode. Mode Set to MPU6000_ACC_ONLY"));
      break;
  }

  //set sampling rate div
  uint8_t sampling_rate_div = (uint8_t)(ACC_CONFIG_STRING>>4 & 0xFF);
  writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_SMPLRT_DIV,sampling_rate_div);
  Serial.println("Sampling rate division set to "+String(sampling_rate_div));

  // Set digital low pass filter bandwidth
  uint8_t dlpf_value = (uint8_t)(ACC_CONFIG_STRING>>12 & 0x7);
  writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_CONFIG,dlpf_value);
  Serial.println("Digtal low pass filter set to "+String(dlpf_value));
}

void loop(){
  Serial.println("Begining");

  long timestamp = micros();

  float acc_x,acc_y,acc_z,gyro_x,gyro_y,gyro_z;

  for(int i=0; i<512; i+=12){

    uint8_t buffer[6];

    readBytes(MPU6000_I2CADDR_DEFAULT, MPU6000_ACCEL_OUT, 6, &buffer[0]);
    for(int j=0; j<6; j++){EEPROM.write(ACC_MEM_LOC+i+j,buffer[j]);}

    readBytes(MPU6000_I2CADDR_DEFAULT, MPU6000_GYRO_OUT, 6, &buffer[0]);
    for(int j=6; j<12; j++) EEPROM.write(GYRO_MEM_LOC+i+j,buffer[j]);
  }

  timestamp = micros()-timestamp;


  for(int i=ACC_MEM_LOC; i<512+ACC_MEM_LOC; i+=6){
    acc_x = (float)(EEPROM.read(i) << 8 | EEPROM.read(i+1)) / MPU6000_LSB_PER_G * -1;
    acc_y = (float)(EEPROM.read(i+2) << 8 | EEPROM.read(i+3)) / MPU6000_LSB_PER_G * -1;
    acc_z = (float)(EEPROM.read(i+4) << 8 | EEPROM.read(i+5)) / MPU6000_LSB_PER_G * -1;

    Serial.println(String(acc_x)+" "+String(acc_y)+" "+String(acc_z));
  }

  // Serial.println("Printing Gyro data");
  //
  // for(int i=GYRO_MEM_LOC; i<512+GYRO_MEM_LOC; i+=6){
  //   gyro_x = (float)(EEPROM.read(i) << 8 | EEPROM.read(i+1)) / 131 * -1; //131 deg/LSB
  //   gyro_y = (float)(EEPROM.read(i+2) << 8 | EEPROM.read(i+3)) / 131 * -1;
  //   gyro_z = (float)(EEPROM.read(i+4) << 8 | EEPROM.read(i+5)) / 131 * -1;
  //
  //   Serial.println(String(gyro_x)+" "+String(gyro_y)+" "+String(gyro_z));
  // }

  Serial.print("It took ");
  Serial.print(timestamp);
  Serial.println(" to run.");

  Serial.println("Restarting in 5 seconds");
  delay(5000);
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
