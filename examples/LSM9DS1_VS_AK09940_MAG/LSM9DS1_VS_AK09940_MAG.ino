/*
  Arduino LSM9DS1 - Simple Magnetometer

  This example reads the magnetic field values from the LSM9DS1
  sensor and continuously prints them to the Serial Monitor
  or Serial Plotter.

  The circuit:
  - Arduino Nano 33 BLE Sense

  created 10 Jul 2019
  by Riccardo Rizzo

  This example code is in the public domain.
*/

#include <Arduino.h>
#include "AK09940.h"


AK09940 sensor;


void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Started");
  sensor.begin();
  while (!sensor.isConnected()) {
    delay(5);
    Serial.println("NOT CONNECTED");
  }
  Serial.println("***********");
  Serial.println();
  sensor.whoAmI();
  Serial.println("***********");
  Serial.println();
  sensor.initialize();

  if (!lsm9ds1.begin()) {
    Serial.println("Failed to initialize lsm9ds1!");
    while (1);
  }
  Serial.print("Magnetic field sample rate = ");
  Serial.print(lsm9ds1.magneticFieldSampleRate());
  Serial.println(" uT");
  Serial.println();
  Serial.println("Magnetic Field in uT");
  Serial.println("X\tY\tZ");
}

void loop() {
  float x, y, z;

  if (lsm9ds1.magneticFieldAvailable()) {
    lsm9ds1.readMagneticField(x, y, z);
      float magX, magY, magZ, temp;
  magX = sensor.getMagX();
  magY = sensor.getMagY();
  magZ = sensor.getMagZ();
  temp = sensor.getTemperature();
  Serial.print(", AK09940, ");
  Serial.print(magX);
  Serial.print(", ");
  Serial.print(magY);
  Serial.print(", ");
  Serial.print(magZ);
  Serial.print(", ");
  Serial.print((pow(pow(magX,2)+pow(magY,2)+pow(magZ,2),0.5)));
  Serial.print(", LSM9DS1, ");
  Serial.print(x*1000);
  Serial.print(", ");
  Serial.print(y*1000);
  Serial.print(", ");
  Serial.print(z*1000);
  Serial.print(", ");
  Serial.println((pow(pow(x*1000,2)+pow(y*1000,2)+pow(z*1000,2),0.5)));


  delay(2500);
  }
}
