/*
Bitstring to define config of Magnetometer Sensor
Bits from LSB to MSB:
(bits): description
0: Power
    0 : PowerDown
    1 : Active
1-2: Data Rate
    00 : 10Hz
    10 : 20Hz
    01 : 100Hz
    11 : 1kHz
3: Mode
    0 : Continuous mode
    1 : Single mode
*/

#include <Wire.h>

#define BM_CONFIG_STRING 0b1000

#define BM1422AGMV_DEVICE_ADDRESS_0E   (0x0E)    // 7bit Addrss
#define BM1422AGMV_DEVICE_ADDRESS_0F   (0x0F)    // 7bit Address
#define BM1422AGMV_WIA_VAL             (0x41)

#define BM1422AGMV_WIA                 (0x0F)
#define BM1422AGMV_DATAX               (0x10)
#define BM1422AGMV_STA1                (0x18)
#define BM1422AGMV_CNTL1               (0x1B)
#define BM1422AGMV_CNTL2               (0x1C)
#define BM1422AGMV_CNTL3               (0x1D)
#define BM1422AGMV_AVE_A               (0x40)
#define BM1422AGMV_CNTL4               (0x5C)

#define BM1422AGMV_STA1_RD_DRDY        (1 << 6)

#define BM1422AGMV_CNTL1_FS1           (1 << 1)
#define BM1422AGMV_CNTL1_ODR_10Hz      (0 << 3)
#define BM1422AGMV_CNTL1_RST_LV        (1 << 5)
#define BM1422AGMV_CNTL1_OUT_BIT       (1 << 6)
#define BM1422AGMV_CNTL1_PC1           (1 << 7)

#define BM1422AGMV_CNTL2_DRP           (1 << 2)
#define BM1422AGMV_CNTL2_DREN          (1 << 3)

#define BM1422AGMV_CNTL3_FORCE         (1 << 6)

#define BM1422AGMV_AVE_A_AVE4          (0 << 2)

#define BM1422AGMV_CNTL1_VAL           (BM1422AGMV_CNTL1_FS1 | BM1422AGMV_CNTL1_OUT_BIT | BM1422AGMV_CNTL1_PC1)
#define BM1422AGMV_CNTL2_VAL           (BM1422AGMV_CNTL2_DREN)
#define BM1422AGMV_CNTL3_VAL           (BM1422AGMV_CNTL3_FORCE)
#define BM1422AGMV_CNTL4_VAL           (0x0000)
#define BM1422AGMV_AVE_A_VAL           (BM1422AGMV_AVE_A_AVE4)

#define BM1422AGMV_14BIT_SENS          (24)
#define BM1422AGMV_12BIT_SENS          (6)

int _sens;

void writeByte (uint8_t registerAddress, uint8_t writeData);
void readBytes(uint8_t registerAddress, uint8_t nBytes, uint8_t * data);

void setup(){
    Serial.begin(9600);
    Wire.begin();
    delay(1000);

    uint16_t first = 0b0010;
    uint16_t second = ((uint16_t)BM_CONFIG_STRING);
    uint16_t result = ((0b0010 << 12) | (BM_CONFIG_STRING << 5)); 

    //Serial.println(result, BIN);
    Wire.beginTransmission(TMP117_TEMP_I2C);
    Wire.write(TMP117_CONFIG_REG);
    Wire.write((uint8_t)(result >> 8));
    Wire.write((uint8_t)result);
    Wire.endTransmission();

    //Serial.println("setup done");
}

void loop(){
    
    uint8_t buffer[2];
    readBytes(TMP117_TEMP_I2C, TMP117_TEMP_REG, 2, &buffer[0]);
    float temp = (float)(buffer[0] << 8 | buffer[1]) * TMP117_RESOLUTION;
    
    Serial.println(String(temp));
    //delay(500);
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
    Serial.println(writeData, BIN);
    Serial.println("Out of writebyte");
}

