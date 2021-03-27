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

TMP117 tmp117;
AK09940 ak09940;
Adafruit_Si7021 si7021 = Adafruit_Si7021();
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);
Adafruit_LSM303AGR_Mag_Unified mag = Adafruit_LSM303AGR_Mag_Unified(12345);
//lsm9ds1 alread constructed as lsm9ds1
ICM20602 icm20602;

//CREATE VARIABLES TO STORE DATA

float tmp117Temp, si7021Temp, lsm9ds1AccelX, lsm9ds1AccelY, lsm9ds1AccelZ, lsm9ds1MagX, lsm9ds1MagY, lsm9ds1MagZ;
float ak09940MagX, ak09940MagY, ak09940MagZ, lsm303_AccelX, lsm303_AccelY, lsm303_AccelZ, lsm303_MagX, lsm303_MagY, lsm303_MagZ;
float icmAccelX, icmAccelY, icmAccelZ;
unsigned long currentTime;

void setup() {
  Serial.begin(9600);     //begin serial communication w/ LunaSat; baud=9600
  icm20602.begin();
  icm20602.initialize();
  ak09940.begin();
  ak09940.initialize();
  mag.enableAutoRange(false);
  accel.setMode(LSM303_MODE_HIGH_RESOLUTION);
  accel.setRange(LSM303_RANGE_8G);
  
  while (tmp117.begin()!=true && lsm9ds1.begin()!=true && si7021.begin()!=true && icm20602.isConnected()!=true && accel.begin()!=true && mag.begin()!=true && ak09940.isConnected()!=true)
  {
    delay(5);
    Serial.println("Waiting to Begin");
  }
  if (tmp117.begin() & si7021.begin() && icm20602.isConnected() && accel.begin() && mag.begin() && ak09940.isConnected() &&lsm9ds1.begin()) // Function to check if the sensor will correctly self-identify with the proper Device ID/Address
  {
    Serial.println("Begin");
  }
}

void loop() {
  currentTime=millis();
  sensors_event_t eventAccel;
  accel.getEvent(&eventAccel);
  lsm303_AccelX = eventAccel.acceleration.x;
  lsm303_AccelY = eventAccel.acceleration.y;        
  lsm303_AccelZ = eventAccel.acceleration.z;
  sensors_event_t eventMag;
  mag.getEvent(&eventMag); 
  lsm303_MagX = eventMag.magnetic.x;
  lsm303_MagY = eventMag.magnetic.y;        
  lsm303_MagZ = eventMag.magnetic.z;
  lsm9ds1.readAcceleration(lsm9ds1AccelX, lsm9ds1AccelY, lsm9ds1AccelZ);
  lsm9ds1.readMagneticField(lsm9ds1MagX, lsm9ds1MagY,lsm9ds1MagZ);
  tmp117Temp = tmp117.readTempC();           
  si7021Temp = si7021.readTemperature(); 
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
  Serial.print(si7021Temp,7);
  Serial.print(",");
  Serial.print(lsm303_AccelX,7);
  Serial.print(",");
  Serial.print(lsm303_AccelY,7);
  Serial.print(",");
  Serial.print(lsm303_AccelZ,7);
  Serial.print(",");
  Serial.print(lsm303_MagX,7);
  Serial.print(",");
  Serial.print(lsm303_MagY,7);
  Serial.print(",");
  Serial.print(lsm303_MagZ,7);
  Serial.print(",");
  Serial.print(lsm9ds1AccelX,7);
  Serial.print(",");
  Serial.print(lsm9ds1AccelY,7);
  Serial.print(",");
  Serial.print(lsm9ds1AccelZ,7);
  Serial.print(",");
  Serial.print(lsm9ds1MagX,7);
  Serial.print(",");
  Serial.print(lsm9ds1MagY,7);
  Serial.print(",");
  Serial.print(lsm9ds1MagZ,7);
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
