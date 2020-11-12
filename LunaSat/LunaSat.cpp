//First Draft LunaSat
#include <Arduino.h>
#include "LunaSat.h"

TMP117 tmp117;
AK09940 ak09940;
ICM20602 icm20602;

//Constructor: Initializes lunasat and subsequent sensors
LunaSat::LunaSat(){                             
  TMP117 tmp117(); 
  ICM20602 icm20602();
  AK09940 ak09940();
  info.lunaSat_id = 0; // Default set to zero 
  info.lunasat_config = {1,1,1}; // 1,1,1 = All sensors connected;

  //TODO: Handle passing modes/configurations as arguments into sensor constructors
  //TODO: LunaSat class should have has its own begin function which begins transmission with other sensors)
}

lunaSat_data_t LunaSat::getData(){
  lunaSat_data_t sample;
  sample.timeStamp = millis();
  sample.temp = tmp117.getTemperatureC();
  sample.acc = icm20602.getData();
  sample.mag = ak09940.getData();
  return sample;
}

LunaSat::printData(){
  // Loop through array of readings 
  Serial.print(data.timeStamp);
  Serial.print(',');
  Serial.print(data.temp);
}
