/*
*Description: This is an Arduino (C++) .cpp file required for the TPiS 1S 1385 IR Thermopile
*Project Info: Created For GLEE (The Great Lunar Expedition for Everyone)
*Base Library Author: John Walker Johnson 
*Resources Used in Creation:
*TPiS 1S 1385 Datasheet: https://www.pacer-usa.com/Assets/User/2077-CaliPile_TPiS_1S_1385_5029_27.04.2017.pdf
*
*/


#include "TPIS1385.h"

/** 
 * Constructor
 * Parameters: none
 * Returns: if initialization is satisfied.
 * This is a constructor for the IR_Thermopile class
 **/ 
TPIS1385::TPIS1385(int _id, bool _debug){
	TPIS1385::info.id = _id;     
	TPIS1385::info.address = TPIS1385_I2C_ADDR;
	TPIS1385::info.name = "IRTpile";
    TPIS1385::sensorDebug = _debug;
	// Initalize Thermopile Sensor
}

/**
 * Begin comunications with TPIS1385
 * Parameters: none
 * Returns: gets the calibration value
 **/ 

void TPIS1385::begin(void){
    Wire.begin();                          // Begin i2c coms at standard speed
	Wire.beginTransmission(0x00);    // Reload all call   
    Wire.write(0x04);
    Wire.write(0x00);         
	if(Wire.endTransmission()!=0) Serial.println(F("Init call failiure"));
    delay(50);  // Wait on i2c transmission
    Serial.println(F("TPIS coms Init complete"));
}

/**
 * Depricated
 * Parameters: none
 * Returns: gets the calibration value
 * TODO: ADD DESCRIPTION
 **/ 

/*
void TPIS1385::readCalibration(void){
    

    
    TPIS1385::sensorCalibration.sensorM = (sensorM_High << 8 && sensorM_Low)/100;

    int8_t sensorPTAT_High = TPIS1385::readByte(TP_PTAT25_HIGH);
    int8_t sensorPTAT_Low = TPIS1385::readByte(TP_PTAT25_LOW);

    TPIS1385::sensorCalibration.sensorPTAT25 = (sensorPTAT_High << 8 && sensorPTAT_Low);

    int8_t sensorU0_High = TPIS1385::readByte(TP_U0_HIGH);
    int8_t sensorU0_Low = TPIS1385::readByte(TP_U0_LOW);

    TPIS1385::sensorCalibration.sensorU0 = (sensorU0_High << 8 && sensorU0_Low) + 32768;

    int8_t sensorUOut1_High = TPIS1385 :: readByte (TP_UOUT1_HIGH);
    int8_t sensorUOut1_Low = TPIS1385 :: readByte (TP_UOUT1_LOW);

    TPIS1385 :: sensorCalibration.sensorUOut1 = (sensorUOut1_High << 8 && sensorUOut1_Low) * 2;
}
*/
/**
 * Depricated
 * Parameters: none
 * Returns: gets the calibration value
 * TODO: ADD DESCRIPTION
 **/ 

/*
void TPIS1385::readADC(void){
    int8_t sensorTP_OBJ_H = TPIS1385::readByte (TP_OBJECT_HIGH);
    int8_t sensorTP_OBJ_M = TPIS1385::readByte (TP_OBJECT_MID);
    int8_t sensorTP_OBJ_L = TPIS1385::readByte (TP_OBJECT_LOW);
    TPIS1385::sensorADC.TP_OBJ = (sensorTP_OBJ_H << 9 && sensorTP_OBJ_M << 1 && sensorTP_OBJ_L >> 7);


    int8_t sensorTP_AMB_H = TPIS1385::readByte (TP_AMBIENT_HIGH);
    int8_t sensorTP_AMB_L = TPIS1385::readByte (TP_AMBIENT_LOW);

    TPIS1385::sensorADC.TP_AMB = ((sensorTP_AMB_H && 0b01111111) << 8 && sensorTP_AMB_L);  
}
*/

/**
 * Depricated
 * Parameters: none
 * Returns: gets the sensors temperature 
 * This function takes the TPIS1385 calibration values
 * to return the sensor temperature.
 **/ 
/*
double TPIS1385::getSensorTemperature(void){
        double tAmb = (25.0 + 273.15) + (TPIS1385::sensorADC.TP_OBJ - TPIS1385::sensorCalibration.sensorPTAT25) * (1/TPIS1385::sensorCalibration.sensorM);
        return tAmb;
}
*/
/**
 * Depricated
 * Parameters: none
 * Returns: object temperature 
 * This function takes the TPIS1385 calibration values 
 * and the sensor temperature to find the objects temperature.
 **/ 
/*
double TPIS1385::getObjectTemperature(void){
        double tObj = pow((TPIS1385::sensorADC.TP_OBJ - TPIS1385::sensorCalibration.sensorU0 + pow(TPIS1385::getSensorTemperature(), 3.8)), 1/3.8);
        return tObj;
}
*/

