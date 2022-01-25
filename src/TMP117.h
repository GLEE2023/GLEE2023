#ifndef TMP117_H
#define TMP117_H

#include "GLEE_Sensor.h"

#define TMP117_RESOLUTION (double)0.0078125

enum TMP117_Register{
    TMP117_TEMP_REG = 0X00,
    TMP117_CONFIG_REG = 0x01,
    TMP117_HIGH_LIMIT_REG = 0X02,
    TMP117_LOW_LIMIT_REG = 0X03,
    TMP117_EEPROM_UL_REG = 0X04,
    TMP117_EEPROM1_REG  = 0X05,
    TMP117_EEPROM2_REG  = 0X06,
    TMP117_TEMP_OFFSET_REG = 0X07,
    TMP117_EEPROM3_REG = 0X08,
    TMP117_DEVICE_ID = 0X0F
};

class TMP117:public Sensor{
    public:
        TMP117(int _id, bool _debug = false); //Constructor
        
        bool dataReady(); //Check if data is ready
    
        //Get Temperature
        double getTemperatureC();     
        double getTemperatureF();
    
        //Get Uncertainty
        float getUncertainty(float inputTemperature);
        
        //Set and Get Low Limits
        void setLowLimit(float lowLimit);
        float getLowLimit();
    
        //Set and Get High Limits
        void setHighLimit(float highLimit);
        float getHighLimit();
        
        //Get Low and High Alerts
        bool getHighAlert();
        bool getLowAlert();
        
        //Reset
        void reset();
    
        //Get Address
        uint8_t getAddress();
    
        //Read and write 2 bytes
        uint16_t read2Byte(uint8_t registerAddress);
        void write2Byte(uint8_t reg, uint16_t data); 
};
#endif
