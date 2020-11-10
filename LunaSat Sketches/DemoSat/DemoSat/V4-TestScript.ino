// Include necessary Librarys
// TODO: Make sure librarys arnt included twice unncecessarily (use ifdef as check)
#include <Arduino.h>
#include <Wire.h>
#include "AK09940.h"
#include "ICM20602.h"
#include "Adafruit_LSM303AGR_Mag.h"
#include "Adafruit_LSM303_Accel.h"
#include "SparkFun_TMP117.h"
#include "Adafruit_Si7021.h"
#include "Adafruit_Sensor.h"
#include "Arduino_LSM9DS1.h"

/*Project: GLEE DemoSat 
 * Board: LunaSat V2
 * 
 * Description:This Arudino Sketch will be uploaded to LunaSat v2 
 * for the purpose of the DemoSat mission. This sketch will combine all available 
 * sensors and their libraries into one sketch in order to record 
 * 
 * 
 * Author: John Walker Johnson
 * Contributors:
 * Date Created:July 15th 2020
 * Date Modified:July 31st 2020
 * 
 * CURRENT SERIAL OUTPUT FORMAL = .csv
*/

//CONSTRUCT SENSORS



unsigned long currentTime;
LunaSat lunaSat; 

void setup() {
  Serial.begin(9600);     //begin serial communication w/ LunaSat; baud=9600 //TODO Note what baud rate is determined by

  //This calls the constructor which inilializes the set of sensors and RF
  lunaSat = new LunaSat(MAGN,TEMP,ACCL,THERM) //Args: (List of Sensors,modes) 

  // Wait for lunasat to begin (TODO: lunasat has its own begin function that waits for sensors and rf to begin)
  while (lunaSat.begin()!=true){
    delay(250);
    Serial.println("Waiting to Begin");
  }

  Serial.println("Begin");
}

void loop() {
  currentTime=millis();
  
  delay(500);
}
