#include <Wire.h>

//MPU6000 Defines
#define MPU6000_I2CADDR_DEFAULT 0x69    // MPU6000 default i2c address w/ AD0 high
#define MPU6000_ACCEL_CONFIG 0x1C       // Accelerometer specific configration register
#define MPU6000_SMPLRT_DIV 0x19         // sample rate divisor register
#define MPU6000_CONFIG 0x1A             // General configuration register
#define MPU6000_PWR_MGMT_1 0x6B         // Primary power/sleep control register
#define MPU6000_LSB_PER_G 16384.0       // How many bits per LSB
#define MPU6000_ACCEL_OUT 0x3B          // base raw accel address (6 bytes for 3 axis)
#define MPU6000_BANDWIDTH 0

//TMP117 Defines
#define TMP117_TEMP_I2C 0x48    // TMP I2C Register
#define TMP117_TEMP_REG 0x00    // TMP Data Register
#define TMP117_RESOLUTION (double)0.0078125

//Capacitive Sensor Defines
#define CAP_PIN 14 //this is the pin number for analog 0 (A0)




void writeByte (uint8_t registerAddress, uint8_t writeData);
void readBytes(uint8_t registerAddress, uint8_t nBytes, uint8_t * data);

void setup(){
  Serial.begin(9600);

  Wire.begin();

  //Accelerometer setup----------------------------------------------------------------------------//
  writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_ACCEL_CONFIG, 0b00000000);   // Set limits to +/- 2g   //
  writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_SMPLRT_DIV, 0); // Set accelerometer division          //
  writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_CONFIG, MPU6000_BANDWIDTH);  // Set bandwidth          //
  writeByte(MPU6000_I2CADDR_DEFAULT,MPU6000_PWR_MGMT_1, 0x01);                                     //
  //-----------------------------------------------------------------------------------------------//

  //Capacitive Sensor setup-----------------------------------------------------//
  pinMode(CAP_PIN,INPUT);                                                       //
  //----------------------------------------------------------------------------//

}

void loop(){
  unsigned long timestamp = micros();

  //Varaible to store data
  uint8_t buffer[6];

  //Accelerometer Data Collection ----------------------------------------------//
  float acc_x,acc_y,acc_z;                                                      //
  readBytes(MPU6000_I2CADDR_DEFAULT, MPU6000_ACCEL_OUT, 6, &buffer[0]);         //
  acc_x = (float)(buffer[0] << 8 | buffer[1]) / MPU6000_LSB_PER_G * -1;         //
  acc_y = (float)(buffer[2] << 8 | buffer[3]) / MPU6000_LSB_PER_G * -1;         //
  acc_z = (float)(buffer[4] << 8 | buffer[5]) / MPU6000_LSB_PER_G * -1;         //
  //----------------------------------------------------------------------------//

  //Temperature Sensor Data Collection------------------------------------------//                                                        //
  readBytes(TMP117_TEMP_I2C, TMP117_TEMP_REG, 2, &buffer[0]);                   //
  float temp = (float)(buffer[0] << 8 | buffer[1]) * TMP117_RESOLUTION;         //
  //----------------------------------------------------------------------------//

  //Capacitive Sensor Data Collection-------------------------------------------//
  int16_t cap_data = analogRead(CAP_PIN);                                       //
  //----------------------------------------------------------------------------//


  timestamp = micros()-timestamp;
  Serial.println(String(timestamp)+", "+String(acc_x)+", "+String(acc_y)+", "+String(acc_z)+", "+String(temp)+", "+String(cap_data));
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
