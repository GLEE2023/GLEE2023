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
 * Board: LunaSat V3
 * 
 * Description:This Arudino Sketch will be uploaded to LunaSat v2 
 * for the purpose of the DemoSat mission. This sketch will combine all available 
 * sensors and their libraries into one sketch in order to record 
 * 
 * 
 * Author: John Walker Johnson
 * Contributors:
 * Date Created:July 15th 2020
 * Date Modified:November 3rd 2020
 * 
 * CURRENT SERIAL OUTPUT FORMAL = .csv
*/

//CONSTRUCT SENSORS

TMP117 tmp117;
AK09940 ak09940;
ICM20602 icm20602;

//CREATE VARIABLES TO STORE DATA

float tmp117Temp;
float ak09940MagX, ak09940MagY, ak09940MagZ;
float icmAccelX, icmAccelY, icmAccelZ;
unsigned long currentTime;

void setup() {
  Serial.begin(9600);     //begin serial communication w/ LunaSat; baud=9600
  icm20602.begin();
  icm20602.initialize();
  ak09940.begin();
  ak09940.initialize();
  
  while (tmp117.begin()!=true && icm20602.isConnected()!=true && ak09940.isConnected()!=true)
  {
    delay(5);
    Serial.println("Waiting to Begin");
  }
  if (tmp117.begin() && icm20602.isConnected() && accel.begin() && mag.begin() && ak09940.isConnected() &&lsm9ds1.begin()) // Function to check if the sensor will correctly self-identify with the proper Device ID/Address
  {
    Serial.println("Begin");
  }
}

void loop() {
  currentTime=millis();
  tmp117Temp = tmp117.readTempC();           
  icmAccelX = icm20602.getAccelX();
  icmAccelY = icm20602.getAccelY();                                
  icmAccelZ = icm20602.getAccelZ();
  ak09940MagX=ak09940.getMagX();
  ak09940MagY=ak09940.getMagY();
  ak09940MagZ=ak09940.getMagZ();
  Serial.print(currentTime);
  Serial.print(",");
  Serial.print(tmp117Temp,7);
  Serial.print(",");
  Serial.print(ak09940MagX,7);
  Serial.print(",");
  Serial.print(ak09940MagY,7);
  Serial.print(",");
  Serial.print(ak09940MagZ,7);
  Serial.print(",");
  Serial.print(icmAccelX,7);
  Serial.print(",");
  Serial.print(icmAccelY,7);
  Serial.print(",");
  Serial.println(icmAccelZ,7);
  delay(500);
}
