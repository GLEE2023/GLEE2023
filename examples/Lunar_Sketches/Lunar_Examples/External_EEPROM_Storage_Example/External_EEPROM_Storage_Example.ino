#include <Wire.h>

#define EXT_EEPROM_ADDR 0x50

void readBytes(uint8_t I2CsensorAddress, uint8_t registerAddress, uint8_t nBytes, uint8_t * data){
    Wire.beginTransmission(I2CsensorAddress);           // begins forming transmission to sensor
    Wire.write(registerAddress);                     // Add register address to transmission
    Wire.endTransmission();
    Wire.requestFrom(I2CsensorAddress, nBytes);         // Request and listen for response
    // Record response, wire will be available until nBytes are read
    int i = 0;
    while(Wire.available()){
        data[i] = Wire.read();
        i++;
    }
}

/**
 * Parameters: Register Address, Write Data
 * Returns: None
 * This function writes data to specified address
**/
void writeByte (uint8_t I2CsensorAddress, uint8_t registerAddress, uint8_t writeData){
    Wire.beginTransmission(I2CsensorAddress);               // begin communication with the sensor
    Wire.write(registerAddress);                                // point to address to be written to
    Wire.write(writeData);                                      // write data to adress specificed above
    Wire.endTransmission();                                     // end communication
}

void setup(){
  Serial.begin(9600);
  Wire.begin();
}

void loop(){

  //write byte
  uint8_t num = 0;
  while(true){

    delay(100);
    Serial.print(F("Writing 8 bit num "));
    Serial.print(num);
    Serial.println(F(" to external eeprom"));

    //Write to EEPROM
    Wire.beginTransmission(EXT_EEPROM_ADDR);
    Wire.write((uint8_t)0);//MSB
    Wire.write((uint8_t)0);//LSB
    Wire.write((uint8_t)num++);
    Serial.print("End transmission code: ");

    delay(1000);
    Serial.println(Wire.endTransmission(true));
    // Serial.println(Wire.endTransmission());

    delay(1000);

    //Read from eeprom
    uint8_t readNum = 77;
    Wire.beginTransmission(EXT_EEPROM_ADDR);
    Wire.write((uint8_t)0); //MSB
    Wire.write((uint8_t)0); //LSB
    Serial.print("End transmission code: ");
    Serial.println(Wire.endTransmission());
    Wire.requestFrom(EXT_EEPROM_ADDR,1);
    readNum = Wire.read();

    Serial.print(F("Read "));
    Serial.print(readNum);
    Serial.println(F(" from eeprom"));

    delay(2000);
  }
}
