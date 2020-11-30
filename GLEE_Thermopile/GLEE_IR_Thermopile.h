/*
*Description: This is an Arduino (C++) Header file required for the AK09940 Magnetometer
*Project Info: Created For GLEE (The Great Lunar Expedition for Everyone)
*Library Author: John Walker Johnson
*Library Contributors:
*Library Created on: July 13th 2020
*Last Modified on: October 7th 2020
*Primary Resources Used in Creation:
*TPiS 1S 1385 Datasheet
*Arduino Wire Library Reference Guide
*
*/

#ifndef GLEE_IR_THERMOPILE_H
#define GLEE_IR_THERMOPILE_H


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

#define IR_THERMOPILE_I2C_ADDR 0x0C             //I2C Sensor Address


struct CalibrationCoef
{
    int16_t sensorM;
    int16_t sensorPTAT25;

};

struct rawADC
{
    int_t
}

class Thermopile : public GLEE_Sensor        //inheritied class from the GLEE_Sensor
{
    public:
        IR_Thermopile();                        //constructor

        getCalibration();                       //Retreive and store calibration info to calibration data structure

        getRawADCs();                           //Retreive raw temperature ADC values

        getSensorTemperature();                 //Retreive most recent ADC Values and store in temperature data structure
        getObjectTemperature();                 //Retreive most recent ADC Values and store in temperature data structure

        tempCtoF();                             //converts a celcius output to kelvin

    private:

}


#endif