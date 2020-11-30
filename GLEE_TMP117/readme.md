# GLEE_TMP117

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

