//First Draft LunaSat
#include <Arduino.h>
#include "AK09940.h"
#include "ICM20602.h"
#include "tmp117.h"
#include "string.h"

class LunaSat {
    public:
        LunaSat();              // LunaSat Constructors
        unsigned long time;
        String sensorList [7];  
        float readings [7];     //TODO Fix hardcoaded #readings
        int nSensors;
        void getData();
        void printData();
        TMP117 tmp117;
        AK09940 ak09940;
        ICM20602 icm20602;
    private:
};
