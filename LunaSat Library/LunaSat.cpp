//First Draft LunaSat
#include <Arduino.h>
#include "LunaSat.h"

TMP117 tmp117;
AK09940 ak09940;
ICM20602 icm20602;

//Constructor: Initializes lunasat and subsequent sensors
LunaSat::LunaSat(){                             
  sensorList = {"TMP117","ICM20602","AK09940"};
  nSensors = sensorList.length();
  tmp117 = new TMP117(); 
  icm20602 = new ICM20602();
  ak09940 = new AK09940();
  //TODO: Handle passing modes/configurations as arguments into sensor constructors
  //TODO: LunaSat class should have has its own begin function which begins transmission with other sensors)
}

LunaSat::getData(float time){
  //time = currentTime;
  readings = {time,
              tmp117.getTemperatureC(), 
              icm20602.getAccelX(),
              icm20602.getAccelY(),
              icm20602.getAccelZ(),
              ak09940.getMagX(),
              ak09940.getMagY(),
              ak09940.getMagZ()};
}

LunaSat::printData(){
  // Loop through array of readings 
  for(int i = 0; i < nSensors-1; i++){
    Serial.print(readings[i],7);
    Serial.print(",");
  }
  Serial.println(readings[nSensors-1],7);
}
