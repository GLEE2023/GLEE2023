//First Draft LunaSat
#include <Arduino.h>
#include "LunaSat.h"

TMP117 tmp117;
AK09940 ak09940;
ICM20602 icm20602;

LunaSat::LunaSat(){   //Constructor: Names LunaSat and Creates Object
  senserList = {"TMP117","ICM20602","AK09940"};
  tmp117 = new TMP117(); //TODO: Allow sensors to take applicable configurations as arguments
  icm20602 = new ICM20602();
  ak09940 = new AK09940();
}

LunaSat::getData(){
  //time = currentTime;
  time = millis();
  readings = {tmp117.getTemperatureC(), 
              icm20602.getAccelX(),
              icm20602.getAccelY(),
              icm20602.getAccelZ(),
              ak09940.getMagX(),
              ak09940.getMagY(),
              ak09940.getMagZ()};
}

LunaSat::printData(){
  Serial.print(time);
  Serial.print(",");
  for(int i = 0; i < nSensors-1; i++){
    Serial.print(",");
    Serial.print(readings[i],7);
  }
  Serial.println(readings[nSensors-1],7);
}
