/*
* Description: This is an Arduino (C++) .cpp file required for the TPiS 1S 1385 IR Thermopile
* Project Info: Created For GLEE (The Great Lunar Expedition for Everyone)
* Base Library Author: John Walker Johnson and Lawson Nerenberg
* Resources Used in Creation:
* TPiS 1S 1385 Datasheet: https://www.pacer-usa.com/Assets/User/2077-CaliPile_TPiS_1S_1385_5029_27.04.2017.pdf
*
*/

#include "TPIS1385_Lunar.h"

/**
 * Constructor
 * Parameters: none
 * Returns: if initialization is satisfied.
 * This is a constructor for the IR_Thermopile class
 **/
TPIS1385_New::TPIS1385_New(int _id, bool _debug){
	TPIS1385_New::info.id = _id;
	TPIS1385_New::info.address = TPIS1385_I2C_ADDR;
	TPIS1385_New::info.name = "IRTpile";
    TPIS1385_New::sensorDebug = _debug;
	// Initalize Thermopile Sensor
}

/**
 * Begin comunications with TPIS1385_New
 * Parameters: none
 * Returns: none
 **/

void TPIS1385_New::begin(void){
    Wire.begin();                          // Begin i2c coms at standard speed
    Lunar_I2C::writeByte (RELOAD_ALL_CALL, FOUR, ZERO);
    delay(50);  // Wait on i2c transmission
}

/**
 * Reads EEprom
 * Parameters: None
 * Returns: None
 **/
void TPIS1385_New::readEEprom(void){
    uint8_t data[2] = {0,0};

    Lunar_I2C::writeByte (TPIS1385_I2C_ADDR, TP_EEPROM_CONTROL, 0x80);

    Lunar_I2C::readBytes(TPIS1385_I2C_ADDR, TP_PROTOCOL, 1, &data[0]);

    // Calibration Constants
    // Read PTAT25 calibration value
    Lunar_I2C::readBytes(TPIS1385_I2C_ADDR, TP_PTAT25, 2, &data[0]);
    TPIS1385_New::sensorCalibration.PTAT25 = ((uint16_t) data[0] << 8) | data[1];

    // Read M calibration value
    Lunar_I2C::readBytes(TPIS1385_I2C_ADDR, TP_M, 2, &data[0]);
    TPIS1385_New::sensorCalibration.M = ((uint16_t) data[0] << 8) | data[1];
    TPIS1385_New::sensorCalibration.M /= 100; // Apply appropriate offset

    // Read U0 calibration value
    Lunar_I2C::readBytes(TPIS1385_I2C_ADDR, TP_U0, 2, &data[0]);
    TPIS1385_New::sensorCalibration.U0 = ((uint16_t) data[0] << 8) | data[1];
    TPIS1385_New::sensorCalibration.U0 += 32768;

    // Read Uout1 calibration value
    Lunar_I2C::readBytes(TPIS1385_I2C_ADDR, TP_UOUT1, 2, &data[0]);
    TPIS1385_New::sensorCalibration.UOut1 = ((uint16_t) data[0] << 8) | data[1];
    TPIS1385_New::sensorCalibration.UOut1 *= 2;

    // Read Tobject 1 cal value

    Lunar_I2C::readBytes(TPIS1385_I2C_ADDR, TP_T_OBJ_1, 1, TPIS1385_New::sensorCalibration.TObj1);
    

    Lunar_I2C::writeByte (TPIS1385_I2C_ADDR, TP_EEPROM_CONTROL, 0x00);


    TPIS1385_New::sensorCalibration.K = ((float) (TPIS1385_New::sensorCalibration.UOut1 - TPIS1385_New::sensorCalibration.U0)/ (pow((float) (TPIS1385_New::sensorCalibration.TObj1 + 273.15f),3.8f) - pow(25.0f + 273.15f,3.8f)));

    #ifdef TPIS_DEBUG
        Serial.print(F("EEPROM Protocol: "));Serial.println(data[0]);
        Serial.print(F("PTAT25 Value: "));Serial.println(TPIS1385_New::sensorCalibration.PTAT25); 
        Serial.print(F("M Value: "));Serial.println(TPIS1385_New::sensorCalibration.M);
        Serial.print(F("U0 Value: "));Serial.println(TPIS1385_New::sensorCalibration.U0);
        Serial.print(F("UOut1 Value: "));Serial.println(TPIS1385_New::sensorCalibration.UOut1);
        Serial.print(F("T_obj_1 Value: "));Serial.println(TPIS1385_New::sensorCalibration.TObj1);
    #endif
}

/**
 * Get and return temp
 * Parameters: none
 * Returns: none
 **/
uint16_t TPIS1385_New::getTPamb(){
    uint8_t data[2] = {0,0};
    Lunar_I2C::readBytes(TPIS1385_I2C_ADDR, TP_AMBIENT, 2, &data[0]);
    uint16_t temp = ( (uint16_t)(data[0] & 0x7F) << 8) | data[1] ; // See data sheet for calc
    return temp;
}

/**
 * Gets Tamb
 * Parameters: TPamb
 * Returns: Tamb
 **/
float TPIS1385_New::getTamb(uint16_t TPamb){
    float temp = 298.15f + ((float) TPamb - (float) TPIS1385_New::sensorCalibration.PTAT25) * (1.0f/(float) TPIS1385_New::sensorCalibration.M);
    return temp;
}

/**
 * Gets TPobj
 * Parameters: None
 * Returns: TPobj
 **/
uint32_t TPIS1385_New::getTPobj(){
    uint8_t data[3] = {0,0,0};
    Lunar_I2C::readBytes(TPIS1385_I2C_ADDR, TP_OBJECT, 3, &data[0]);
    uint32_t temp = ( (uint32_t) ( (uint32_t)data[0] << 24) | ( (uint32_t)data[1] << 16) | ( (uint32_t)data[2] & 0x80) << 8) >> 15;
    return temp;
}

/**
 * Gets Tobj
 * Parameters: TPobj, Tamb
 * Returns: Tobj
 **/
float TPIS1385_New::getTobj(uint32_t TPobj, float Tamb){
    float f1 = pow(Tamb, 3.8f);
    float f2 = ( ((float) TPobj) - ((float) sensorCalibration.U0)  ) / sensorCalibration.K; // EQ. from datasheet
    return pow((f1 + f2), 0.2631578947f); // Magic constant for inverse root efficency (1/3.8 = 0.2631578947f)
}

/**
 * Gets corrected Tobj using emisivity
 * Parameters: emisivity
 * Returns: Corrected Tobj
 **/
float TPIS1385_New::getCorrectedTobj(uint32_t TPobj, float Tamb, float emi){
    float f1 = pow(Tamb, 3.8f);
    float f2 = ( ((float) TPobj) - ((float) sensorCalibration.U0)  ) / (sensorCalibration.K * emi); // EQ. from datasheet
    return pow((f1 + f2), 0.2631578947f); // Magic constant for inverse root efficency (1/3.8 = 0.2631578947f)
}

/**
 * Gets sample
 * Parameters: none
 * Returns: Sample
 **/
TPsample_t TPIS1385_New::getSample(){
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