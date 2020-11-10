//First Draft LunaSat

/*
This cpp file is a part of the LunaSat library. This file provides the functions necessary
for the utilization the LunaSat library and vital for the operation of the LunaSat.
This project was created for the Great Lunar Expedition for Everyone (GLEE), a distributed
lunar science project by the Colorado Space Grant Consortium.
Author: John Walker Johnson
Contributors:
Date Created: September 10th 2020
Date Revised:September 11th 2020
*/
#include <Arduino.h>
#include <Wire.h>
#include "AK09940.h"
#include "ICM20602.h"
#include "tmp117.h"
#include "Adafruit_Sensor.h"
#include "LunaSat.h"

TMP117 tmp117;
AK09940 ak09940;
ICM20602 icm20602;

LunaSat::LunaSat(){   //Constructor: Names LunaSat and Creates Object
}

LunaSat::getData(){
  //time = currentTime;
  time = millis();
  tmp117Temp = tmp117.getTemperatureC();
  icmAccelX = icm20602.getAccelX();
  icmAccelY = icm20602.getAccelY();
  icmAccelZ = icm20602.getAccelZ();
  ak09940MagX=ak09940.getMagX();
  ak09940MagY=ak09940.getMagY();
  ak09940MagZ=ak09940.getMagZ();
}

LunaSat::printData(){
  Serial.print(time);
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
}
