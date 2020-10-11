#include <Wire.h>
#include <Arduino.h>


#define TMP117_RESOLUTION (double)0.0078125

enum TMP117_REG{
  TMP117_TEMP_REG = 0x00
  TMP117_CONFIG_REG = 0x01
  TMP117_HIGH_LIMIT_REG = 0x02
  TMP117_LOW_LIMIT_REG = 0x03
  TMP117_EEPROM_UL_REG = 0x04
  TMP117_EEPROM1_REG = 0x05
  TMP117_EEPROM2_REG = 0x06
  TMP117_TEMP_OFFSET_REG = 0x07
  TMP117_EEPROM3_REG = 0x08
  TMP117_DEVICE_ID = 0x0F
}

class TMP117 : public Sensor
{

    public:
        TMP117(uint8_t addr);
        uint8_t getAddress();

        double getTemperatureC();
        double getTemperatureF();

        float getUncertainty(float inputTemperature);

        void setLowLimit(float lowLimit);
        float getLowLimit();
        void setHighLimit(float highLimit);
        float getHighLimit();

        bool getHighAlert();
      	bool getLowAlert();

        void reset();

        bool dataReady();
  };

  #endif
