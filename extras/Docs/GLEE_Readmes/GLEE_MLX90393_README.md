# GLEE - LunaSat Magnetometer Sensor Library - GLEE_MLX90393 
This library is intended for GLEE's Magnetometer Sensor, the 
MLX90393.
Paired with GLEE's LunaSat object class, it can be used to monitor, configure and control 
the MLX90393. GLEE_MLX90393 supports the basic functionality of the magnetometer with reading 
and saving the three-axis magnetic field. [[[It has nine operation modes, including: power-down mode, single measurement mode, continuous measurement modes 1 through 6, and self-test mode and four drive modes with to of each of low noise mode and low power mode. More details about these sensor modes can be found below.]]]

## Magnetometer Data Sheet
[Melexis MLX90393](https://www.melexis.com/en/documents/documentation/datasheets/datasheet-mlx90393)

## Magnetometer Example
This example is for making basic magnetic field observations.
```c++
#include "MLX90393.h"

MLX90393 magnetometer = MLX90393();

AK_Sample_t sample;

void setup (){
    Serial.begin(9600);
    magnetometer.begin_I2C();
    magnetometer.setGain(MLX90393_GAIN_2_5X);
    magnetometer.setResolution(MLX90393_X, MLX90393_RES_19);
    magnetometer.setResolution(MLX90393_Y, MLX90393_RES_19);
    magnetometer.setResolution(MLX90393_Z, MLX90393_RES_16);

    // Set oversampling
    magnetometer.setOversampling(MLX90393_OSR_2);

    // Set digital filtering
    magnetometer.setFilter(MLX90393_FILTER_6);
    //magnetometer.getSensor(magnetometer);
    //magnetometer.setOpMode(true, LOW_POWER_1, POWER_DOWN);
    //Equivalent line:
};

void loop (){   
    float x, y, z;
    //Serial.print(F("Data Ready Pin Showing: ")); 
    //Serial.println(magnetometer.dataReady());

    sample = magnetometer.getSample();

    if (magnetometer.readData(&x, &y, &z)) {
        Serial.print("X: "); Serial.print(sample.magnetic.x, 4); Serial.println(" uT");
        Serial.print("Y: "); Serial.print(sample.magnetic.y, 4); Serial.println(" uT");
        Serial.print("Z: "); Serial.print(sample.magnetic.z, 4); Serial.println(" uT");
        Serial.print(F("Magnetic Field Magnitude: ")); 
        Serial.println(sample.strength);
    } else {
        Serial.println("Unable to read XYZ data from the sensor.");
    }
    delay(1000); // Take samples every one sec
};
```
# Registers

## Registers for Modes and Commands
| Register Name | Register Value (Hex) | Comments |
|---|---|---|
| MLX90393_REG_SB | 0x10 | Start burst mode |
| MLX90393_REG_SW | 0x20 | Start wakeup on change mode |
| MLX90393_REG_SM | 0x30 | Start single-measure mode |
| MLX90393_REG_RM | 0x40 | Read measurement |
| MLX90393_REG_RR | 0x50 | Read register |
| MLX90393_REG_WR | 0x60 | Write register |
| MLX90393_REG_EX | 0x80 | Exit Mode |
| MLX90393_REG_HR | 0xD0 | Memory recall |
| MLX90393_REG_HS | 0x70 | Memory store |
| MLX90393_REG_RT | 0xF0 | Reset |
| MLX90393_REG_NOP | 0x00 | NOP |

## Memory map
| Register | 15 | 14 | 13 | 12 | 11 | 10 | 9 | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| 0x00h | Z-Series | Gain_SEL | Gain_SEL | Gain_SEL | HALLCONF | HALLCONF | HALLCONF | HALLCONF | ANA_RESERVED_LOW | ANA_RESERVED_LOW | ANA_RESERVED_LOW | ANA_RESERVED_LOW | ANA_RESERVED_LOW | ANA_RESERVED_LOW | ANA_RESERVED_LOW | BIST |
| Ox01h | BURST_SEL (zyxt) | BURST_SEL (zyxt) | BURST_DATA_RATE (BDR) | BURST_DATA_RATE (BDR) | BURST_DATA_RATE (BDR) | BURST_DATA_RATE (BDR) | BURST_DATA_RATE (BDR) | BURST_DATA_RATE (BDR) | TRIG_INT | COMM_MODE | COMM_MODE | WOC_DIFF | EXT_TRG | TCMP_EN | BURST_SEL (zyxt) | BURST_SEL (zyxt) |
| 0x02h | RES_Y | RES_X | RES_X | DIG_FILT | DIG_FILT | DIG_FILT | OSR | OSR | | | | OSR2 | OSR2 | RES_Z | RES_Z | RES_Y |
| 0x03h | SENS_TC_LT | SENS_TC_LT | SENS_TC_LT | SENS_TC_LT | SENS_TC_LT | SENS_TC_LT | SENS_TC_LT | SENS_TC_LT | SENS_TC_HT | SENS_TC_HT | SENS_TC_HT | SENS_TC_HT | SENS_TC_HT | SENS_TC_HT | SENS_TC_HT | SENS_TC_HT |
| 0x04h | OFFSET_X | OFFSET_X | OFFSET_X | OFFSET_X | OFFSET_X | OFFSET_X | OFFSET_X | OFFSET_X | OFFSET_X | OFFSET_X | OFFSET_X | OFFSET_X | OFFSET_X | OFFSET_X | OFFSET_X | OFFSET_X |
| 0x05h | OFFSET_Y | OFFSET_Y | OFFSET_Y | OFFSET_Y | OFFSET_Y | OFFSET_Y | OFFSET_Y | OFFSET_Y | OFFSET_Y | OFFSET_Y | OFFSET_Y | OFFSET_Y | OFFSET_Y | OFFSET_Y | OFFSET_Y | OFFSET_Y |
| 0x06h | OFFSET_Z | OFFSET_Z | OFFSET_Z | OFFSET_Z | OFFSET_Z | OFFSET_Z | OFFSET_Z | OFFSET_Z | OFFSET_Z | OFFSET_Z | OFFSET_Z | OFFSET_Z | OFFSET_Z | OFFSET_Z | OFFSET_Z | OFFSET_Z |
| 0x07h | WOXY_THRESHOLD | WOXY_THRESHOLD | WOXY_THRESHOLD | WOXY_THRESHOLD | WOXY_THRESHOLD | WOXY_THRESHOLD | WOXY_THRESHOLD | WOXY_THRESHOLD | WOXY_THRESHOLD | WOXY_THRESHOLD | WOXY_THRESHOLD | WOXY_THRESHOLD | WOXY_THRESHOLD | WOXY_THRESHOLD | WOXY_THRESHOLD | WOXY_THRESHOLD |
| 0x08h | WOZ_THRESHOLD | WOZ_THRESHOLD | WOZ_THRESHOLD | WOZ_THRESHOLD | WOZ_THRESHOLD | WOZ_THRESHOLD | WOZ_THRESHOLD | WOZ_THRESHOLD | WOZ_THRESHOLD | WOZ_THRESHOLD | WOZ_THRESHOLD | WOZ_THRESHOLD | WOZ_THRESHOLD | WOZ_THRESHOLD | WOZ_THRESHOLD | WOZ_THRESHOLD |
| 0x09h | WOT_THRESHOLD | WOT_THRESHOLD | WOT_THRESHOLD | WOT_THRESHOLD | WOT_THRESHOLD | WOT_THRESHOLD | WOT_THRESHOLD | WOT_THRESHOLD | WOT_THRESHOLD | WOT_THRESHOLD | WOT_THRESHOLD | WOT_THRESHOLD | WOT_THRESHOLD | WOT_THRESHOLD | WOT_THRESHOLD | WOT_THRESHOLD |
| 0x0Ah...0x1Fh | FREE | FREE | FREE | FREE | FREE | FREE | FREE | FREE | FREE | FREE | FREE | FREE | FREE | FREE | FREE | FREE |



# Commands
| Command Name | Symbol | CMD1 byte | CMD2 byte | CMD3 byte | CMD4 byte |
|---|---|---|---|---|---|
| Start Burst Mode | SB | 0001 zyxt |
| Start Wake-up on Change Mode | SW | 0010 zyxt |
| Start Single Measurement Mode | SM | 0011 zyxt |
| Read Measurement | RM | 0100 zyxt |
| Read Register | RR | 0101 0abc | {A5...A0,0,0} |
| Write Register | WR | 0110 0abc | D15...D8 | D7...D0 | {A5...A0,0,0} |
| Exit Mode | EX | 1000 0000 |
| Memory Recall | HR | 1101 0000 |
| Memory Store | HS | 1110 0000 |
| Reset | RT | 1111 0000 |

## Status Byte
The first byte transmitted by the MLX90393 as a response to a command.

| 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
|---|---|---|---|---|---|---|---|
| BURST_MODE | WOC_MODE | SM_MODE | ERROR | SED | RS | D1 | D0 |


# Modes

## Communication Modes (COMM_MODE[1:0])
| Setting | Description |
|---|---|
| 0x0X | First valid command transmitted defines operating mode for all future commands until reset |
| 0x10 | SPI mode only |
| 0x11 | I2C mode only |

# Customized Data Types
| Data Type Name | Data Structure | Usage |
|---|---|---|
| mlx90393_gain | enum | Gain settings for CONF1 register |
| mlx90393_resolution | enum | Resolution settings for CONF3 register |
| mlx90393_axis | enum | Axis designator |
| mlx90393_filter | enum | Digital filter settings for CONF3 register |
| mlx90393_oversampling | enum | Oversampling settings for CONF3 register |
| sensors_event_t | 
| sensor_t | 



# Methods 
| Method Name | return type | args | Comments |
|---|---|---|---|
| begin_I2C | bool | uint8_t, TwoWire | Sets up hardware and initializes I2C |
| begin_SPI | bool | uint8_t, SPIClass | Sets up hardware and initializes hardware SPI |
| reset | bool | void | Perform a soft reset | 
| exitMode | bool | void | Perform a mode exit | 
| readMeasurement | bool | float, float, float | Reads data from data register & returns the results |
| startSingleMeasurement | bool | void | Begin a single measurement on all axes |
| setGain | bool | mlx90393_gain | Sets the sensor gain to the specified level |
| getGain | mlx90393_gain | void | Gets the current sensor gain |
| setResolution | bool | mlx90393_axis, mlx90393_resolution | Sets the sensor resolution to the specified level. |
| getResolution | mlx90393_resolution | mlx90393_axis | Gets the current sensor resolution | 
| setFilter | bool | mlx90393_filter | Sets the digital filter |
| getFilter | mlx90393_filter | void | Gets the current digital filter setting | 
| setOversampling | bool | mlx90393_oversampling | Sets the oversampling | 
| getOversampling | mlx90393_oversampling | void | Gets the current oversampling setting | 
| setTrigInt | bool | bool | Sets the TRIG_INT pin to the specified function | 
| readData | bool | float, float, float | Performs a single X/Y/Z conversion and returns the results |
| getEvent | sensors_event_t | bool | Gets the most recent sensor event, Adafruit Unified Sensor format | 
| getSensor | sensor_t | void | Gets the sensor_t device data, Adafruit Unified Sensor format |
| readRegister | bool | uint8_t, uint16_t |
| writeRegister | bool | uint8_t, uint16_t |
| _init | bool | void |
| transceive | uint8_t | uint8_t, uint8_t, uint8_t, uint8_t, uint8_t | Peforms a full read/write transcation with the sensor |



