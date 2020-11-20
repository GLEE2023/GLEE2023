## Baseline Configuration Register Addresses ##
- **slave_addr** : **0x69** 
- **PWR_MGMT_1** : **0x6B** 
- **PWR_MGMT_2** : **0x6C** 
- **CONFIG** : **0x1A** 
- **GYRO_CONFIG** : **0x1B**
- **ACCEL_CONFIG** : **0x1C**

## Configuration details ##

# Default PWR MGMT 1 Configuration #
- **PWR_MGMT_1** = **DEVICE_RESET, SLEEP, CYCLE, GYRO_STANDBY, TEMP_DIS, CLKSEL** = **0 0 0 0 0 001**

# Default PWR MGMT 2 Configuration #
- **PWR_MGMT_2** = **Reserved, Reserved, STBY_XA, STBY_YA, STBY_ZA, STBY_XG, STBY_YG, STBY_ZG** = **0 0 1 1 1 0 0 0**

'''cpp
#define ICM20602_slave_addr       0x69
#define ICM20602_PWR_MGMT_1       0x6B  
#define ICM20602_PWR_MGMT_2       0x6C
#define ICM20602_CONFIG           0x1A
#define ICM20602_GYRO_CONFIG      0x1B
#define ICM20602_ACCEL_CONFIG     0x1C

#define ICM20602_ACCEL_XOUT_H     0x3B
#define ICM20602_ACCEL_XOUT_L     0x3C
#define ICM20602_ACCEL_YOUT_H     0x3D
#define ICM20602_ACCEL_YOUT_L     0x3E
#define ICM20602_ACCEL_ZOUT_H     0x3F
#define ICM20602_ACCEL_ZOUT_L     0x40

#define ICM20602_WHO_AM_I         0x75
'''