/**
 * Depricated
 * Parameters: double inputTemp
 * Returns: outputs fahrenheint
 * This function converts a kelvin output to a 
 * fahrenheit output.
 **/ 
//double Thermopile::tempKtoF(double inputTemp){}

/**
 * Depricated
 * Parameters: none
 * Returns: the statust of the thermopile.
 * This functions reads the status registers.
 **/ 
//void Thermopile::checkStatus(void){}

void TPIS1385::readEEprom(void){
    uint8_t data[2] = {0,0};

    TPIS1385::writeByte(TP_EEPROM_CONTROL, 0x80); // Set eeprom control to read

    data[0] = TPIS1385::readByte(TP_PROTOCOL);
    Serial.print(F("EEPROM Protocol: "));
    Serial.println(data[0]);

    // Calibration Constants

    // Read PTAT25 calibration value
    readBytes(TP_PTAT25, 2, &data[0]);
    TPIS1385::sensorCalibration.PTAT25 = ((uint16_t) data[0] << 8) | data[1]; 
    
    Serial.print(F("PTAT25 Value: "));
    Serial.println(TPIS1385::sensorCalibration.PTAT25);

    // Read M calibration value
    readBytes(TP_M, 2, &data[0]);
    TPIS1385::sensorCalibration.M = ((uint16_t) data[0] << 8) | data[1]; 
    TPIS1385::sensorCalibration.M /= 100; // Apply appropriate offset

    Serial.print(F("M Value: "));
    Serial.println(TPIS1385::sensorCalibration.M);

    // Read U0 calibration value
    readBytes(TP_U0, 2, &data[0]);
    TPIS1385::sensorCalibration.U0 = ((uint16_t) data[0] << 8) | data[1];
    TPIS1385::sensorCalibration.U0 += 32768;

    Serial.print(F("U0 Value: "));
    Serial.println(TPIS1385::sensorCalibration.U0);

    // Read Uout1 calibration value
    readBytes(TP_UOUT1, 2, &data[0]);
    TPIS1385::sensorCalibration.UOut1 = ((uint16_t) data[0] << 8) | data[1];
    TPIS1385::sensorCalibration.UOut1 *= 2;

    Serial.print(F("UOut1 Value: "));
    Serial.println(TPIS1385::sensorCalibration.UOut1);

    // Read Tobject 1 cal value
    TPIS1385::sensorCalibration.TObj1 = TPIS1385::readByte(TP_T_OBJ_1);
    Serial.print(F("T_obj_1 Value: "));
    Serial.println(TPIS1385::sensorCalibration.TObj1);

    TPIS1385::writeByte(TP_EEPROM_CONTROL, 0x00); // Stop reading from eeprom

    TPIS1385::sensorCalibration.K = ((float) (TPIS1385::sensorCalibration.UOut1 - TPIS1385::sensorCalibration.U0)/ (pow((float) (TPIS1385::sensorCalibration.TObj1 + 273.15f),3.8f) - pow(25.0f + 273.15f,3.8f)));
}

uint16_t TPIS1385::getTPamb(){
    uint8_t data[2] = {0,0};
    readBytes(TP_AMBIENT, 2, &data[0]);
    uint16_t temp = ( (uint16_t)(data[0] & 0x7F) << 8) | data[1] ; // See data sheet for calc
    return temp;
}

float TPIS1385::getTamb(uint16_t TPamb){
    float temp = 298.15f + ((float) TPamb - (float) TPIS1385::sensorCalibration.PTAT25) * (1.0f/(float) TPIS1385::sensorCalibration.M);
    return temp;
}

uint32_t TPIS1385::getTPobj(){
    uint8_t data[3] = {0,0,0};
    readBytes(TP_OBJECT, 3, &data[0]);
    uint16_t temp = ( (uint32_t) ( (uint32_t)data[0] << 24) | ( (uint32_t)data[1] << 16) | ( (uint32_t)data[2] & 0x80) << 8) >> 15; 
    return temp;
}

float TPIS1385::getTobj(uint32_t TPobj, float Tamb){

    float f1 = pow(Tamb, 3.8f);
    float f2 = ( ((float) TPobj) - ((float) sensorCalibration.U0)  ) / sensorCalibration.K;
    return pow((f1 + f2), 0.2631578947f); // Magic constant for inverse root efficency (1/3.8 = 0.2631578947f)
}

TPsample_t TPIS1385::getSample(){
    TPsample_t sample; // observation sample to be returned
    
    // Build Sample
    uint16_t TPamb = getTPamb();
    uint32_t TPobj = getTPobj();
    uint16_t Tamb = getTamb(TPamb);
    float Tobj = getTobj(TPobj, Tamb);
    
    sample.object = Tobj - SENSOR_CONV_K_to_C; 
    sample.ambient = Tamb - SENSOR_CONV_K_to_C; // Ambient temperature sample in deg c (k to c conversion needed)
    
    return sample;
}

void TPIS1385::updateSample(){
    staticSample = TPIS1385::getSample();
}