// LunaSat Class contains core lunasat functionality such as transmission and sensing
#include "LunaSat.h"

/**
 * Constructor
 * Parameters: the sensor ID as an integer, array containg active functionality,
 * and the debug mode as a booelan
 * Returns: none
 * This is the constructor for the Capacitive class. 
 * Initializes lunsat and subsequent sensors.
**/
LunaSat::LunaSat(int _id, int _conf[6], bool _debug){                             
    // Set lunaSat info
    info.id = _id;
    
    for(int i = 0; i < 6; i++){ // Might need to replace this with more optimal refrencing
        info.conf[i] = _conf[i];
    }

    // Initialize sensors

    // Set private variables
    debug = _debug;
    
    Serial.println(F("Done initalization"));

    // Set indicator LED pin modes
    // pinMode(LED1, OUTPUT);
    // pinMode(LED2, OUTPUT);

    //TODO: Handle passing modes/configurations as arguments into sensor constructors
    //TODO: LunaSat class should have has its own begin function which begins transmission with other sensors)

}

/**
 * Parameters: int baudRate
 * Returns: if serial communications have began.
 * This function starts serial communication at 
 * given baud rate then sensor begins initialization.
**/
void LunaSat::begin(int baudRate){
    // The begin function starts serial communications at the given baud rate.
    Serial.begin(baudRate);
    delay(5);
    if (debug){
        Serial.println(F("Serial Coms Active"));
    }

    // Sensor specific begins and initializations
    if (info.conf[1]){
        // LunaSat::mpu6000->begin();
        // LunaSat::mpu6000->initialize();
        // LunaSat::mpu6000->setAccelRange(MPU6000_RANGE_2_G);

        // Depricated 5.0 Accel Init
        // LunaSat::icm20602->begin();
        // LunaSat::icm20602->initialize();

        if (debug) Serial.println(F("Accel Initialized"));
    }

    if (info.conf[2]==1) {
        // LunaSat::mlx90393->begin_I2C();
        // LunaSat::mlx90393->setGain(MLX90393_GAIN_2_5X);
        // LunaSat::mlx90393->setResolution(MLX90393_X, MLX90393_RES_19);
        // LunaSat::mlx90393->setResolution(MLX90393_Y, MLX90393_RES_19);
        // LunaSat::mlx90393->setResolution(MLX90393_Z, MLX90393_RES_16);
        // LunaSat::mlx90393->setOversampling(MLX90393_OSR_2);
        // LunaSat::mlx90393->setFilter(MLX90393_FILTER_6);

       if (debug) Serial.println(F("Mag Initialized"));
    } 

    if (info.conf[3]==1){
        LunaSat::tpis1385->begin();
        LunaSat::tpis1385->readEEprom();

        if (debug) Serial.println(F("T-Pile Initialized"));
    }

    if (info.conf[4]==1){
        LunaSat::cap->begin();
        if (debug) Serial.println(F("Cap Initialized"));
    }
    if(info.conf[5]==1){
        // Default radio initialization
        LunaSat::rad.initialize_radio();
        delay(50);
        if (debug) Serial.println(F("Radio Initialized"));
    }
}

lunaSat_sample_t LunaSat::getSample(void){
    lunaSat_sample_t sample;

    sample.timeStamp = millis();

    // Handle Temperature Sensor Sample base on configuration
    if (info.conf[0] == 1){
        sample.TMPtemperature = LunaSat::tmp117->getTemperatureC();
    }else{
        sample.TMPtemperature = 0;
    }

    // Handle acceleration sample based on configuration
    if (info.conf[1] == 1){
        
        //sample.acceleration = LunaSat::mpu6000->getSample();
        
        // 5.0 ICM [deprecated]
        // sensor_int16_vec_t rawAccel = LunaSat::icm20602->getRawAccel();
        // sample.acceleration = LunaSat::icm20602->getGAccel(rawAccel);
    } else {
        sample.acceleration.x = 0;
        sample.acceleration.y = 0;
        sample.acceleration.z = 0;
    }

    // Handle Magnetometer
    if (info.conf[2] == 1){
        // sample.magnetic = LunaSat::mlx90393->getSample();
    } else {
        sample.magnetic.x = 0;
        sample.magnetic.y = 0;
        sample.magnetic.z = 0;
    }

    // Handle Thermopile Sample based on configuration
    if (info.conf[3] == 1){
        sample.TPTemperature = LunaSat::tpis1385->getSample();
    } else {
        sample.TPTemperature.ambient = 0;
        sample.TPTemperature.object = 0;
    }

    if (info.conf[4] == 1){
        sample.cap = LunaSat::cap->getRawData();
    } else {
        sample.cap = 0;
    }
        
    return sample;
}


