//First Draft LunaSat
#include <Arduino.h>
#include "AK09940.h"
#include "ICM20602.h"
#include "tmp117.h"

typedef struct {
    float x;
    float y;
    float z;
} vec_t;

typedef struct {
    int32_t lunaSat_id;
    bool lunaSat_config[3];
    //int32_t lunaSat_role; // Container for role of lunasat
} lunaSat_info_t;

typedef struct {
    unsigned long timeStamp;
    float temp;
    vec_t acc;
    vec_t mag;
} lunaSat_data_t;

class LunaSat {
    public:
        LunaSat();              // LunaSat Constructors
        void getData();
        void printData();
        unsigned long time;
        info_t info;
        data_t data;
        TMP117 tmp117;
        AK09940 ak09940;
        ICM20602 icm20602;
    private:
};



