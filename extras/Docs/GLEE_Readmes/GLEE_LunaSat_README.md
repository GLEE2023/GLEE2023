# GLEE LunaSat Integrated Functionality Library
This library puts all the basic functionality of the lunasat in one place. The lunasat library can be included and utilized to conduct quick and easy initialization of subsystems and controlled querying and transmission of sensor observations.

![LunaSat Front](/Docs/Images/LunaSat_front.jpg) ![LunaSat Back](/Docs/Images/LunaSat_back.jpg)

## Sensors

* TMP117:
  * Digital Temperature Sensor
  
* AK09940:
  * 3-axis magnetometer
  
* ICM20602:
  * 3-axis Magnetometer
  * 3-axis gyroscope (currently unused)
  * Digital Temperature 
  
* CAP:
  * Interdigitated analog capacitance Sensor
  
* TPIS1385
  * Contactless IR thermal sensor
  
# How To Use
Basic Sample Functionality Example
```C++
#include <LunaSat.h>    

// Set lunasat configuration (1's equates to default mode)
int lunaSatConfig[6] = {1,1,0,1,1,1}; // Configuration format: {TMP117, ICM20602, AK09940, TIPS1385, CAP, SX1272}

// LunaSat object initialization is used for declaration of parameters such as ID and debugging mode
LunaSat lunaSat(0, lunaSatConfig, false);

// Custom datatypes allow for sample specialization, user can craft their own ideal sample
lunaSat_sample_t sample;  

void setup() {
    // The GLEE library architecture enables easy, interpretable and familiar programming of the lunasat
    lunaSat.begin(9600);    // Direct serial communications with computer
    delay(5000);
}

void loop() {
    // Simple fetching of sensor observation using lunasat class 
    // Later versions will allow for dynamic sampling based on user defined config
    sample = lunaSat.getSample(); 

    // Simple examples of interacting with an observation sample
    lunaSat.dispSample(sample); // Observation samples can be directly displayed via serial
    
    delay(100);
}
```

Transmit Sample Example

```C++
#include <LunaSat.h>    


int lunaSatConfig[6] = {1,1,0,1,1,1}; // Configuration format: {TMP117, ICM20602, AK09940, TIPS1385, CAP, SX1272}

LunaSat lunaSat(0, lunaSatConfig, false);

lunaSat_sample_t sample;  

void setup() {
    lunaSat.begin(9600);   
    delay(5000);
}

void loop() {
    sample = lunaSat.getSample(); 

    // Dynamic sample transmission (string based on config)
    lunaSat.transmitSample(sample);
    
    delay(100);
}
```

### Lunasat Info
* ID: Lunasat identification number (helpfull when working with collections of lunasats)
* Name: String representing the name of the lunasat
* Sensor Config: Configurations parameters for sensors (TODO)
* RF Config: Wireless communication system configuration parameters (TODO)

# LunaSat Configuration
Default Configuration: {1,1,1,1,1,1}\
Configuration Description: All sensors enabled and in default single measurement mode 

### Configuration Format
* config[0] - TMP117 (Board Temperature Sensor)
* config[1] - ICM20602 (Inertial Measurement Unit)
* config[2] - AK09940 (Magnetometer)
* config[3] - TPiS1385 (IR Thermopile)
* config[4] - CAP (Capacitance Sensor) 
* config[5] - SX1272 (LoRA Transciever) 
