# GLEE_TMP117 Sensor Library

[![High Precision Temperature Sensor Datasheet- TMP117](https://www.digikey.com/-/media/Images/Product%20Highlights/T/Texas%20Instruments/TMP117%20Digital%20Temperature%20Sensor/ti-tmp117-digital-temp-sensor-200.jpg?ts=27501a08-04eb-4c59-8cad-756afe4e53b8&la=en-US)](https://cdn.sparkfun.com/assets/9/e/5/1/8/tmp117.pdf)

[High Precision Temperature Sensor Datasheet- TMP117](https://cdn.sparkfun.com/assets/9/e/5/1/8/tmp117.pdf)

This library is intended for GLEE's Temperature Sensor, the TMP117. Paired with the GLEE's LunaSat object class, it can be used to monitor, configure and control the TMP117. GLEE_TMP117 supports basic functionality such as Celsius/Fahrenheit temperature readouts and low/high temperature alerts. 

## Registers
	
| Register Name | Register Value (Hex) | Comments  |
|---|---|---|
| TMP117_TEMP_REG				| 0X00 | Digital Temperature Reading Register  |  
| TMP117_CONFIG_REG     | 0x01 | Configuration register |   
| TMP117_HIGH_LIMIT_REG | 0X02 | High limit register for monitoring specific states |
| TMP117_LOW_LIMIT_REG  | 0X03 | Low Limit Register |
| TMP117_EEPROM_UL_REG 	| 0X04 | TODO |
| TMP117_EEPROM1_REG  	| 0X05 | TODO |
| TMP117_EEPROM2_REG  	| 0X06 | TODO |
| TMP117_TEMP_OFFSET_REG| 0X07 | TODO |
| TMP117_EEPROM3_REG  	| 0X08 | TODO |
| TMP117_DEVICE_ID  		| 0X0F | TODO |

## Methods 

| Method Name | return type | args | Comments  |
|---|---|---|---|
| dataReady | bool | void | Returns true or false on wether data is ready via looking at config register |
| getTemperatureC | double | void | Returns temperature processed temperature in degrees C |
| getTemperatureF | double | void | Returns temperature processed temperature in degrees F |
| getUncertainty | float | float Temperature Input | Takes an input temperature and returns calculated uncertainty |
| setLowLimit | void | float Low Limit | Sets low limit |
| getLowLimit | float | float Low Limit | Returns low limit |
| setHighLimit | void | float High Limit | Sets high limit |
| getHighimit | float | float High Limit | Returns high limit |
| getHighAlert | bool | void | Passes high alert register after reading |
| getLowAlert | bool | void | Pass low alert register after |
| getAddress | uint8_t | void | Returns the I2C address of the temperature sensor |
| read2Byte | uint16_t | uint8 Register Address | Reads and returns 2 bytes as subg 16bit unsigned integer. |
| right2Byte | uint16_t | uint8 Register Address, uint16_t Data | Sets bits of data at 8bit register address. |

## Examples

GLEE_TMP117 - Dedicated Temperature Sesnor Functionality
- Board temperature observations
```C++
        #include "TMP117.h"

        // Instantiate TMP117 class with id of 1
        TMP117 thermometer(1);

        float temperature; 

        void setup(){
            Serial.begin(9600);
        };

        void loop(){
            temperature = thermometer.getTemperatureF();
            
            Serial.print("Temperature (C): "); Serial.println(temperature);

            delay(1000);
        };
```
