# GLEE_AK09940
This library is intended for GLEE's Magnetometer Sensor, the 
AK09940.
Paired with GLEE's LunaSat object class, it can be used to monitor, configure and control 
the AK09940. GLEE_AK09940 supports the basic functionality of the magnetometer with reading 
and saving the three-axis magnetic field. It has nine operation modes, including: power-down mode, single measurement mode, continuous measurement modes 1 through 6, and self-test mode and four drive modes with to of each of low noise mode and low power mode. More details about these sensor modes can be found below.

## Baseline Configuration Registers
| Register Name | Register Value (Hex) | Comments |
|---|---|---|
| AK09940_WAI_1 | 0x00 | Who Am I Register #1- Company ID |
| AK09940_WAI_2 | 0x01 | Who Am I Register #2- Device ID |
| AK09940_ST_1 | 0x10 | Status Register- Data Status 1 |
| AK09940_ST_2 | 0x1B | Status Register- Data Status 2 |
| AK09940_TMPS | 0x1A | Temperature Measurement Data |
| AK09940_CNTL_1 | 0x30 | Control Settings #1 |
| AK09940_CNTL_2 | 0x31 | Control Settings #2 |
| AK09940_CNTL_3 | 0x32 | Control Settings #3 |
| AK09940_CNTL_4 | 0x33 | Control Settings #4 |

## Customized Data Types
| Data Type Name | Data Structure | Usage |
|---|---|---|
| ak09940_RawData_s | struct | Holds raw sensors readings of temperature and the magnetic field in all three axis |
| ak09940_CalculatedData_s | struct | Holds processed temperature and magnetic field in all three axis |
| ak09940_DataStatus_s | struct | Holds the two data status- data valid and data overflow |
| ak09940_Measurement_Mode_t | enum | Holds measurement modes |
| ak09940_Drive_Mode_ | enum | Holds drive mode options |

## Configuration and Operation Mode Details
# Sensor Modes (MODE[4:0]) 
| Operation Mode | Name in Library | Setting | Description |
|---|---|---|---|
| Power-Down | POWER_DOWN | 0b00000 | Power to almost all internal circuits is turned off |
| Single Measurement | SINGLE_MEASURE | 0b00001 | Measures sensor for one time and data is output. Transits to power-down mode automatically after measurement ended. |
| Continuous Measurement 1 | CONT_MEASURE_1 | 0b00010 | 10 Hz, Magnetic sensor measurement is started periodically and stored |
| Continuous Measurement 2 | CONT_MEASURE_2 | 0b00100 | 20 Hz, Magnetic sensor measurement is started periodically and stored |
| Continuous Measurement 3 | CONT_MEASURE_3 | 0b00110 | 50 Hz, Magnetic sensor measurement is started periodically and stored |
| Continuous Measurement 4 | CONT_MEASURE_4 | 0b01000 | 100 Hz, Magnetic sensor measurement is started periodically and stored |
| Continuous Measurement 5 | CONT_MEASURE_5 | 0b01010 | 200 Hz, Magnetic sensor measurement is started periodically and stored |
| Continuous Measurement 6 | CONT_MEASURE_6 | 0b01100 | 400 Hz, Magnetic sensor measurement is started periodically and stored |
| Self Test | SELF_TEST | 0b10000 | Used to check if the magnetic sensor is working normally |

# Drive Modes (MT[1:0])
| Drive Mode Name | Name in Library | Setting | Description |
|---|---|---|---|
| Low Noise 1 | LOW_NOISE_1 | 0b00 | Used to save the current consumption |
| Low Noise 2 | LOW_NOISE_2 | 0b01 | Used to save the current consumption |
| Low Power 1 | LOW_POWER_1 | 0b10 | Used to reduce the noise of the AK09940 |
| Low Power 2 | LOW_POWER_2 | 0b11 | Used to reduce the noise of the AK09940 |


## Methods 
| Method Name | return type | args | Comments |
|---|---|---|---|
| ak09940WAI | bool | none | Who Am I function to check or proper sensor initialition |
| softReset | void | none | Resets all the registers to the values defined |
| interpret18BitAs32Bit | int32_t | int32_t | Converts 18 bit magnetic sensor output to 32 bit output for ardiuno|
| getRawData | bool | none | Reads in and stores the raw data |
| getrawData | sensor_float_vec_t | none | Gets raw data returns raw data in sensor float structure for testing |
| getRawData_fuzzed | sensor_float_vec_t | none | Gets raw data returns raw data in sensor float structure for testing |
| dataReady | bool | none | Checks whether or not the data is ready to be read in |
| checkDataStatus | void | none | Verifies that the data was correctly read, with no data overwrites or gaps in measurements |
| setCalcultedData | void | none | converts raw data bytes to nT and Celcius |
| setMeasurementMode | void | ak09940_Measurement_Mode_t | sets the measurement mode |
| getMeasurementMode | ak09940_Measurement_Mode_t | none | gets the current measurement mode |
| setWatermarkMode | void | uint8_t | Sets water mark level (between 0 and 7) |
| getWatermarkMode | uint8_t | none | Gets currrent ater mark mode |
| setDriveMode | void | ak09940_Drive_Mode_t | Sets the drive mode |
| getDriveMode | ak09940_Drive_Mode_t | none | Gets the current drive mode |
| setFIFO | void | bool | Sets the state of the FIFO buffer |
| getFIFOState | bool | void | Gets whether the FIFO state is enabled or disabled |
| getNumDataInBuffer | uint8_t | void | Gets the number of data sets currently stored in FIFO buffer |
| setDebugMode | void | bool | Sets the sensor to debug mode |


