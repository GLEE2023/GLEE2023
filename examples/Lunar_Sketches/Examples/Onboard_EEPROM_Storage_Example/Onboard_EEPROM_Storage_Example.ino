/*
This script stores data from the temperature sensor to the built in EEPROM
storage on the LunaSat's ATmega328P then reads it afterward. Since EEPROM is
non-volitile, it will store the data even after being shutdown or restarted
*/
#include <EEPROM.h>
#include "TMP117.h"

#define NUM_MEASUREMENTS 100
#define BYTES_PER_MEAS 4

TMP117 thermometer(1);

void setup(){
  //begin serial comms
  Serial.begin(9600);
}

void loop(){
  //wait for user input to take and record measurements
  Serial.println("Press Enter To take measurements.");
  while(!Serial.available()){}
  Serial.readString();

  // take 100 float measurements from the temp sensor.
  // the address starts at 0, and is incremented by 4 because floats are 4 bytes
  for(int addr=0; addr<NUM_MEASUREMENTS*BYTES_PER_MEAS; addr+=BYTES_PER_MEAS){
    //get sample
    float tempSample = thermometer.getTemperatureC();
    EEPROM.put(addr,tempSample);
    /*
    EEPROM.write() is another option but will only write one byte of data (floats
    are 4 bytes!).

    EEPROM.put() will write as many bytes as necessary to write tempSample to EEPROM
    at the address specified by addr. put() knows how many bytes to write because
    the function put() is overlaoded with different data type inputs, therefore
    we are calling the function that takes in a datatype of float
    */

    Serial.print("."); //just to let the user know its running
    delay(100);
  }

  // Access values stored by the previous for loop
  Serial.println("Printing Values");
  for(int addr=0; addr<NUM_MEASUREMENTS*BYTES_PER_MEAS; addr+=BYTES_PER_MEAS){
    float val;
    EEPROM.get(addr,val);
    Serial.println(val);
  }

  Serial.println("Restarting");
}
