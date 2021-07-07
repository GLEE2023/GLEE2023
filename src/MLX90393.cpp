/*
* Description: This is an Arduino (C++) .cpp file required for the MLX90393 Magnetometer
* Project Info: Created For GLEE (The Great Lunar Expedition for Everyone)
* Library Author: John Walker Johnson
* Library Contributors: Lawson Nerenberg
* Resources Used in Creation:
* MLX90393 Datasheet
*
*/

#include "MLX90393.h"

/**
 * Constructor
 * Parameters: {Sensor ID, Debug Flag}
 * Returns: none
 * This is the constructor for the MLX90393 class. 
**/
MLX90393::MLX90393(int _id, bool _debug){
    MLX90393::info.id = _id;
    MLX90393::info.name = "MLX90393 Magnetometer";		// assign sensor name
    MLX90393::info.address = MLX90393_SENSOR_ADDR;		// assign sensor i2c address
    MLX90393::sensorDebug= _debug;						// debug flag status
	MLX90393::FIFO = true;                               // FIFO true by default (enables dataready pin on sensor)
}

/**
 * Begin method
 * Contains pin mode and i2c buss enable and startup procedures
**/
void MLX90393::begin(){
    // Enable 1.8v-3.3v level shifter. Might want to implement at lunasat level TBD
    pinMode(A3,OUTPUT);
    digitalWrite(A3,HIGH);
    
    pinMode(7, INPUT); // Data ready signal pin

	Wire.begin(MLX90393::info.address);  // Begin i2c comms with sensor, only called once
}

/**
 * Initialization method
 * Contians initial configuration setup
**/
// void MLX90393::init(){
//     // Standard configuration
//         // Drive Mode: Low Noise 1
//         // Measurement mode: Power down 
//         // Watermark Level: 1
// }

void MLX90393::readWAI(void){
	uint8_t WAI1 = readByte(MLX90393_WAI_1);
	uint8_t WAI2 = readByte(MLX90393_WAI_2);

	Serial.print(F("Company ID: ")); Serial.println(WAI1, HEX);
	Serial.print(F("Device ID: ")); Serial.println(WAI2, HEX);
}

void MLX90393::setOpMode(bool fifo, MLX90393_Drive_Mode_t driveMode, MLX90393_Measurement_Mode_t measureMode){
	MLX90393::FIFO = fifo;
	MLX90393::driveMode = driveMode;
	MLX90393::measurementMode = measureMode;
	uint8_t writeData = ((uint8_t) fifo << 7 |  driveMode << 5 | measureMode);
	MLX90393::writeByte(MLX90393_CNTL_3, writeData);
}

AK_RawData_t MLX90393::getRawData(void){
	AK_RawData_t rawData;
	uint8_t dataBuf[3] = {0,0,0};

	MLX90393::readBytes(MLX90393_HX, 3, &dataBuf[0]);
	dataBuf[2] = dataBuf[2] && 0b00000011; // Remove higher 6 bits
	rawData.x = (uint32_t) ( dataBuf[2] << 16 | dataBuf[1] << 8 | dataBuf[0]);

	MLX90393::readBytes(MLX90393_HY, 3, &dataBuf[0]);
	dataBuf[2] = dataBuf[2] && 0b00000011; // Remove higher 6 bits
	rawData.y = (uint32_t) ( dataBuf[2] << 16 | dataBuf[1] << 8 | dataBuf[0]);

	MLX90393::readBytes(MLX90393_HZ, 3, &dataBuf[0]);
	dataBuf[2] = dataBuf[2] && 0b00000011; // Remove higher 6 bits
	rawData.z = (uint32_t) ( dataBuf[2] << 16 | dataBuf[1] << 8 | dataBuf[0]);

	rawData.temp = readByte(MLX90393_TMPS);

	return rawData;
}

float MLX90393::getTemperature(uint8_t rawTemp){
	float temp = 30.0f - ((float) rawTemp)/1.72f; // Conversion in datasheet
	return temp;
}

sensor_float_vec_t MLX90393::getMagnetic(AK_RawData_t rawData){
    sensor_float_vec_t magnetic;
    
    int32_t magX = rawData.x;
    int32_t magY = rawData.y;
    int32_t magZ = rawData.z;

    magnetic.x = magX * 10;
    magnetic.y = magY * 10;
    magnetic.z = magZ * 10;

    return magnetic;
}

float MLX90393::getMagFieldStrength(sensor_float_vec_t magnetic){
    return sqrt(pow(magnetic.x,2) + pow(magnetic.y,2) + pow(magnetic.z,2)); // L2 norm
}

bool MLX90393::dataReady(){
    if (digitalRead(7)) return true; // TODO: Change for const pin declaration
    else return false;
}

AK_Sample_t MLX90393::getSample(void){
    AK_Sample_t sample;
    
    MLX90393::setOpMode(true, LOW_POWER_1, SINGLE_MEASURE); // Only single measurement mode is currently supported
    delay(5); // Wait for single measurement to be taken

    if(MLX90393::dataReady()){
        AK_RawData_t rawData = MLX90393::getRawData();
        MLX90393::setOpMode(true, LOW_POWER_1, POWER_DOWN);
        sample.magnetic = MLX90393::getMagnetic(rawData);
        sample.strength = MLX90393::getMagFieldStrength(sample.magnetic);
        sample.temp = MLX90393::getTemperature(rawData.temp);
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