#include <Wire.h>
#include "Arduino.h"
#include "Lunar_I2C.h"

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

void setup (){
  Serial.begin(9600);
  Serial.println("Rohm BM1422AGMV Magnet Sensor sample");
  Wire.begin();

  unsigned char reg;
  unsigned char buf[2];

  //read(BM1422AGMV_WIA, &reg, sizeof(reg));

  // Step1
  reg = BM1422AGMV_CNTL1_VAL;
  Lunar_I2C::writeBytes(BM1422AGMV_DEVICE_ADDRESS_0E,BM1422AGMV_CNTL1, sizeof(reg), &reg);

  // Check 12bit or 14bit
  buf[0] = (BM1422AGMV_CNTL1_VAL & BM1422AGMV_CNTL1_OUT_BIT);
  if (buf[0] == BM1422AGMV_CNTL1_OUT_BIT) {
    _sens = BM1422AGMV_14BIT_SENS;
  } else {
    _sens = BM1422AGMV_12BIT_SENS;
  }

  delay(1);

  buf[0] = (BM1422AGMV_CNTL4_VAL >> 8) & 0xFF;
  buf[1] = (BM1422AGMV_CNTL4_VAL & 0xFF);
  // write(BM1422AGMV_CNTL4, buf, sizeof(buf));
  Lunar_I2C::writeBytes(BM1422AGMV_DEVICE_ADDRESS_0E,BM1422AGMV_CNTL4, sizeof(buf), buf);

  // Step2
  reg = BM1422AGMV_CNTL2_VAL;
  // write(BM1422AGMV_CNTL2, &reg, sizeof(reg));
  Lunar_I2C::writeByte(BM1422AGMV_DEVICE_ADDRESS_0E,BM1422AGMV_CNTL2, reg);


  // Step3
  // Option
  reg = BM1422AGMV_AVE_A_VAL;
  // write(BM1422AGMV_AVE_A, &reg, sizeof(reg));
  Lunar_I2C::writeByte(BM1422AGMV_DEVICE_ADDRESS_0E,BM1422AGMV_CNTL2,reg);
};


void loop (){
  float magData[3];
  unsigned char val[6];
  signed short rawdata[3];
  unsigned char reg = BM1422AGMV_CNTL3_VAL;


  Lunar_I2C::writeByte(BM1422AGMV_DEVICE_ADDRESS_0E,BM1422AGMV_CNTL3, reg);
  delay(2);
  Lunar_I2C::readBytes(BM1422AGMV_DEVICE_ADDRESS_0E,BM1422AGMV_DATAX, 6, val);

  rawdata[0] = ((signed short)val[1] << 8) | (val[0]);
  rawdata[1] = ((signed short)val[3] << 8) | (val[2]);
  rawdata[2] = ((signed short)val[5] << 8) | (val[4]);

  magData[0] = (float)rawdata[0] / _sens;
  magData[1] = (float)rawdata[1] / _sens;
  magData[2] = (float)rawdata[2] / _sens;

  Serial.print("BM1422AGMV XDATA=");
  Serial.print(magData[0], 3);
  Serial.println("[uT]");
  Serial.print("BM1422AGMV YDATA=");
  Serial.print(magData[1], 3);
  Serial.println("[uT]");
  Serial.print("BM1422AGMV ZDATA=");
  Serial.print(magData[2], 3);
  Serial.println("[uT]");
  Serial.println();

  delay(500);

};


//
// void write(unsigned char memory_address, unsigned char *data, unsigned char size)
// {
//   unsigned int cnt;
//
//   Wire.beginTransmission(BM1422AGMV_DEVICE_ADDRESS_0E);
//   Wire.write(memory_address);
//   Wire.write(data, size);
//   Wire.endTransmission();
// }
//
// void read(unsigned char memory_address, unsigned char *data, int size)
// {
//   unsigned char cnt;
//
//   Wire.beginTransmission(BM1422AGMV_DEVICE_ADDRESS_0E);
//   Wire.write(memory_address);
//   Wire.endTransmission(false);
//
//   Wire.requestFrom((int)BM1422AGMV_DEVICE_ADDRESS_0E, (int)size, (int)true);
//   cnt = 0;
//   while(Wire.available()) {
//     data[cnt] = Wire.read();
//     cnt++;
//   }
// }
