#include "BM1422AGMV.h"
#include <Wire.h>

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



BM1422AGMV magnetometer = BM1422AGMV(1,false);

mlx_sample_t sample;

void setup (){
    Serial.begin(9600);
    
    uint8_t rc;
    unsigned char reg;
    unsigned char buf[2];
    
    rc = Wire.read(BM1422AGMV_WIA, &reg, sizeof(reg));
    
    Serial.print("BM1422AGMV_WHO_AMI Register Value = 0x");
    Serial.println(reg, HEX);

    // Step1
    reg = BM1422AGMV_CNTL1_VAL;
    rc = Wire.write(BM1422AGMV_CNTL1, &reg, sizeof(reg));
    
    // Check 12bit or 14bit
    buf[0] = (BM1422AGMV_CNTL1_VAL & BM1422AGMV_CNTL1_OUT_BIT);
    if (buf[0] == BM1422AGMV_CNTL1_OUT_BIT) {
        int _sens = BM1422AGMV_14BIT_SENS;
    }else{
        int _sens = BM1422AGMV_12BIT_SENS;
    }
    
    delay(1);
    
    buf[0] = (BM1422AGMV_CNTL4_VAL >> 8) & 0xFF;
    buf[1] = (BM1422AGMV_CNTL4_VAL & 0xFF);
    rc = Wire.write(BM1422AGMV_CNTL4, buf, sizeof(buf));
    
    // Step2
    reg = BM1422AGMV_CNTL2_VAL;
    rc = Wire.write(BM1422AGMV_CNTL2, &reg, sizeof(reg));
    
    // Step3
    // Option
    reg = BM1422AGMV_AVE_A_VAL;
    rc = Wire.write(BM1422AGMV_AVE_A, &reg, sizeof(reg));
    
    return (rc);
};

void loop (){   

    sample = magnetometer.getSample();

    // Print out magnetic field measurements for each axis 
    Serial.println("Magnetic Field Axes Measurements");
    Serial.print("X: "); Serial.print(sample.magnetic.x,4); Serial.println(" uT");
    Serial.print("Y: "); Serial.print(sample.magnetic.y,4); Serial.println(" uT");
    Serial.print("Z: "); Serial.print(sample.magnetic.z,4); Serial.println(" uT");
    Serial.println();
    // Print out strength of magnetic field
    Serial.println("Magnetic Field Strength (Magnitude)"); 
    Serial.print(sample.strength,4); Serial.println(" uT");
    Serial.println();
   
    delay(1000); // Take samples every one second
};

void write(unsigned char memory_address, unsigned char *data, unsigned char size)
{
  uint8_t rc;
  unsigned int cnt;

  Wire.beginTransmission(_device_address);
  Wire.write(memory_address);
  Wire.write(data, size);
  rc = Wire.endTransmission();
}

void read(unsigned char memory_address, unsigned char *data, int size)
{
  uint8_t rc;
  unsigned char cnt;

  Wire.beginTransmission(_device_address);
  Wire.write(memory_address);
  rc = Wire.endTransmission(false);
  
  Wire.requestFrom((int)_device_address, (int)size, (int)true);
  cnt = 0;
  while(Wire.available()) {
    data[cnt] = Wire.read();
    cnt++;
  }
}