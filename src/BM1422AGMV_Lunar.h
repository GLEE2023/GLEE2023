#include <Arduino.h>
#include "Lunar_I2C.h"

#define BM1422AGMV_DEBUG

#ifndef _BM1422AGMV_H_
#define _BM1422AGMV_H_
#endif

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

namespace BM1422AGMV_Lunar{
  void setConfig(uint8_t config);
  void getData(uint8_t* buffer);
}
