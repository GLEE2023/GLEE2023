#include <Wire.h>

#define TP_OBJECT 1                             // Object Temperature reg                         (3xBytes) 17bit value [read] 
#define TP_AMBIENT 3                            // Ambient Temp Reg                               (2xBytes) 15bit value [read]
#define TP_OBJECT_LP1 5                         // Low Pass filter of object signal value 1 reg   (3xBytes) 20bit value [read] comparison: 8
#define TP_OBJECT_LP2 7                         // Low Pass filter of object signal value 2 reg   (3xBytes) 20bit value [read] comparison: 8
#define TP_AMBIENT_LP3 10                       // Low Pass filter of ambient signal value 3 reg  (2xBytes) 16bit value [read] comparison: 2
#define TP_OBJECT_LP2_FROZEN 12                 // Low pass filter of object on motion reg        (3xbytes) 24bit
#define TP_PRESENCE 15                          // READ
#define TP_MOTION 16                            // READ
#define TP_AMBIENT_SHOCK 17                     // READ
#define TP_INTERUPT_STATUS 18                   // READ
#define TP_CHIP_STATUS 19                       // READ
#define TP_LOW_PASS_TIME_CONST_LP1_LP2 20       // READ/WRITE
#define TP_LOW_PASS_TIME_CONST_LP3 21           // READ/WRITE
#define TP_PRESENCE_THRESHOLD 22                // READ/WRITE
#define TP_MOTION_THRESHOLD 23                  // READ/WRITE
#define TP_AMBIENT_SHOCK_THRESHOLD 24           // READ/WRITE
#define TP_INTERUPT_MASK 25                     // READ/WRITE
#define TP_MULTIPLE 26                          // READ/WRITE
#define TP_TIMER_INTERRUPT 27                   // READ/WRITE
#define TP_OT_THRESHOLD_HIGH 28                 // READ/WRITE
// Set to 0x80 for eeprom reads through red, set to 0x00 for no accsess
#define TP_EEPROM_CONTROL 31                    // 1byte READ/WRITE
#define TP_PROTOCOL 32                          // READ/WRITE
#define TP_CHSUM 33                             // READ
#define TP_LOOKUP 41                            // READ
#define TP_PTAT25 42                            // 2xbytes 15 bit value READ
#define TP_M 44                                 // 2xbytes 16 bit value requires RegVal/100 offset typically 172 counts / K READ
#define TP_U0 46                                // 2xbytes 16 bit value requires RegVal + 32768 offset READ
#define TP_UOUT1 48                             // 2xbytes 16 bit value
#define TP_T_OBJ_1 50                           // 1xbytes 
#define TP_I2C_ADDR 63  
#define TPIS1385_I2C_ADDR 0x0D             //I2C Sensor Address


void writeByte (uint8_t registerAddress, uint8_t writeData);
void readBytes(uint8_t registerAddress, uint8_t nBytes, uint8_t * data);

void setup(){
  Serial.begin(9600); // Sets baud rate to 9600 bits per second and starts serial communications
  
  //thermopile.begin(); // Thermopile start-up
  Wire.begin();                          // Begin i2c coms at standard speed
	Wire.beginTransmission(0x00);    // Reload all call   
  Wire.write(0x04);
  Wire.write(0x00);
	if(Wire.endTransmission()!=0) Serial.println(F("Init call failiure"));
    delay(50);  // Wait on i2c transmission
    Serial.println(F("TPIS coms Init complete"));
  
  //thermopile.readEEprom(); // Prints eeprom and updates calibration constants
}

void loop(){
  
  //getTPamb
  uint8_t data[2] = {0,0};
  readBytes(TPIS1385_I2C_ADDR, TP_AMBIENT, 2, &data[0]);
  uint16_t getTPamb = ( (uint16_t)(data[0] & 0x7F) << 8) | data[1] ; // See data sheet for calc

  //getTPobj
  uint8_t data1[3] = {0,0,0};
  readBytes(TPIS1385_I2C_ADDR, TP_OBJECT, 3, &data1[0]);
  uint32_t getTPobj = ( (uint32_t) ( (uint32_t)data1[0] << 24) | ( (uint32_t)data1[1] << 16) | ( (uint32_t)data1[2] & 0x80) << 8) >> 15;

  //Serial.print("Ambient temperature: "); 
  Serial.println(getTPamb);
  
  //Serial.print("Object temperature: "); 
  Serial.println(getTPobj);
  delay(1000);

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
