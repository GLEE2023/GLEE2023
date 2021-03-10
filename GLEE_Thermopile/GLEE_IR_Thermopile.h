/*
*Description: This is an Arduino (C++) Header file required for the TPiS 1S 1385 Infrared Thermopile
*Project Info: Created For GLEE (The Great Lunar Expedition for Everyone)
*Library Author: John Walker Johnson
*Library Contributors:
*Library Created on: November 13th 2020
*Last Modified on: November 30th 2020
*Primary Resources Used in Creation:
*TPiS 1S 1385 Datasheet
*Arduino Wire Library Reference Guide
*
*/

#ifndef GLEE_IR_THERMOPILE_H
#define GLEE_IR_THERMOPILE_H

#include "GLEE_Sensor.h"

/*
*All definitions below allow us to use variable names, rather than integers, to 
*adress the sensors internal memory registers. These registers are found on the data 
*sheet and used to 
*/

#define TP_OBJECT_HIGH 1                        //READ
#define TP_OBJECT_MID 2                         //READ
#define TP_OBJECT_LOW 3                         //READ
#define TP_AMBIENT_HIGH 3                       //READ
#define TP_AMBIENT_LOW 4                        //READ
#define TP_OBJECT_LP1_HIGH 5                    //READ
#define TP_OBJECT_LP1_MID 6                     //READ
#define TP_OBJECT_LP1_LOW 7                     //READ
#define TP_OBJECT_LP2_HIGH 7                    //READ
#define TP_OBJECT_LP2_MID 8                     //READ
#define TP_OBJECT_LP2_LOW 9                     //READ
#define TP_AMBIENT_LP3_HIGH 10                  //READ
#define TP_AMBIENT_LP3_LOW 11                   //READ
#define TP_OBJECT_LP2_FROZEN_HIGH 12            //READ
#define TP_OBJECT_LP2_FROZEN_MID 13             //READ
#define TP_OBJECT_LP2_FROZEN_LOW 14             //READ
#define TP_PRESENCE 15                          //READ
#define TP_MOTION 16                            //READ
#define TP_AMBIENT_SHOCK 17                     //READ
#define TP_INTERUPT_STATUS 18                   //READ
#define TP_CHIP_STATUS 19                       //READ
#define TP_LOW_PASS_TIME_CONST_LP1_LP2 20       //READ/WRITE
#define TP_LOW_PASS_TIME_CONST_LP3 21           //READ/WRITE
#define TP_PRESENCE_THRESHOLD 22                //READ/WRITE
#define TP_MOTION_THRESHOLD 23                  //READ/WRITE
#define TP_AMBIENT_SHOCK_THRESHOLD 24           //READ/WRITE
#define TP_INTERUPT_MASK 25                     //READ/WRITE
#define TP_MULTIPLE 26                          //READ/WRITE
#define TP_TIMER_INTERRUPT 27                   //READ/WRITE
#define TP_OT_THRESHOLD_HIGH 28                 //READ/WRITE
#define TP_OT_THRESHOLD_LOW 29                  //READ/WRITE
#define TP_EEPROM_CONTROL_REGISTER 31           //READ/WRITE
#define TP_PROTOCOL 32                          //READ/WRITE
#define TP_CHSUM_HIGH 33                        //READ
#define TP_CHSUM_LOW 34                         //READ
#define TP_LOOKUP 41                            //READ
#define TP_PTAT25_HIGH 42                       //READ
#define TP_PTAT25_LOW 43                        //READ
#define TP_M_HIGH 44                            //READ
#define TP_M_LOW 45                             //READ
#define TP_U0_HIGH 46                           //READ
#define TP_U0_LOW 47                            //READ
#define TP_UOUT1_HIGH 48                        //READ
#define TP_UOUT1_LOW 49                         //READ
#define TP_T_OBJ_1 50                           //READ
#define TP_I2C_ADDR 63  

/*
*End Register Definitions
*/

#define IR_THERMOPILE_I2C_ADDR 0x0D             //I2C Sensor Address

//Description of struct
struct CalibrationCoef{
    int16_t sensorM;
    int16_t sensorPTAT25;
    int16_t sensorU0;
    int16_t sensorUOut1;
};

//Description of struct
struct RawADC{
    int8_t TP_OBJ_ADC_H;
    int8_t TP_OBJ_ADC_M;       //Raw 17 bit ADC output of Object Temp
    int8_t TP_OBJ_ADC_L;

    int8_t TP_AMB_ADC_H;       //Raw 15 bit ambient temperature output
    int8_t TP_AMB_ADC_L;

    int8_t TP_OBJ_LP1_ADC_H;
    int8_t TP_OBJ_LP1_ADC_M;       //20 bit ADC output of Object Temp with Low-Pass Filter #1
    int8_t TP_OBJ_LP1_ADC_L;

    int8_t TP_OBJ_LP2_ADC_H;
    int8_t TP_OBJ_LP2_ADC_M;       //20 bit ADC output of Object Temp with Low-Pass Filter #2
    int8_t TP_OBJ_LP2_ADC_L;

    int8_t TP_AMB_LP3_ADC_H;    //16 bit ADC value of the Low pass filtered TP_AMB signal
    int8_t TP_AMB_LP3_ADC_L;
};

//Description of struct
struct SensorADC{
    int32_t TP_OBJ;
    int32_t TP_AMB;
};


class Thermopile : public Sensor{                   //Inheritied class from the GLEE_Sensor
    public:
        Thermopile();                                //Constructor

        void getCalibration(void);                      //Retreive and store calibration info to calibration data structure

        void readADC(void);                             //Retreive raw temperature ADC values

        double getSensorTemperature(void);              //Retreive most recent ADC Values and store in temperature data structure
        double getObjectTemperature(void);              //Retreive most recent ADC Values and store in temperature data structure

        double convertTempKtoF(double inputTemp);              //Converts a kelvin output to fahrenheit

        void checkStatus(void);                         //Reads status registers

    private:
        CalibrationCoef sensorCalibration;              //Structure containing the sensor Calibration Details
        RawADC sensorRawADC;                            //Structure containing the sensors most recently read raw ADC values
        SensorADC sensorADC;
        uint8_t TEMPADDRESS;
};


#endif