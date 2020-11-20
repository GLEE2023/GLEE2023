// GLEE I2C Sensor 
// Customized I2C scanner for use in debugging Lunasat



#include <Wire.h>

byte addr; // Byte representation of I2C addresses
byte err; // Byte to store error responses
int numDevices; // Accumulator for tracking number of found devices

void setup() {
  Wire.begin();
  Wire.setClock(100000);
  Serial.begin(9600);
  if(!Serial){
    delay(1000);
  }

  Serial.println("\nWelcome to the I2C Scan, I hope you find what your looking for");

}

void loop() {
  byte addr;
  int numDevices = 0;

  for (addr=1; addr<128; addr++){
    Wire.beginTransmission(addr);

    err = Wire.endTransmission();
    switch (err){
      case 0:
        Serial.print("Response from: 0x");
        if (addr < 10) Serial.print("0");
        Serial.println(addr, HEX);
        numDevices++;
        break;

      case 4:
        Serial.print("Error Response from 0x");
        if (addr < 10) Serial.print("0");
        Serial.println(addr, HEX);
        break;
    }
  }

  Serial.print("Number of devices found: ");
  Serial.println(numDevices);

  delay(10000);
}
