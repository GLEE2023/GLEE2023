//First Draft LunaSat
#include <Arduino.h>
#include "AK09940.h"
#include "ICM20602.h"
#include "tmp117.h"
#include "Adafruit_Sensor.h"

class LunaSat {
    public:
        LunaSat();              // LunaSat Constructors
        unsigned long time;
        string sensorList [7];  
        float readings [7];     //TODO Fix hardcoaded #readings
        int nSensors;
        void getData();
        void printData();
    private:
};
