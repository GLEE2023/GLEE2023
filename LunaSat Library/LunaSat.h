//First Draft LunaSat
#include <Arduino.h>
#include <Wire.h>
#include "AK09940.h"
#include "ICM20602.h"
#include "tmp117.h"
#include "Adafruit_Sensor.h"
#include "LunaSat.h"

class LunaSat {

    public:
        LunaSat();// LunaSat Constructors

        string senserList [7]; 
        float readings [7]; //TODO Fix hardcoaded #readings
        unsigned long time;
        void getData();
        void printData();

    private:
};
