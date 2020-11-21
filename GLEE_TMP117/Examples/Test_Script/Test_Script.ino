#include <Arduino.h>
#include "GLEE_Sensor.h"
#include "tmp117.h"

TMP117 thermometer = TMP117(1);

void setup(){
    // All functions called for compile testing

    uint8_t addr_ = thermometer.getAddress();
    double tempC_ = thermometer.getTemperatureC();
    double tempF_ = thermometer.getTemperatureF();
    float uncertainty_ = thermometer.getUncertainty(22);
    thermometer.setLowLimit(44);
    float lowLimit_ = thermometer.getLowLimit();

    thermometer.setHighLimit(100);
    float highLimit_ = thermometer.getHighLimit();

    thermometer.reset();

    bool dataready_ = thermometer.dataReady();
    
    uint16_t read2Byte_ = thermometer.read2Byte(1);

    thermometer.write2Byte(1,1);
};

void loop(){

};