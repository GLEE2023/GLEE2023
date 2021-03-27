/*
* Description: This is an Arduino (C++) .cpp file required for the AK09940 Magnetometer
* Project Info: Created For GLEE (The Great Lunar Expedition for Everyone)
* Library Author: John Walker Johnson
* Library Contributors: Lawson Nerenberg
* Resources Used in Creation:
* AK09940 Datasheet
*
*/

#include "AK09940.h"

/**
 * Constructor
 * Parameters: {Sensor ID, Debug Flag}
 * Returns: none
 * This is the constructor for the AK09940 class. 
**/
AK09940::AK09940(int _id, bool _debug){
    AK09940::info.id = _id;
    AK09940::info.name = "AK09940 Magnetometer";		// assign sensor name
    AK09940::info.address = AK09940_SENSOR_ADDR;		// assign sensor i2c address
    AK09940::sensorDebug= _debug;						// debug flag status
	AK09940::FIFO = true;                               // FIFO true by default (enables dataready pin on sensor)
}

/**
 * Begin method
 * Contains pin mode and i2c buss enable and startup procedures
**/
void AK09940::begin(){
    // Enable 1.8v-3.3v level shifter. Might want to implement at lunasat level TBD
    pinMode(A3,OUTPUT);
    digitalWrite(A3,HIGH);
    
    pinMode(7, INPUT); // Data ready signal pin

	Wire.begin(AK09940::info.address);  // Begin i2c comms with sensor, only called once
}

/**
 * Initialization method
 * Contians initial configuration setup
**/
// void AK09940::init(){
//     // Standard configuration
//         // Drive Mode: Low Noise 1
//         // Measurement mode: Power down 
//         // Watermark Level: 1
// }

void AK09940::readWAI(void){
	uint8_t WAI1 = readByte(AK09940_WAI_1);
	uint8_t WAI2 = readByte(AK09940_WAI_2);

	Serial.print(F("Company ID: ")); Serial.println(WAI1, HEX);
	Serial.print(F("Device ID: ")); Serial.println(WAI2, HEX);
}

void AK09940::setOpMode(bool fifo, ak09940_Drive_Mode_t driveMode, ak09940_Measurement_Mode_t measureMode){
	AK09940::FIFO = fifo;
	AK09940::driveMode = driveMode;
	AK09940::measurementMode = measureMode;
	uint8_t writeData = ((uint8_t) fifo << 7 |  driveMode << 5 | measureMode);
	AK09940::writeByte(AK09940_CNTL_3, writeData);
}

AK_RawData_t AK09940::getRawData(void){
	AK_RawData_t rawData;
	uint8_t dataBuf[3] = {0,0,0};

	AK09940::readBytes(AK09940_HX, 3, &dataBuf[0]);
	dataBuf[2] = dataBuf[2] && 0b00000011; // Remove higher 6 bits
	rawData.x = (uint32_t) ( dataBuf[2] << 16 | dataBuf[1] << 8 | dataBuf[0]);

	AK09940::readBytes(AK09940_HY, 3, &dataBuf[0]);
	dataBuf[2] = dataBuf[2] && 0b00000011; // Remove higher 6 bits
	rawData.y = (uint32_t) ( dataBuf[2] << 16 | dataBuf[1] << 8 | dataBuf[0]);

	AK09940::readBytes(AK09940_HZ, 3, &dataBuf[0]);
	dataBuf[2] = dataBuf[2] && 0b00000011; // Remove higher 6 bits
	rawData.z = (uint32_t) ( dataBuf[2] << 16 | dataBuf[1] << 8 | dataBuf[0]);

	rawData.temp = readByte(AK09940_TMPS);

	return rawData;
}

float AK09940::getTemperature(uint8_t rawTemp){
	float temp = 30.0f - ((float) rawTemp)/1.72f; // Conversion in datasheet
	return temp;
}

sensor_float_vec_t AK09940::getMagnetic(AK_RawData_t rawData){
    sensor_float_vec_t magnetic;
    
    int32_t magX = rawData.x;
    int32_t magY = rawData.y;
    int32_t magZ = rawData.z;

    magnetic.x = magX * 10;
    magnetic.y = magY * 10;
    magnetic.z = magZ * 10;

    return magnetic;
}

float AK09940::getMagFieldStrength(sensor_float_vec_t magnetic){
    return sqrt(pow(magnetic.x,2) + pow(magnetic.y,2) + pow(magnetic.z,2)); // L2 norm
}

bool AK09940::dataReady(){
    if (digitalRead(7)) return true; // TODO: Change for const pin declaration
    else return false;
}

AK_Sample_t AK09940::getSample(void){
    AK_Sample_t sample;
    
    AK09940::setOpMode(true, LOW_POWER_1, SINGLE_MEASURE); // Only single measurement mode is currently supported
    delay(5); // Wait for single measurement to be taken

    if(AK09940::dataReady()){
        AK_RawData_t rawData = AK09940::getRawData();
        AK09940::setOpMode(true, LOW_POWER_1, POWER_DOWN);
        sample.magnetic = AK09940::getMagnetic(rawData);
        sample.strength = AK09940::getMagFieldStrength(sample.magnetic);
        sample.temp = AK09940::getTemperature(rawData.temp);
    }else{
        Serial.println(F("Waiting for data to get ready"));
    }

    return sample;
}   

// 9 Opperational Modes:
        // 1. Power Down: "00000"
        // 2. Single Measurement Mode: "00001"
            // Sensor measured once, data output, returns to power down
        // 3. 10Hz:"00010"
        // 4. 20Hz:"00100"
        // 5. 50Hz:"00110"
        // 6. 100Hz:"01000"
        // 7. 200Hz:"00110"
        // 8. 400Hz:"00110"
        // 9. Self Test:"10000"