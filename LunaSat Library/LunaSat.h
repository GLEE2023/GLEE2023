//First Draft LunaSat
#include <Arduino.h>

class LunaSat {

    public:
        LunaSat();      // LunaSat Constructors

        float tmp117Temp;
        float icmAccelX, icmAccelY, icmAccelZ;
        float ak09940MagX, ak09940MagY, ak09940MagZ;
        unsigned long time;
        void getData();
        void printData();

    private:

};
