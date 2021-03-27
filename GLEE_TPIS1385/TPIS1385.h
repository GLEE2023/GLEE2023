/*
* Description: TPiS 1S 1385 Infrared Thermopile Class Header
* Project Info: Created in support of LunaSat as part of the GLEE (The Great Lunar Expedition for Everyone) mission and Artemis student challange.
* Library Contributors: John Walker Johnson, Lawson Nerenberg
* Primary Resources Used in Creation:
* TPiS 1S 1385 Datasheet (https://media.digikey.com/pdf/Data%20Sheets/Excelitas%20PDFs/TPiS_1S_1385.pdf)
* Arduino Wire Library Reference Guide
* Special Thanks to Kriswiners for CalPile Library inspiration 
*/

#ifndef TPIS1385_H
#define TPIS1385_H

#include "GLEE_Sensor.h"

/**
 * All definitions below allow us to use variable names, 
 * rather than integers, to adress the sensors internal memory registers. 
 * These registers are found on the data sheet and used to obtain readings 
 * an calibration constants from the TPIS sensor 
**/

// Ref: Page 12 of datasheet
#define TP_OBJECT 1                             // Object Temperature reg               (3xBytes) 17bit value [read] 
#define TP_AMBIENT 3                            // Ambient Temp Reg                     (2xBytes) 15bit value [read]
#define TP_OBJECT_LP1 5                         // Low Pass filter of object signal value 1 reg   (3xBytes) 20bit value [read] comparison: 8
#define TP_OBJECT_LP2 7                         // Low Pass filter of object signal value 2 reg   (3xBytes) 20bit value [read] comparison: 8
#define TP_AMBIENT_LP3 10                       // Low Pass filter of ambient signal value 3 reg  (2xBytes) 16bit value [read] comparison: 2
#define TP_OBJECT_LP2_FROZEN 12                 // Low pass filter of object on motion reg        (3xbytes) 24bit
#define TP_PRESENCE 15                          // READ
#define TP_MOTION 16                            // READ
#define TP_AMBIENT_SHOCK 17                     // READ
#define TP_INTERUPT_STATUS 18                   // READ
#define TP_CHIP_STATUS 19                       // READ
#define TP_LOW_PASS_TIME_CONST_LP1_LP2 20       // READ/WRITE
#define TP_LOW_PASS_TIME_CONST_LP3 21           // READ/WRITE
#define TP_PRESENCE_THRESHOLD 22                // READ/WRITE
#define TP_MOTION_THRESHOLD 23                  // READ/WRITE
#define TP_AMBIENT_SHOCK_THRESHOLD 24           // READ/WRITE
#define TP_INTERUPT_MASK 25                     // READ/WRITE
#define TP_MULTIPLE 26                          // READ/WRITE
#define TP_TIMER_INTERRUPT 27                   // READ/WRITE
#define TP_OT_THRESHOLD_HIGH 28                 // READ/WRITE

// Set to 0x80 for eeprom reads through red, set to 0x00 for no accsess
#define TP_EEPROM_CONTROL 31                    // 1byte READ/WRITE
#define TP_PROTOCOL 32                          // READ/WRITE
#define TP_CHSUM 33                             // READ
#define TP_LOOKUP 41                            // READ

#define TP_PTAT25 42                            // 2xbytes 15 bit value READ
#define TP_M 44                                 // 2xbytes 16 bit value requires RegVal/100 offset typically 172 counts / K READ
#define TP_U0 46                                // 2xbytes 16 bit value requires RegVal + 32768 offset READ
#define TP_UOUT1 48                             // 2xbytes 16 bit value
#define TP_T_OBJ_1 50                           // 1xbytes 

#define TP_I2C_ADDR 63  

/*
*End Register Definitions
*/

#define TPIS1385_I2C_ADDR 0x0D             //I2C Sensor Address

//Description of struct
struct CalibrationCoef{
    uint16_t M;
    uint16_t PTAT25;
    uint16_t U0;
    uint32_t UOut1;
    uint8_t TObj1;
    float K; // calibration const see sec 8.4
};

//Description of struct
/*
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
*/

//Description of struct
struct SensorADC{
    int32_t TP_OBJ;
    int32_t TP_AMB;
};

typedef struct{
    float object;
    float ambient;
}TPsample_t;


class TPIS1385 : public Sensor{                       //Inheritied class from the GLEE_Sensor
    public:
        TPIS1385(int _id, bool _debug = false);       //Constructor

        
        void readCalibration(void);                      //Retreive and store calibration info to calibration data structure

        void readADC(void);                             //Retreive raw temperature ADC values
        double getSensorTemperature(void);              //Retreive most recent ADC Values and store in temperature data structure
        double getObjectTemperature(void);              //Retreive most recent ADC Values and store in temperature data structure
        double convertTempKtoF(double inputTemp);       //Converts a kelvin output to fahrenheit
        void checkStatus(void);                         //Reads status registers
        void begin(void);

        void readEEprom(void);

        float getTamb(uint16_t TPamb);
        uint16_t getTPamb(void);

        uint32_t getTPobj(void);
        float getTobj(uint32_t TPobj, float Tamb);

        TPsample_t getSample(void);
        void updateSample(void);

    private:
        CalibrationCoef sensorCalibration;              //Structure containing the sensor Calibration Details
        //RawADC sensorRawADC;                            //Structure containing the sensors most recently read raw ADC values
        SensorADC sensorADC;
        uint8_t TEMPADDRESS;
        TPsample_t staticSample;
};


#endif