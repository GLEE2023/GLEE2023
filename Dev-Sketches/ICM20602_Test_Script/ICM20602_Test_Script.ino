#include <Arduino.h>
#include <Wire.h>
#include "ICM20602.h"

#define ICM20602_slave_addr 0x69

#define ICM20602_ACCEL_XOUT_H     0x3B
#define ICM20602_ACCEL_XOUT_L     0x3C
#define ICM20602_ACCEL_YOUT_H     0x3D
#define ICM20602_ACCEL_YOUT_L     0x3E
#define ICM20602_ACCEL_ZOUT_H     0x3F
#define ICM20602_ACCEL_ZOUT_L     0x40


ICM20602 icm20602;

float icmAccelX, icmAccelY, icmAccelZ;
unsigned long currentTime;

char H, L;
uint16_t Reading;

void setup() {
  Serial.begin(9600);     //begin serial communication w/ LunaSat; baud=9600
  
  delay(100);
  
  Wire.setClock(400000);
  Serial.println("Beginning Communication With ICM20602");
}

void loop() {
  currentTime=millis();
  Wire.requestFrom(ICM20602_slave_addr,4);
  while(Wire.available()){
    char byt = Wire.read();
    Serial.println(byt);
  }
  
  delay(1000);
}