/**
 * Parameters: lunaSat_sample_t sample
 * Returns: display of sample
 * This function displays the sample including
 * temperature, acceleration, and magnetic.
**/
void LunaSat::dispSample(lunaSat_sample_t sample){
    Serial.print(sample.timeStamp);
    Serial.print(',');
    
    if (info.conf[0]==1){
        Serial.print(sample.TMPtemperature,3);
        Serial.print(',');
    }
    if (info.conf[1]==1){
        Serial.print(sample.acceleration.x,5);
        Serial.print(',');
        Serial.print(sample.acceleration.y,5);
        Serial.print(',');
        Serial.print(sample.acceleration.z,5);
    }
    if (info.conf[2]==1){
        Serial.print(sample.magnetic.x);
        Serial.print(',');
        Serial.print(sample.magnetic.y);
        Serial.print(',');
        Serial.print(sample.magnetic.z);
    }
    if (info.conf[3]==1){
        Serial.print(',');
        Serial.print(sample.TPTemperature.object,5);
        Serial.print(',');
        Serial.print(sample.TPTemperature.ambient,5);
    }
    if (info.conf[4]==1){
        Serial.print(',');
        Serial.print(sample.cap);
    }
    Serial.println("");
}

void LunaSat::dispAccel(sensor_float_vec_t accel){
    Serial.print(accel.x,5);
    Serial.print(" ");
    Serial.print(accel.y,5);
    Serial.print(' ');
    Serial.println(accel.z,5);
}

void LunaSat::transmitSample(lunaSat_sample_t sample){
    char dataBuffer[46];
    String sampleString = String(sample.timeStamp);
    String sep = ",";

    if (info.conf[0]==1){
        sampleString = sampleString + sep;
        sampleString = sampleString + String(sample.TMPtemperature);
    }
    if (info.conf[1]==1){
        sampleString = sampleString + sep;
        sampleString = sampleString + String(sample.acceleration.x);
        sampleString = sampleString + sep;
        sampleString = sampleString + String(sample.acceleration.y);
        sampleString = sampleString + sep;
        sampleString = sampleString + String(sample.acceleration.z);
    }
    if (info.conf[2]==1){
        sampleString = sampleString + sep;
        sampleString = sampleString + String(sample.magnetic.x);
        sampleString = sampleString + sep;
        sampleString = sampleString + String(sample.magnetic.y);
        sampleString = sampleString + sep;
        sampleString = sampleString + String(sample.magnetic.z);

    }
    if (info.conf[3]==1){
        sampleString = sampleString + sep;
        sampleString = sampleString + String(sample.TPTemperature.ambient);
        sampleString = sampleString + sep;
        sampleString = sampleString + String(sample.TPTemperature.object);
    }
    if (info.conf[4]==1){
        sampleString = sampleString + sep;
        sampleString = sampleString + String(sample.cap);
    }

    sampleString.toCharArray(dataBuffer, 46);
    Serial.print("Sample: "); Serial.println(sampleString);
    LunaSat::rad.transmit_data(dataBuffer);
    delay(5);
}

void LunaSat::transmit_data(char *buff){
    LunaSat::rad.transmit_data(buff);
}

/**
 * Parameters: int _LED and int _delay
 * Returns: none
 * This function to make the lunasat blink
 * given delays between high and low. 
**/
void LunaSat::blink(int _LED, int _delay){
    int pin;
    
    if (_LED == 1){
        pin = LED1;
    }else if (_LED == 2){
        pin = LED2;
    }else{
        // Invalid Pin
    }

    digitalWrite(pin, HIGH);
    delay(_delay);
    digitalWrite(pin, LOW);
    delay(_delay);
}
